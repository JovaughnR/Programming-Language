#ifndef THREAD_H
#define THREAD_H

#include "build.h"

extern const ModuleFuncInfo THREAD_MODULE[];
extern const int THREAD_MODULE_COUNT;

typedef enum
{
   THREAD_CREATE,
   THREAD_JOIN,
   THREAD_DETACH,
   THREAD_SLEEP,
   THREAD_ID,
   MUTEX_CREATE,
   MUTEX_LOCK,
   MUTEX_UNLOCK,
   MUTEX_DESTROY,
   COND_CREATE,
   COND_WAIT,
   COND_SIGNAL,
   COND_BROADCAST,
   COND_DESTROY,
} ThreadType;

void interp_lock(void);
void interp_unlock(void);
void initializeThreadModule(Runtime *mod_rt);
Data *dispatchThreadBuiltin(ThreadType type, List *args, Dict *kwargs, Runtime *rt);
extern Data *executeFunction(Data *func_data, List *args, Runtime *rt);

#endif