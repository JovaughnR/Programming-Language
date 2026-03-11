#ifndef SYS_H
#define SYS_H

#include "build.h"

extern const ModuleFuncInfo SYS_MODULE[];
extern const int SYS_MODULE_COUNT;

typedef enum
{
   SYS_ARGS,
   SYS_EXIT,
   SYS_PLATFORM,
   SYS_VERSION,
   SYS_GETPID,
   SYS_GETENV,
   SYS_STDIN,
   SYS_STDOUT,
   SYS_STDERR,
} SysType;

// Called from main() to store argc/argv before interpreter starts
void sys_init(int argc, char **argv);

void initializeSysModule(Runtime *mod_rt);
Data *dispatchSysBuiltin(SysType type, List *args, Dict *kwargs, Runtime *rt);

#endif