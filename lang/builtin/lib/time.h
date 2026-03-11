#ifndef TIME_H
#define TIME_H

#include "build.h"

extern const ModuleFuncInfo TIME_MODULE[];
extern const int TIME_MODULE_COUNT;

typedef enum
{
   TIME_NOW,
   TIME_HOUR,
   TIME_MINUTE,
   TIME_SECOND,
   TIME_YEAR,
   TIME_MONTH,
   TIME_DAY,
   TIME_TIMESTAMP,
   TIME_SLEEP,
   TIME_DIFF,
   TIME_FORMAT,
   TIME_CLOCK,
} TimeType;

void initializeTimeModule(Runtime *mod_rt);
Data *dispatchTimeBuiltin(TimeType type, List *args, Dict *kwargs, Runtime *rt);

#endif