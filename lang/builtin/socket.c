#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include "./lib/socket.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo SOCKET_MODULE[] = {
    {MODULE_SOCKET, SOCKET_CREATE, "__socket_create__", 1, 1},
    {MODULE_SOCKET, SOCKET_BIND, "__socket_bind__", 3, 3},
    {MODULE_SOCKET, SOCKET_LISTEN, "__socket_listen__", 1, 2},
    {MODULE_SOCKET, SOCKET_ACCEPT, "__socket_accept__", 1, 1},
    {MODULE_SOCKET, SOCKET_CONNECT, "__socket_connect__", 3, 3},
    {MODULE_SOCKET, SOCKET_SEND, "__socket_send__", 2, 2},
    {MODULE_SOCKET, SOCKET_RECV, "__socket_recv__", 1, 2},
    {MODULE_SOCKET, SOCKET_CLOSE, "__socket_close__", 1, 1},
    {MODULE_SOCKET, SOCKET_SETOPT, "__socket_setopt__", 3, 3},
    {MODULE_SOCKET, SOCKET_GETADDR, "__socket_getaddr__", 1, 1},
};

const int SOCKET_MODULE_COUNT = sizeof(SOCKET_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Socket Handle — stored as TYPE_FILE (reusing file handle)
//  We store the fd as a FILE* using fdopen for compatibility
//=========================================================

static const char *get_str(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE || d->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a string", funcname);
      return NULL;
   }
   return d->str;
}

static int get_int(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE || d->type != TYPE_INT)
   {
      throw_error(ERROR_TYPE, "%s() argument must be an integer", funcname);
      return -1;
   }
   return *(int *)d->atom;
}

// Store socket fd as TYPE_INT for simplicity
static Data *make_socket(int fd)
{
   return createData(TYPE_INT, &fd);
}

static int get_socket(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_INT)
      return d ? *(int *)d->atom : -1;
   throw_error(ERROR_TYPE, "%s() argument must be a socket", funcname);
   return -1;
}

//=========================================================
//  Implementations
//=========================================================

// create(type) -> socket
// Creates a new socket. type is "tcp" or "udp".
// Returns a socket handle (int file descriptor).
// Example: s = socket.create("tcp")
// Example: s = socket.create("udp")
Data *__socket_create__(Data *type)
{
   const char *t = get_str(type, "socket.create");
   if (!t)
      return createData(TYPE_NONE, NULL);

   int sock_type = SOCK_STREAM; // TCP default
   if (strcmp(t, "udp") == 0)
      sock_type = SOCK_DGRAM;
   else if (strcmp(t, "tcp") != 0)
   {
      throw_error(ERROR_VALUE, "socket.create(): type must be 'tcp' or 'udp'");
      return createData(TYPE_NONE, NULL);
   }

   int fd = socket(AF_INET, sock_type, 0);
   if (fd < 0)
   {
      throw_error(ERROR_RUNTIME, "socket.create(): %s", strerror(errno));
      return createData(TYPE_NONE, NULL);
   }

   return make_socket(fd);
}

// bind(socket, host, port) -> bool
// Binds a socket to a host and port.
// Use "0.0.0.0" to bind to all interfaces.
// Example: socket.bind(s, "0.0.0.0", 8080)
Data *__socket_bind__(Data *sock, Data *host, Data *port)
{
   int fd = get_socket(sock, "socket.bind");
   if (fd < 0)
      return createData(TYPE_BOOL, &(int){0});

   const char *h = get_str(host, "socket.bind");
   if (!h)
      return createData(TYPE_BOOL, &(int){0});

   int p = get_int(port, "socket.bind");
   if (p < 0)
      return createData(TYPE_BOOL, &(int){0});

   // Allow reuse of address
   int opt = 1;
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_port = htons((uint16_t)p);
   addr.sin_addr.s_addr = strcmp(h, "0.0.0.0") == 0
                              ? INADDR_ANY
                              : inet_addr(h);

   if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
   {
      throw_error(ERROR_RUNTIME, "socket.bind(): %s", strerror(errno));
      return createData(TYPE_BOOL, &(int){0});
   }

   return createData(TYPE_BOOL, &(int){1});
}

// listen(socket, backlog=5) -> bool
// Marks a socket as passive (server), ready to accept connections.
// backlog is the maximum number of pending connections.
// Example: socket.listen(s)
// Example: socket.listen(s, 10)
Data *__socket_listen__(Data *sock, Data *backlog)
{
   int fd = get_socket(sock, "socket.listen");
   if (fd < 0)
      return createData(TYPE_BOOL, &(int){0});

   int bl = 5;
   if (backlog && backlog->type == TYPE_INT)
      bl = *(int *)backlog->atom;

   if (listen(fd, bl) < 0)
   {
      throw_error(ERROR_RUNTIME, "socket.listen(): %s", strerror(errno));
      return createData(TYPE_BOOL, &(int){0});
   }

   return createData(TYPE_BOOL, &(int){1});
}

// accept(socket) -> dict
// Accepts an incoming connection on a listening socket.
// Returns dict: {"socket": int, "host": str, "port": int}
// Blocks until a connection arrives.
// Example: conn = socket.accept(s)
// Example: client_sock = conn["socket"]
// Example: client_host = conn["host"]
Data *__socket_accept__(Data *sock)
{
   int fd = get_socket(sock, "socket.accept");
   if (fd < 0)
      return createData(TYPE_NONE, NULL);

   struct sockaddr_in client_addr;
   socklen_t addr_len = sizeof(client_addr);

   int client_fd = accept(fd, (struct sockaddr *)&client_addr, &addr_len);
   if (client_fd < 0)
   {
      throw_error(ERROR_RUNTIME, "socket.accept(): %s", strerror(errno));
      return createData(TYPE_NONE, NULL);
   }

   char host[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, &client_addr.sin_addr, host, sizeof(host));
   int port = ntohs(client_addr.sin_port);

   Dict *result = dict_create(__size__);

   Data *sock_key = createData(TYPE_STR, "socket");
   Data *host_key = createData(TYPE_STR, "host");
   Data *port_key = createData(TYPE_STR, "port");

   dict_insert(sock_key, make_socket(client_fd), result);
   dict_insert(host_key, createData(TYPE_STR, strdup(host)), result);
   dict_insert(port_key, createData(TYPE_INT, &port), result);

   return createData(TYPE_DICT, result);
}

// connect(socket, host, port) -> bool
// Connects a socket to a remote host and port.
// Example: socket.connect(s, "example.com", 80)
Data *__socket_connect__(Data *sock, Data *host, Data *port)
{
   int fd = get_socket(sock, "socket.connect");
   if (fd < 0)
      return createData(TYPE_BOOL, &(int){0});

   const char *h = get_str(host, "socket.connect");
   if (!h)
      return createData(TYPE_BOOL, &(int){0});

   int p = get_int(port, "socket.connect");
   if (p < 0)
      return createData(TYPE_BOOL, &(int){0});

   // Resolve hostname
   struct hostent *server = gethostbyname(h);
   if (!server)
   {
      throw_error(ERROR_RUNTIME, "socket.connect(): cannot resolve host '%s'", h);
      return createData(TYPE_BOOL, &(int){0});
   }

   struct sockaddr_in addr;
   memset(&addr, 0, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_port = htons((uint16_t)p);
   memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);

   if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
   {
      throw_error(ERROR_RUNTIME, "socket.connect(): %s", strerror(errno));
      return createData(TYPE_BOOL, &(int){0});
   }

   return createData(TYPE_BOOL, &(int){1});
}

// send(socket, data) -> int
// Sends data through a connected socket.
// Returns the number of bytes sent.
// Example: bytes = socket.send(s, "Hello, World!")
// Example: bytes = socket.send(s, "GET / HTTP/1.0\r\n\r\n")
Data *__socket_send__(Data *sock, Data *data)
{
   int fd = get_socket(sock, "socket.send");
   if (fd < 0)
      return createData(TYPE_INT, &(int){-1});

   const char *msg = get_str(data, "socket.send");
   if (!msg)
      return createData(TYPE_INT, &(int){-1});

   int sent = (int)send(fd, msg, strlen(msg), 0);
   if (sent < 0)
   {
      throw_error(ERROR_RUNTIME, "socket.send(): %s", strerror(errno));
      return createData(TYPE_INT, &(int){-1});
   }

   return createData(TYPE_INT, &sent);
}

// recv(socket, size=4096) -> str
// Receives data from a connected socket.
// Returns the received data as a string, or "" on connection close.
// Blocks until data is available.
// Example: data = socket.recv(s)
// Example: data = socket.recv(s, 1024)
Data *__socket_recv__(Data *sock, Data *size)
{
   int fd = get_socket(sock, "socket.recv");
   if (fd < 0)
      return createData(TYPE_STR, strdup(""));

   int buf_size = 4096;
   if (size && size->type == TYPE_INT)
      buf_size = *(int *)size->atom;

   char *buf = malloc(buf_size + 1);
   int received = (int)recv(fd, buf, buf_size, 0);

   if (received < 0)
   {
      free(buf);
      throw_error(ERROR_RUNTIME, "socket.recv(): %s", strerror(errno));
      return createData(TYPE_STR, strdup(""));
   }

   buf[received] = '\0';
   return createData(TYPE_STR, buf);
}

// close(socket) -> none
// Closes a socket connection.
// Example: socket.close(s)
Data *__socket_close__(Data *sock)
{
   int fd = get_socket(sock, "socket.close");
   if (fd >= 0)
      close(fd);
   return createData(TYPE_NONE, NULL);
}

// setopt(socket, option, value) -> bool
// Sets a socket option.
// Options: "reuseaddr", "keepalive", "nodelay", "nonblocking", "timeout"
// Example: socket.setopt(s, "reuseaddr", true)
// Example: socket.setopt(s, "timeout", 5000)  # 5 second timeout in ms
Data *__socket_setopt__(Data *sock, Data *option, Data *value)
{
   int fd = get_socket(sock, "socket.setopt");
   if (fd < 0)
      return createData(TYPE_BOOL, &(int){0});

   const char *opt = get_str(option, "socket.setopt");
   if (!opt)
      return createData(TYPE_BOOL, &(int){0});

   if (strcmp(opt, "reuseaddr") == 0)
   {
      int v = value && value->type == TYPE_BOOL ? *(int *)value->atom : 1;
      setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));
   }
   else if (strcmp(opt, "keepalive") == 0)
   {
      int v = value && value->type == TYPE_BOOL ? *(int *)value->atom : 1;
      setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &v, sizeof(v));
   }
   else if (strcmp(opt, "nodelay") == 0)
   {
      int v = value && value->type == TYPE_BOOL ? *(int *)value->atom : 1;
      setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &v, sizeof(v));
   }
   else if (strcmp(opt, "nonblocking") == 0)
   {
      int flags = fcntl(fd, F_GETFL, 0);
      int v = value && value->type == TYPE_BOOL ? *(int *)value->atom : 1;
      if (v)
         fcntl(fd, F_SETFL, flags | O_NONBLOCK);
      else
         fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
   }
   else if (strcmp(opt, "timeout") == 0)
   {
      int ms = value && value->type == TYPE_INT ? *(int *)value->atom : 5000;
      struct timeval tv;
      tv.tv_sec = ms / 1000;
      tv.tv_usec = (ms % 1000) * 1000;
      setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
      setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
   }
   else
   {
      throw_error(ERROR_VALUE, "socket.setopt(): unknown option '%s'", opt);
      return createData(TYPE_BOOL, &(int){0});
   }

   return createData(TYPE_BOOL, &(int){1});
}

// getaddr(host) -> dict
// Resolves a hostname to an IP address.
// Returns dict: {"host": str, "ip": str}
// Example: info = socket.getaddr("example.com")
// Example: print(info["ip"])
Data *__socket_getaddr__(Data *host)
{
   const char *h = get_str(host, "socket.getaddr");
   if (!h)
      return createData(TYPE_NONE, NULL);

   struct hostent *he = gethostbyname(h);
   if (!he)
   {
      throw_error(ERROR_RUNTIME, "socket.getaddr(): cannot resolve '%s'", h);
      return createData(TYPE_NONE, NULL);
   }

   char ip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, he->h_addr_list[0], ip, sizeof(ip));

   Dict *result = dict_create(__size__);
   Data *host_key = createData(TYPE_STR, "host");
   Data *ip_key = createData(TYPE_STR, "ip");

   dict_insert(host_key, createData(TYPE_STR, strdup(h)), result);
   dict_insert(ip_key, createData(TYPE_STR, strdup(ip)), result);

   return createData(TYPE_DICT, result);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeSocketModule(Runtime *mod_rt)
{
   for (int i = 0; i < SOCKET_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)SOCKET_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&SOCKET_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchSocketBuiltin(SocketType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case SOCKET_CREATE:
      return __socket_create__(ARG(0));
   case SOCKET_BIND:
      return __socket_bind__(ARG(0), ARG(1), ARG(2));
   case SOCKET_LISTEN:
      return __socket_listen__(ARG(0), ARG(1));
   case SOCKET_ACCEPT:
      return __socket_accept__(ARG(0));
   case SOCKET_CONNECT:
      return __socket_connect__(ARG(0), ARG(1), ARG(2));
   case SOCKET_SEND:
      return __socket_send__(ARG(0), ARG(1));
   case SOCKET_RECV:
      return __socket_recv__(ARG(0), ARG(1));
   case SOCKET_CLOSE:
      return __socket_close__(ARG(0));
   case SOCKET_SETOPT:
      return __socket_setopt__(ARG(0), ARG(1), ARG(2));
   case SOCKET_GETADDR:
      return __socket_getaddr__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown socket builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}