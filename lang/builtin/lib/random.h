#ifndef RANDOM_H
#define RANDOM_H

#include "build.h"

extern const ModuleFuncInfo RANDOM_MODULE[];
extern const int RANDOM_MODULE_COUNT;

typedef enum
{
   RANDOM_RANDOM,
   RANDOM_RANDINT,
   RANDOM_CHOICE,
   RANDOM_SHUFFLE,
   RANDOM_SEED,
   RANDOM_UNIFORM,
   RANDOM_SAMPLE,
   RANDOM_RANDRANGE,
} RandomType;

void initializeRandomModule(Runtime *mod_rt);
Data *dispatchRandomBuiltin(RandomType type, List *args, Dict *kwargs, Runtime *rt);

#endif