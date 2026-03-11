#ifndef ARGS_H
#define ARGS_H

#include "build.h"

extern const ModuleFuncInfo ARGS_MODULE[];
extern const int ARGS_MODULE_COUNT;

typedef enum
{
   ARGS_PARSE,
   ARGS_GET,
   ARGS_HAS,
   ARGS_ALL,
   ARGS_USAGE,
} ArgsType;

void initializeArgsModule(Runtime *mod_rt);
Data *dispatchArgsBuiltin(ArgsType type, List *args, Dict *kwargs, Runtime *rt);

#endif