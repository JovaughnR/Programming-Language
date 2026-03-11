#ifndef SOCKET_H
#define SOCKET_H

#include "build.h"

extern const ModuleFuncInfo SOCKET_MODULE[];
extern const int SOCKET_MODULE_COUNT;

typedef enum
{
   SOCKET_CREATE,
   SOCKET_BIND,
   SOCKET_LISTEN,
   SOCKET_ACCEPT,
   SOCKET_CONNECT,
   SOCKET_SEND,
   SOCKET_RECV,
   SOCKET_CLOSE,
   SOCKET_SETOPT,
   SOCKET_GETADDR,
} SocketType;

typedef enum
{
   SOCK_TYPE_TCP,
   SOCK_TYPE_UDP,
} SockKind;

void initializeSocketModule(Runtime *mod_rt);
Data *dispatchSocketBuiltin(SocketType type, List *args, Dict *kwargs, Runtime *rt);

#endif