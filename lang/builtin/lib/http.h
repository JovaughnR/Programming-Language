#ifndef HTTP_H
#define HTTP_H

#include "build.h"

extern const ModuleFuncInfo HTTP_MODULE[];
extern const int HTTP_MODULE_COUNT;

typedef enum
{
   HTTP_SERVER,
   HTTP_LISTEN,
   HTTP_STOP,
   HTTP_GET,
   HTTP_POST,
   HTTP_PUT,
   HTTP_DELETE,
   HTTP_PATCH,
   HTTP_USE, // middleware
} HttpType;

void initializeHttpModule(Runtime *mod_rt);
Data *dispatchHttpBuiltin(HttpType type, List *args, Dict *kwargs, Runtime *rt);

#endif