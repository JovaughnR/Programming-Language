#ifndef DECIMAL_H
#define DECIMAL_H

#include "build.h"

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

extern const ModuleFuncInfo DECIMAL_MODULE[];
extern const int DECIMAL_MODULE_COUNT;

typedef enum
{
   DECIMAL_ADD,
   DECIMAL_SUB,
   DECIMAL_MUL,
   DECIMAL_DIV,
   DECIMAL_MOD,
   DECIMAL_POW,
   DECIMAL_ABS,
   DECIMAL_NEG,
   DECIMAL_FLOOR,
   DECIMAL_CEIL,
   DECIMAL_ROUND,
   DECIMAL_CMP,
   DECIMAL_EQ,
   DECIMAL_LT,
   DECIMAL_GT,
   DECIMAL_TOSTR,
   DECIMAL_TOFLOAT,
   DECIMAL_TOINT,
   DECIMAL_FROMSTR,
   DECIMAL_SQRT,      // ← add
   DECIMAL_FACTORIAL, // ← add
   DECIMAL_GCD,       // ← add
   DECIMAL_LCM,       // ← add
} DecimalType;

void initializeDecimalModule(Runtime *mod_rt);
Data *dispatchDecimalBuiltin(DecimalType type, List *args, Dict *kwargs, Runtime *rt);

#endif