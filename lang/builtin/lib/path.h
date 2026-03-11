#ifndef PATH_H
#define PATH_H

#include "build.h"

extern const ModuleFuncInfo PATH_MODULE[];
extern const int PATH_MODULE_COUNT;

typedef enum
{
   PATH_JOIN,
   PATH_BASENAME,
   PATH_DIRNAME,
   PATH_EXTENSION,
   PATH_ABSOLUTE,
   PATH_NORMALIZE,
   PATH_SPLIT,
   PATH_SPLITEXT,
   PATH_ISABS,
   PATH_EXPANDUSER,
} PathType;

void initializePathModule(Runtime *mod_rt);
Data *dispatchPathBuiltin(PathType type, List *args, Dict *kwargs, Runtime *rt);

#endif