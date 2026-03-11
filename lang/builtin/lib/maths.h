// Math module builtins
#ifndef MATH_H
#define MATH_H
#include "build.h"

extern const ModuleFuncInfo JSON_MODULE[];
extern const int JSON_MODULE_COUNT;

typedef enum
{
    MATH_SQRT,
    MATH_CBRT,
    MATH_POW,
    MATH_HYPOT,
    MATH_EXP,
    MATH_LOG,
    MATH_LOG2,
    MATH_LOG10,
    MATH_SIN,
    MATH_COS,
    MATH_TAN,
    MATH_ASIN,
    MATH_ACOS,
    MATH_ATAN,
    MATH_ATAN2,
    MATH_FLOOR,
    MATH_CEIL,
    MATH_ROUND,
    MATH_DEGREES,
    MATH_RADIANS,
    MATH_PI,
    MATH_E,
    MATH_TAU,
    MATH_INF,
} MathType;

void initializeMathModule(Runtime *mod_rt);
Data *dispatchMathBuiltin(MathType type, List *args, Dict *kwargs, Runtime *rt);

#endif // MATH_H