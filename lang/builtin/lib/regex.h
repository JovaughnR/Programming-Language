#ifndef RE_H
#define RE_H

#include "build.h"

extern const ModuleFuncInfo RE_MODULE[];
extern const int RE_MODULE_COUNT;

typedef enum
{
   RE_MATCH,
   RE_SEARCH,
   RE_FINDALL,
   RE_REPLACE,
   RE_SPLIT,
   RE_TEST,
   RE_GROUPS,
} ReType;

void initializeReModule(Runtime *mod_rt);
Data *dispatchReBuiltin(ReType type, List *args, Dict *kwargs, Runtime *rt);

#endif