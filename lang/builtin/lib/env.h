#ifndef ENV_H
#define ENV_H

#include "build.h"

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

extern const ModuleFuncInfo ENV_MODULE[];
extern const int ENV_MODULE_COUNT;

typedef enum
{
   ENV_LOAD,
   ENV_GET,
   ENV_SET,
   ENV_HAS,
   ENV_ALL,
   ENV_UNSET,
} EnvType;

void initializeEnvModule(Runtime *mod_rt);
Data *dispatchEnvBuiltin(EnvType type, List *args, Dict *kwargs, Runtime *rt);

#endif