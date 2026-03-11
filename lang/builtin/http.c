#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "./lib/http.h"

#include "../core/lib/type.h"
#include "../core/lib/error.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/exec.h"
#include "../core/lib/class.h"
#include "../core/lib/utils.h"

extern pthread_mutex_t g_interp_lock;
extern void interp_lock(void);
extern void interp_unlock(void);
Data *executeFunction(Data *func_data, List *args, Runtime *rt);

const ModuleFuncInfo HTTP_MODULE[] = {
    {MODULE_HTTP, HTTP_SERVER, "__http_server__", 0, 0},
    {MODULE_HTTP, HTTP_LISTEN, "__http_listen__", 2, 3},
    {MODULE_HTTP, HTTP_STOP, "__http_stop__", 1, 1},
    {MODULE_HTTP, HTTP_GET, "__http_get__", 3, 3},
    {MODULE_HTTP, HTTP_POST, "__http_post__", 3, 3},
    {MODULE_HTTP, HTTP_PUT, "__http_put__", 3, 3},
    {MODULE_HTTP, HTTP_DELETE, "__http_delete__", 3, 3},
    {MODULE_HTTP, HTTP_PATCH, "__http_patch__", 3, 3},
    {MODULE_HTTP, HTTP_USE, "__http_use__", 2, 2},
};

const int HTTP_MODULE_COUNT = sizeof(HTTP_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Route Table
//=========================================================

#define MAX_ROUTES 256
#define MAX_MIDDLEWARE 32

typedef struct Route
{
   char method[16];
   char path[256];
   Data *handler; // CX function
} Route;

typedef struct Server
{
   Route routes[MAX_ROUTES];
   int route_count;
   Data *middleware[MAX_MIDDLEWARE];
   int middleware_count;
   int fd; // server socket fd
   int running;
   int port;
   Runtime *rt;
} Server;

//=========================================================
//  Helper — get value from res dict without leaking key
//=========================================================
static Data *res_get(Dict *d, const char *key)
{
   Data *k = createData(TYPE_STR, (void *)key);
   Data *val = dict_get(k, d);
   data_free(k);
   return val;
}

//=========================================================
//  HTTP Parsing
//=========================================================

typedef struct
{
   char method[16];
   char path[256];
   char query[1024];
   char body[65536];
   char headers[4096];
   int content_length;
} HttpRequest;

static void parse_http_request(const char *raw, HttpRequest *req)
{
   memset(req, 0, sizeof(HttpRequest));

   // Parse request line: METHOD /path?query HTTP/1.1
   sscanf(raw, "%15s %255s", req->method, req->path);

   // Split path and query
   char *q = strchr(req->path, '?');
   if (q)
   {
      strncpy(req->query, q + 1, sizeof(req->query) - 1);
      *q = '\0';
   }

   // Find headers (after first line)
   const char *headers_start = strchr(raw, '\n');
   if (headers_start)
   {
      headers_start++;
      // Find body (after \r\n\r\n)
      const char *body_start = strstr(raw, "\r\n\r\n");
      if (body_start)
      {
         body_start += 4;
         int header_len = (int)(body_start - headers_start - 4);
         if (header_len > 0 && header_len < (int)sizeof(req->headers) - 1)
            strncpy(req->headers, headers_start, header_len);
         strncpy(req->body, body_start, sizeof(req->body) - 1);
      }
      else
      {
         strncpy(req->headers, headers_start, sizeof(req->headers) - 1);
      }
   }
}

static Dict *parse_query_string(const char *query)
{
   Dict *d = dict_create(8);
   if (!query || !*query)
      return d;

   char buf[1024];
   strncpy(buf, query, sizeof(buf) - 1);

   char *pair = strtok(buf, "&");
   while (pair)
   {
      char *eq = strchr(pair, '=');
      if (eq)
      {
         *eq = '\0';
         Data *k = createData(TYPE_STR, pair);
         Data *v = createData(TYPE_STR, eq + 1);
         dict_insert(k, v, d);
      }
      pair = strtok(NULL, "&");
   }
   return d;
}

static Dict *parse_headers_dict(const char *headers_str)
{
   Dict *d = dict_create(16);
   if (!headers_str || !*headers_str)
      return d;

   char buf[4096];
   strncpy(buf, headers_str, sizeof(buf) - 1);

   char *line = strtok(buf, "\r\n");
   while (line)
   {
      char *colon = strchr(line, ':');
      if (colon)
      {
         *colon = '\0';
         char *val = colon + 1;
         while (*val == ' ')
            val++;
         Data *k = createData(TYPE_STR, line);
         Data *v = createData(TYPE_STR, val);
         dict_insert(k, v, d);
      }
      line = strtok(NULL, "\r\n");
   }
   return d;
}

//=========================================================
//  Response Builder
//=========================================================

typedef struct
{
   int status_code;
   char content_type[128];
   char body[65536];
   int sent;
} ResponseState;

static void send_http_response(int client_fd, ResponseState *rs)
{
   const char *status_text = "OK";
   if (rs->status_code == 201)
      status_text = "Created";
   else if (rs->status_code == 204)
      status_text = "No Content";
   else if (rs->status_code == 400)
      status_text = "Bad Request";
   else if (rs->status_code == 401)
      status_text = "Unauthorized";
   else if (rs->status_code == 403)
      status_text = "Forbidden";
   else if (rs->status_code == 404)
      status_text = "Not Found";
   else if (rs->status_code == 500)
      status_text = "Internal Server Error";

   char header[512];
   snprintf(header, sizeof(header),
            "HTTP/1.1 %d %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n",
            rs->status_code, status_text,
            rs->content_type,
            (int)strlen(rs->body));

   send(client_fd, header, strlen(header), 0);
   send(client_fd, rs->body, strlen(rs->body), 0);
}

//=========================================================
//  Request/Response as Dicts with function values
//=========================================================

// We build req and res as dicts.
// res functions are CX lambdas stored in the dict.
// When handler calls res.json({...}), it hits dict key "json"
// which is a lambda that captures response_state ptr via closure.

// Since lambdas can't capture C pointers directly, we use a
// different approach: res is a dict with special keys that the
// C dispatch layer intercepts.

// Actually the cleanest approach: store res as TYPE_DICT with
// a hidden "__rs__" key pointing to a TYPE_FILE-reused pointer
// to ResponseState. Then intercept res.json/text/status calls
// in a special res_call handler.

// For simplicity: pass res as a dict, handler populates it,
// we read it back after the call.

static Data *build_request(HttpRequest *req)
{
   Dict *d = dict_create(8);

   dict_insert(createData(TYPE_STR, "method"),
               createData(TYPE_STR, req->method), d);
   dict_insert(createData(TYPE_STR, "path"),
               createData(TYPE_STR, req->path), d);
   dict_insert(createData(TYPE_STR, "body"),
               createData(TYPE_STR, req->body), d);
   dict_insert(createData(TYPE_STR, "query"),
               createData(TYPE_DICT, parse_query_string(req->query)), d);
   dict_insert(createData(TYPE_STR, "headers"),
               createData(TYPE_DICT, parse_headers_dict(req->headers)), d);
   dict_insert(createData(TYPE_STR, "params"),
               createData(TYPE_DICT, dict_create(4)), d);

   return createData(TYPE_DICT, d);
}

static Data *build_response(void)
{
   // res starts as a dict:
   // {
   //   "_status": 200,
   //   "_body": "",
   //   "_type": "application/json",
   //   "_sent": false
   // }
   Dict *d = dict_create(8);

   dict_insert(createData(TYPE_STR, "_status"),
               createData(TYPE_INT, &(int){200}), d);
   dict_insert(createData(TYPE_STR, "_body"),
               createData(TYPE_STR, ""), d);
   dict_insert(createData(TYPE_STR, "_type"),
               createData(TYPE_STR, "application/json"), d);
   dict_insert(createData(TYPE_STR, "_sent"),
               createData(TYPE_BOOL, &(int){0}), d);

   return createData(TYPE_DICT, d);
}

//=========================================================
//  Route Matching
//=========================================================

// Match path with params: /users/:id matches /users/42
// Extracts params into a dict
static int match_route(const char *pattern, const char *path, Dict *params)
{
   char pat[256], p[256];
   strncpy(pat, pattern, sizeof(pat) - 1);
   strncpy(p, path, sizeof(p) - 1);

   char *pstate = NULL, *rstate = NULL; // ← strtok_r state

   char *ptok = strtok_r(pat, "/", &pstate);
   char *rtok = strtok_r(p, "/", &rstate);

   while (ptok && rtok)
   {
      if (ptok[0] == ':')
      {
         // Save segment values before any other calls
         char key[128], val[128];
         strncpy(key, ptok + 1, sizeof(key) - 1);
         strncpy(val, rtok, sizeof(val) - 1);

         // Advance BEFORE dict_insert to avoid strtok corruption
         ptok = strtok_r(NULL, "/", &pstate);
         rtok = strtok_r(NULL, "/", &rstate);

         Data *k = createData(TYPE_STR, key);
         Data *v = createData(TYPE_STR, val);
         dict_insert(k, v, params);
         continue;
      }
      else if (strcmp(ptok, rtok) != 0)
         return 0;

      ptok = strtok_r(NULL, "/", &pstate);
      rtok = strtok_r(NULL, "/", &rstate);
   }

   return ptok == NULL && rtok == NULL;
}

//=========================================================
//  Connection Handler
//=========================================================

typedef struct
{
   int client_fd;
   Server *server;
} ConnArgs;

static void handle_connection(ConnArgs *ca)
{
   int client_fd = ca->client_fd;
   Server *server = ca->server;
   free(ca);

   char raw[65536] = {0};
   int n = recv(client_fd, raw, sizeof(raw) - 1, 0);
   if (n <= 0)
   {
      close(client_fd);
      return;
   }
   raw[n] = '\0';

   HttpRequest req;
   parse_http_request(raw, &req);

   // Find matching route
   Route *matched = NULL;
   Dict *params = dict_create(4);

   for (int i = 0; i < server->route_count; i++)
   {
      Route *r = &server->routes[i];
      if (strcasecmp(r->method, req.method) == 0 &&
          match_route(r->path, req.path, params))
      {
         matched = r;
         break;
      }
   }

   if (!matched)
   {
      // 404
      const char *msg =
          "HTTP/1.1 404 Not Found\r\n"
          "Content-Type: text/plain\r\n"
          "Content-Length: 9\r\n"
          "Connection: close\r\n"
          "\r\n"
          "Not Found";
      send(client_fd, msg, strlen(msg), 0);
      close(client_fd);
      dict_free(params);
      return;
   }

   // Build req/res dicts
   Data *req_data = build_request(&req);
   // Inject params
   Data *params_key = createData(TYPE_STR, "params");
   dict_insert(params_key, createData(TYPE_DICT, params), DICT_PTR(req_data));

   Data *res_data = build_response();

   // Call handler(req, res)
   List *args = list_create(2);
   list_append(req_data, args);
   list_append(res_data, args);

   interp_lock();
   clear_error();

   Env *env = env_create(NULL);
   Runtime *hrt = malloc(sizeof(Runtime));
   memcpy(hrt, server->rt, sizeof(Runtime));
   hrt->env = env;

   executeFunction(matched->handler, args, hrt);

   env_free(env);
   free(hrt);
   interp_unlock();

   // Read response BEFORE freeing args
   Dict *res_dict = DICT_PTR(res_data);

   ResponseState rs;
   rs.sent = 0;
   rs.status_code = 200;
   strcpy(rs.content_type, "application/json");
   memset(rs.body, 0, sizeof(rs.body));

   Data *status_d = res_get(res_dict, "_status");
   if (status_d && status_d->type == TYPE_INT)
      rs.status_code = *(int *)status_d->atom;

   Data *body_d = res_get(res_dict, "_body");
   if (body_d && body_d->type == TYPE_STR)
      strncpy(rs.body, body_d->str, sizeof(rs.body) - 1);

   Data *type_d = res_get(res_dict, "_type");
   if (type_d && type_d->type == TYPE_STR)
      strncpy(rs.content_type, type_d->str, sizeof(rs.content_type) - 1);

   send_http_response(client_fd, &rs);

   // NOW free args — this frees req_data and res_data together
   list_free(args, data_free);
   close(client_fd);
}

static void *connection_thread(void *arg)
{
   handle_connection((ConnArgs *)arg);
   return NULL;
}

//=========================================================
//  Server Implementations
//=========================================================

// server() -> dict (server handle)
Data *__http_server__(Runtime *rt)
{
   Server *s = calloc(1, sizeof(Server));
   s->fd = -1;
   s->running = 0;
   s->rt = rt;

   Data *result = createData(TYPE_HTTP_SERVER, s);
   return result;
}

static Server *get_server(Data *d, const char *fn)
{
   if (!d || d->type != TYPE_HTTP_SERVER || !d->ref || !d->ref->object)
   {
      throw_error(ERROR_TYPE, "%s(): invalid server handle", fn);
      return NULL;
   }
   return (Server *)d->ref->object;
}

// _register_route(server, method, path, handler)
static Data *register_route(Data *srv, const char *method,
                            Data *path, Data *handler)
{
   Server *s = get_server(srv, "http.route");
   if (!s)
      return createData(TYPE_NONE, NULL);

   if (!path || path->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "http route: path must be a string");
      return createData(TYPE_NONE, NULL);
   }
   if (!handler || (handler->type != TYPE_FUNCTION &&
                    handler->type != TYPE_LAMBDA))
   {
      throw_error(ERROR_TYPE, "http route: handler must be a function");
      return createData(TYPE_NONE, NULL);
   }
   if (s->route_count >= MAX_ROUTES)
   {
      throw_error(ERROR_RUNTIME, "http: maximum routes (%d) exceeded", MAX_ROUTES);
      return createData(TYPE_NONE, NULL);
   }

   Route *r = &s->routes[s->route_count++];
   strncpy(r->method, method, sizeof(r->method) - 1);
   strncpy(r->path, path->str, sizeof(r->path) - 1);
   r->handler = cloneData(handler);

   return createData(TYPE_NONE, NULL);
}

// listen(server, port, host="0.0.0.0")
Data *__http_listen__(Data *srv, Data *port_d, Data *host_d)
{
   Server *s = get_server(srv, "http.listen");
   if (!s)
      return createData(TYPE_NONE, NULL);

   int port = 8080;
   if (port_d && port_d->type == TYPE_INT)
      port = *(int *)port_d->atom;

   s->port = port;

   int fd = socket(AF_INET, SOCK_STREAM, 0);
   if (fd < 0)
   {
      throw_error(ERROR_RUNTIME, "http.listen(): failed to create socket");
      return createData(TYPE_NONE, NULL);
   }

   int opt = 1;
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

   struct sockaddr_in addr = {0};
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   addr.sin_addr.s_addr = INADDR_ANY;

   if (host_d && host_d->type == TYPE_STR)
      inet_pton(AF_INET, host_d->str, &addr.sin_addr);

   if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
   {
      close(fd);
      throw_error(ERROR_RUNTIME, "http.listen(): failed to bind port %d", port);
      return createData(TYPE_NONE, NULL);
   }

   listen(fd, 128);
   s->fd = fd;
   s->running = 1;

   printf("Server listening on http://0.0.0.0:%d\n", port);
   fflush(stdout);

   while (s->running)
   {
      struct sockaddr_in client_addr;
      socklen_t client_len = sizeof(client_addr);
      int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
      if (client_fd < 0)
      {
         if (!s->running)
            break;
         continue;
      }

      ConnArgs *ca = malloc(sizeof(ConnArgs));
      ca->client_fd = client_fd;
      ca->server = s;

      pthread_t tid;
      pthread_create(&tid, NULL, connection_thread, ca);
      pthread_detach(tid);
   }

   close(fd);
   return createData(TYPE_NONE, NULL);
}

// stop(server)
Data *__http_stop__(Data *srv)
{
   Server *s = get_server(srv, "http.stop");
   if (!s)
      return createData(TYPE_NONE, NULL);
   s->running = 0;
   if (s->fd >= 0)
      close(s->fd);
   return createData(TYPE_NONE, NULL);
}

Data *__http_get__(Data *s, Data *p, Data *h) { return register_route(s, "GET", p, h); }
Data *__http_post__(Data *s, Data *p, Data *h) { return register_route(s, "POST", p, h); }
Data *__http_put__(Data *s, Data *p, Data *h) { return register_route(s, "PUT", p, h); }
Data *__http_delete__(Data *s, Data *p, Data *h) { return register_route(s, "DELETE", p, h); }
Data *__http_patch__(Data *s, Data *p, Data *h) { return register_route(s, "PATCH", p, h); }

// use(server, middleware_fn)
Data *__http_use__(Data *srv, Data *mw)
{
   Server *s = get_server(srv, "http.use");
   if (!s)
      return createData(TYPE_NONE, NULL);
   if (!mw || (mw->type != TYPE_FUNCTION && mw->type != TYPE_LAMBDA))
   {
      throw_error(ERROR_TYPE, "http.use(): middleware must be a function");
      return createData(TYPE_NONE, NULL);
   }
   if (s->middleware_count < MAX_MIDDLEWARE)
      s->middleware[s->middleware_count++] = cloneData(mw);
   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Init & Dispatch
//=========================================================

void initializeHttpModule(Runtime *mod_rt)
{
   for (int i = 0; i < HTTP_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)HTTP_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&HTTP_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

Data *dispatchHttpBuiltin(HttpType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   switch (type)
   {
   case HTTP_SERVER:
      return __http_server__(rt);
   case HTTP_LISTEN:
      return __http_listen__(ARG(0), ARG(1), ARG(2));
   case HTTP_STOP:
      return __http_stop__(ARG(0));
   case HTTP_GET:
      return __http_get__(ARG(0), ARG(1), ARG(2));
   case HTTP_POST:
      return __http_post__(ARG(0), ARG(1), ARG(2));
   case HTTP_PUT:
      return __http_put__(ARG(0), ARG(1), ARG(2));
   case HTTP_DELETE:
      return __http_delete__(ARG(0), ARG(1), ARG(2));
   case HTTP_PATCH:
      return __http_patch__(ARG(0), ARG(1), ARG(2));
   case HTTP_USE:
      return __http_use__(ARG(0), ARG(1));
   default:
      throw_error(ERROR_RUNTIME, "unknown http builtin %d", type);
      return createData(TYPE_NONE, NULL);
   }
}