#ifndef NET_H
#define NET_H

#include "build.h"

extern const ModuleFuncInfo NET_MODULE[];
extern const int NET_MODULE_COUNT;

typedef enum
{
   NET_GET,
   NET_POST,
   NET_PUT,
   NET_DELETE,
   NET_PATCH,
   NET_HEAD,
   NET_REQUEST,
} NetType;

void initializeNetModule(Runtime *mod_rt);
Data *dispatchNetBuiltin(NetType type, List *args, Dict *kwargs, Runtime *rt);

#endif