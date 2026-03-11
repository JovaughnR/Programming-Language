#include <stdlib.h>
#include <math.h>

#include "./lib/maths.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo MATH_MODULE[] = {
    {MODULE_MATH, MATH_SQRT, "__sqrt__", 1, 1},
    {MODULE_MATH, MATH_CBRT, "__cbrt__", 1, 1},
    {MODULE_MATH, MATH_POW, "__pow__", 2, 2},
    {MODULE_MATH, MATH_HYPOT, "__hypot__", 2, 2},
    {MODULE_MATH, MATH_EXP, "__exp__", 1, 1},
    {MODULE_MATH, MATH_LOG, "__log__", 1, 2},
    {MODULE_MATH, MATH_LOG2, "__log2__", 1, 1},
    {MODULE_MATH, MATH_LOG10, "__log10__", 1, 1},
    {MODULE_MATH, MATH_SIN, "__sin__", 1, 1},
    {MODULE_MATH, MATH_COS, "__cos__", 1, 1},
    {MODULE_MATH, MATH_TAN, "__tan__", 1, 1},
    {MODULE_MATH, MATH_ASIN, "__asin__", 1, 1},
    {MODULE_MATH, MATH_ACOS, "__acos__", 1, 1},
    {MODULE_MATH, MATH_ATAN, "__atan__", 1, 1},
    {MODULE_MATH, MATH_ATAN2, "__atan2__", 2, 2},
    {MODULE_MATH, MATH_FLOOR, "__floor__", 1, 1},
    {MODULE_MATH, MATH_CEIL, "__ceil__", 1, 1},
    {MODULE_MATH, MATH_ROUND, "__round__", 1, 2},
    {MODULE_MATH, MATH_DEGREES, "__degrees__", 1, 1},
    {MODULE_MATH, MATH_RADIANS, "__radians__", 1, 1},
};

#define MATH_MODULE_COUNT (sizeof(MATH_MODULE) / sizeof(BuiltinInfo))

static double to_double(Data *d)
{
   if (!d || d->type == TYPE_NONE)
      return 0.0;
   if (d->type == TYPE_FLOAT)
      return *(double *)d->real;
   if (d->type == TYPE_INT)
      return (double)*(int *)d->atom;
   return 0.0;
}

// =============================================
//    Constants
// =============================================

Data *__pi__()
{
   return createData(TYPE_FLOAT, &(double){M_PI});
}

Data *__e__()
{
   return createData(TYPE_FLOAT, &(double){M_E});
}

Data *__tau__()
{
   return createData(TYPE_FLOAT, &(double){M_PI * 2.0});
}

Data *__inf__()
{
   return createData(TYPE_FLOAT, &(double){INFINITY});
}

//=========================================================
//  Powers & Roots
//=========================================================

Data *__sqrt__(Data *x)
{
   double val = to_double(x);
   if (val < 0)
   {
      throw_error(ERROR_VALUE, "math.sqrt() domain error");
      return createData(TYPE_NONE, NULL);
   }
   return createData(TYPE_FLOAT, &(double){sqrt(val)});
}

Data *__cbrt__(Data *x)
{
   double val = to_double(x);
   return createData(TYPE_FLOAT, &(double){cbrt(val)});
}

Data *__pow__(Data *base, Data *exp)
{
   double b = to_double(base);
   double e = to_double(exp);
   return createData(TYPE_FLOAT, &(double){pow(b, e)});
}

Data *__hypot__(Data *x, Data *y)
{
   double v1 = to_double(x);
   double v2 = to_double(y);
   return createData(TYPE_FLOAT, &(double){hypot(v1, v2)});
}

//=========================================================
//  Exponential & Logarithm
//=========================================================

Data *__exp__(Data *x)
{
   double val = to_double(x);
   return createData(TYPE_FLOAT, &(double){exp(val)});
}

Data *__log__(Data *x, Data *base)
{
   double val = to_double(x);
   if (val <= 0)
   {
      throw_error(ERROR_VALUE, "math.log() domain error");
      return createData(TYPE_NONE, NULL);
   }
   if (base && (base->type == TYPE_INT || base->type == TYPE_FLOAT))
   {
      double b = to_double(base);
      if (b <= 0 || b == 1)
      {
         throw_error(ERROR_VALUE, "math.log() invalid base");
         return createData(TYPE_NONE, NULL);
      }
      return createData(TYPE_FLOAT, &(double){log(val) / log(b)});
   }
   return createData(TYPE_FLOAT, &(double){log(val)});
}

Data *__log2__(Data *x)
{
   double val = to_double(x);
   if (val <= 0)
   {
      throw_error(ERROR_VALUE, "math.log2() domain error");
      return createData(TYPE_NONE, NULL);
   }
   return createData(TYPE_FLOAT, &(double){log2(val)});
}

Data *__log10__(Data *x)
{
   double val = to_double(x);
   if (val <= 0)
   {
      throw_error(ERROR_VALUE, "math.log10() domain error");
      return createData(TYPE_NONE, NULL);
   }
   return createData(TYPE_FLOAT, &(double){log10(val)});
}

//=========================================================
//  Trigonometry
//=========================================================

Data *__sin__(Data *x)
{
   return createData(TYPE_FLOAT, &(double){sin(to_double(x))});
}

Data *__cos__(Data *x)
{
   return createData(TYPE_FLOAT, &(double){cos(to_double(x))});
}

Data *__tan__(Data *x)
{
   return createData(TYPE_FLOAT, &(double){tan(to_double(x))});
}

Data *__asin__(Data *x)
{
   double val = to_double(x);
   if (val < -1 || val > 1)
   {
      throw_error(ERROR_VALUE, "math.asin() domain error");
      return createData(TYPE_NONE, NULL);
   }
   return createData(TYPE_FLOAT, &(double){asin(val)});
}

Data *__acos__(Data *x)
{
   double val = to_double(x);
   if (val < -1 || val > 1)
   {
      throw_error(ERROR_VALUE, "math.acos() domain error");
      return createData(TYPE_NONE, NULL);
   }
   return createData(TYPE_FLOAT, &(double){acos(val)});
}

Data *__atan__(Data *x)
{
   return createData(TYPE_FLOAT, &(double){atan(to_double(x))});
}

Data *__atan2__(Data *y, Data *x)
{
   return createData(TYPE_FLOAT, &(double){atan2(to_double(y), to_double(x))});
}

//=========================================================
//  Rounding
//=========================================================

Data *__floor__(Data *x)
{
   return createData(TYPE_INT, &(int){(int)floor(to_double(x))});
}

Data *__ceil__(Data *x)
{
   return createData(TYPE_INT, &(int){(int)ceil(to_double(x))});
}

Data *__round__(Data *x, Data *digits)
{
   double val = to_double(x);
   if (digits && digits->type == TYPE_INT)
   {
      double factor = pow(10.0, (double)*(int *)digits->atom);
      return createData(TYPE_FLOAT, &(double){round(val * factor) / factor});
   }
   return createData(TYPE_INT, &(int){(int)round(val)});
}
//=========================================================
//  Angle Conversion
//=========================================================

Data *__degrees__(Data *x)
{
   return createData(TYPE_FLOAT, &(double){to_double(x) * (180.0 / M_PI)});
}

Data *__radians__(Data *x)
{
   return createData(TYPE_FLOAT, &(double){to_double(x) * (M_PI / 180.0)});
}

/** Note methods marked as TYPE_MODULE are static methods defined
    for class abstraction or set of functions and shouldn't
    be accessed like functions such as print(), len() etc ..

    eg usage:
    import math # set of functions
    from mod import * classes or functions
    math.sqrt(x);
*/
void initializeMathModule(Runtime *mod_rt)
{
   for (int i = 0; i < MATH_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)MATH_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&MATH_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }

   // Constants stored directly as floats
   struct
   {
      const char *name;
      double val;
   } consts[] = {
       {"__pi__", M_PI},
       {"__e__", M_E},
       {"__tau__", M_PI * 2.0},
       {"__inf__", INFINITY},
   };

   for (int i = 0; i < 4; i++)
   {
      Data *k = createData(TYPE_STR, (void *)(consts[i].name));
      env_save(k, createData(TYPE_FLOAT, &consts[i].val), mod_rt->env);
      data_free(k);
   }
}
//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchMathBuiltin(MathType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   // Powers & roots
   case MATH_SQRT:
      return __sqrt__(ARG(0));
   case MATH_CBRT:
      return __cbrt__(ARG(0));
   case MATH_POW:
      return __pow__(ARG(0), ARG(1));
   case MATH_HYPOT:
      return __hypot__(ARG(0), ARG(1));

   // Exponential & log
   case MATH_EXP:
      return __exp__(ARG(0));
   case MATH_LOG:
      return __log__(ARG(0), ARG(1));
   case MATH_LOG2:
      return __log2__(ARG(0));
   case MATH_LOG10:
      return __log10__(ARG(0));

   // Trig
   case MATH_SIN:
      return __sin__(ARG(0));
   case MATH_COS:
      return __cos__(ARG(0));
   case MATH_TAN:
      return __tan__(ARG(0));
   case MATH_ASIN:
      return __asin__(ARG(0));
   case MATH_ACOS:
      return __acos__(ARG(0));
   case MATH_ATAN:
      return __atan__(ARG(0));
   case MATH_ATAN2:
      return __atan2__(ARG(0), ARG(1));

   // Rounding
   case MATH_FLOOR:
      return __floor__(ARG(0));
   case MATH_CEIL:
      return __ceil__(ARG(0));
   case MATH_ROUND:
      return __round__(ARG(0), ARG(1));

   // Angle conversion
   case MATH_DEGREES:
      return __degrees__(ARG(0));
   case MATH_RADIANS:
      return __radians__(ARG(0));

   default:
      throw_error(ERROR_SYNTAX, "unknown math function type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}