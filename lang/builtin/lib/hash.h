#ifndef HASH_H
#define HASH_H

#include "build.h"

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

extern const ModuleFuncInfo HASH_MODULE[];
extern const int HASH_MODULE_COUNT;

typedef enum
{
   HASH_MD5,
   HASH_SHA256,
   HASH_SHA1,
   HASH_SHA512,
} HashType;

void initializeHashModule(Runtime *mod_rt);
Data *dispatchHashBuiltin(HashType type, List *args, Dict *kwargs, Runtime *rt);

#endif