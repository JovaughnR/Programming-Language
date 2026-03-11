#ifndef LOG_H
#define LOG_H

#include "build.h"

extern const ModuleFuncInfo LOG_MODULE[];
extern const int LOG_MODULE_COUNT;

typedef enum
{
   LOG_DEBUG,
   LOG_INFO,
   LOG_WARN,
   LOG_ERROR,
   LOG_FATAL,
   LOG_SETLEVEL,
   LOG_SETFILE,
   LOG_SETFORMAT,
   LOG_CLOSE,
} LogType;

typedef enum
{
   LOG_LEVEL_DEBUG = 0,
   LOG_LEVEL_INFO = 1,
   LOG_LEVEL_WARN = 2,
   LOG_LEVEL_ERROR = 3,
   LOG_LEVEL_FATAL = 4,
   LOG_LEVEL_OFF = 5,
} LogLevel;

void initializeLogModule(Runtime *mod_rt);
Data *dispatchLogBuiltin(LogType type, List *args, Dict *kwargs, Runtime *rt);

#endif