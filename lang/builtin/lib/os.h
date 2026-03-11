#ifndef OS_H
#define OS_H

#include "build.h"

typedef enum
{
   OS_EXISTS,
   OS_LISTDIR,
   OS_MKDIR,
   OS_REMOVE,
   OS_RENAME,
   OS_GETCWD,
   OS_GETENV,
   OS_ISFILE,
   OS_ISDIR,
   OS_GETSIZE,
} OsType;

extern const ModuleFuncInfo OS_MODULE[];
extern const int OS_MODULE_COUNT;

void initializeOsModule(Runtime *mod_rt);
Data *dispatchOsBuiltin(OsType type, List *args, Dict *kwargs, Runtime *rt);

#endif