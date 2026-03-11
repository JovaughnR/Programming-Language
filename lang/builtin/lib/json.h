#ifndef JSON_H
#define JSON_H

#include "build.h"

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

extern const ModuleFuncInfo JSON_MODULE[];
extern const int JSON_MODULE_COUNT;

typedef enum
{
   JSON_PARSE,
   JSON_STRINGIFY,
} JsonType;

void initializeJsonModule(Runtime *mod_rt);
Data *dispatchJsonBuiltin(JsonType type, List *args, Dict *kwargs, Runtime *rt);

#endif