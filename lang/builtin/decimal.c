#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gmp.h>

#include "./lib/decimal.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo DECIMAL_MODULE[] = {
    {MODULE_DECIMAL, DECIMAL_ADD, "__decimal_add__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_SUB, "__decimal_sub__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_MUL, "__decimal_mul__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_DIV, "__decimal_div__", 2, 3},
    {MODULE_DECIMAL, DECIMAL_MOD, "__decimal_mod__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_POW, "__decimal_pow__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_ABS, "__decimal_abs__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_NEG, "__decimal_neg__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_FLOOR, "__decimal_floor__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_CEIL, "__decimal_ceil__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_ROUND, "__decimal_round__", 1, 2},
    {MODULE_DECIMAL, DECIMAL_CMP, "__decimal_cmp__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_EQ, "__decimal_eq__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_LT, "__decimal_lt__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_GT, "__decimal_gt__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_TOSTR, "__decimal_tostr__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_TOFLOAT, "__decimal_tofloat__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_TOINT, "__decimal_toint__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_FROMSTR, "__decimal_fromstr__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_SQRT, "__decimal_sqrt__", 1, 2},
    {MODULE_DECIMAL, DECIMAL_FACTORIAL, "__decimal_factorial__", 1, 1},
    {MODULE_DECIMAL, DECIMAL_GCD, "__decimal_gcd__", 2, 2},
    {MODULE_DECIMAL, DECIMAL_LCM, "__decimal_lcm__", 2, 2},
};

const int DECIMAL_MODULE_COUNT = sizeof(DECIMAL_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Precision (global, adjustable)
//=========================================================

static int g_precision = 28; // default decimal digits of precision

//=========================================================
//  Helpers
//=========================================================

// Convert Data to mpf_t — caller must call mpf_clear
static int data_to_mpf(Data *d, mpf_t out, const char *funcname)
{
   if (!d || d->type == TYPE_NONE)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a number or decimal string", funcname);
      return 0;
   }

   mp_bitcnt_t bits = g_precision * 4; // ~4 bits per decimal digit
   mpf_init2(out, bits);

   if (d->type == TYPE_STR)
   {
      if (mpf_set_str(out, d->str, 10) != 0)
      {
         mpf_clear(out);
         throw_error(ERROR_VALUE, "%s() invalid decimal string: '%s'", funcname, d->str);
         return 0;
      }
   }
   else if (d->type == TYPE_INT)
      mpf_set_si(out, *(int *)d->atom);
   else if (d->type == TYPE_FLOAT)
      mpf_set_d(out, *(double *)d->real);
   else
   {
      mpf_clear(out);
      throw_error(ERROR_TYPE, "%s() unsupported type", funcname);
      return 0;
   }
   return 1;
}

// Convert mpf_t to a clean decimal string
static char *mpf_to_str(mpf_t val, int precision)
{
   // gmp_asprintf gives us full control
   char *buf = NULL;
   gmp_asprintf(&buf, "%.*Ff", precision, val);

   // Strip trailing zeros after decimal point
   char *dot = strchr(buf, '.');
   if (dot)
   {
      char *end = buf + strlen(buf) - 1;
      while (end > dot && *end == '0')
         *end-- = '\0';
      if (*end == '.')
         *end = '\0';
   }

   return buf; // caller must free with free()
}

// Convert Data to mpz_t (integer) — caller must call mpz_clear
static int data_to_mpz(Data *d, mpz_t out, const char *funcname)
{
   if (!d || d->type == TYPE_NONE)
   {
      throw_error(ERROR_TYPE, "%s() argument must be an integer or decimal string", funcname);
      return 0;
   }

   mpz_init(out);

   if (d->type == TYPE_STR)
   {
      // Strip decimal part if present
      char *copy = strdup(d->str);
      char *dot = strchr(copy, '.');
      if (dot)
         *dot = '\0';
      int ret = mpz_set_str(out, copy, 10);
      free(copy);
      if (ret != 0)
      {
         mpz_clear(out);
         throw_error(ERROR_VALUE, "%s() invalid integer string: '%s'", funcname, d->str);
         return 0;
      }
   }
   else if (d->type == TYPE_INT)
      mpz_set_si(out, *(int *)d->atom);
   else
   {
      mpz_clear(out);
      throw_error(ERROR_TYPE, "%s() argument must be an integer", funcname);
      return 0;
   }

   return 1;
}

//=========================================================
//  Implementations
//=========================================================

// add(a, b) -> str
// Adds two decimal values with arbitrary precision.
// Example: decimal.add("0.1", "0.2") -> "0.3"
Data *__decimal_add__(Data *a_data, Data *b_data)
{
   mpf_t a, b, result;
   if (!data_to_mpf(a_data, a, "decimal.add"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.add"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   mpf_init2(result, g_precision * 4);
   mpf_add(result, a, b);

   char *s = mpf_to_str(result, g_precision);
   Data *res = createData(TYPE_STR, s);

   mpf_clear(a);
   mpf_clear(b);
   mpf_clear(result);
   return res;
}

// sub(a, b) -> str
// Subtracts b from a with arbitrary precision.
// Example: decimal.sub("1.0", "0.3") -> "0.7"
Data *__decimal_sub__(Data *a_data, Data *b_data)
{
   mpf_t a, b, result;
   if (!data_to_mpf(a_data, a, "decimal.sub"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.sub"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   mpf_init2(result, g_precision * 4);
   mpf_sub(result, a, b);

   char *s = mpf_to_str(result, g_precision);
   Data *res = createData(TYPE_STR, s);

   mpf_clear(a);
   mpf_clear(b);
   mpf_clear(result);
   return res;
}

// mul(a, b) -> str
// Multiplies two decimal values with arbitrary precision.
// Example: decimal.mul("0.1", "0.3") -> "0.03"
Data *__decimal_mul__(Data *a_data, Data *b_data)
{
   mpf_t a, b, result;
   if (!data_to_mpf(a_data, a, "decimal.mul"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.mul"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   mpf_init2(result, g_precision * 4);
   mpf_mul(result, a, b);

   char *s = mpf_to_str(result, g_precision);
   Data *res = createData(TYPE_STR, s);

   mpf_clear(a);
   mpf_clear(b);
   mpf_clear(result);
   return res;
}

// div(a, b, precision=28) -> str
// Divides a by b to arbitrary precision.
// Example: decimal.div("1", "3")    -> "0.3333333333333333333333333333"
// Example: decimal.div("1", "3", 5) -> "0.33333"
Data *__decimal_div__(Data *a_data, Data *b_data, Data *prec_data)
{
   int precision = g_precision;
   if (prec_data && prec_data->type == TYPE_INT)
      precision = *(int *)prec_data->atom;

   mpf_t a, b, result;
   if (!data_to_mpf(a_data, a, "decimal.div"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.div"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   if (mpf_sgn(b) == 0)
   {
      mpf_clear(a);
      mpf_clear(b);
      throw_error(ERROR_RUNTIME, "decimal.div(): division by zero");
      return createData(TYPE_NONE, NULL);
   }

   mpf_init2(result, precision * 4);
   mpf_div(result, a, b);

   char *s = mpf_to_str(result, precision);
   Data *res = createData(TYPE_STR, s);

   mpf_clear(a);
   mpf_clear(b);
   mpf_clear(result);
   return res;
}

// mod(a, b) -> str
// Returns the remainder of integer division.
// Example: decimal.mod("10", "3") -> "1"
Data *__decimal_mod__(Data *a_data, Data *b_data)
{
   mpz_t a, b, result;
   if (!data_to_mpz(a_data, a, "decimal.mod"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpz(b_data, b, "decimal.mod"))
   {
      mpz_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   if (mpz_sgn(b) == 0)
   {
      mpz_clear(a);
      mpz_clear(b);
      throw_error(ERROR_RUNTIME, "decimal.mod(): division by zero");
      return createData(TYPE_NONE, NULL);
   }

   mpz_init(result);
   mpz_mod(result, a, b);

   char *s = mpz_get_str(NULL, 10, result);
   Data *res = createData(TYPE_STR, s);

   mpz_clear(a);
   mpz_clear(b);
   mpz_clear(result);
   return res;
}

// pow(a, exp) -> str
// Raises a to an integer power with arbitrary precision.
// Example: decimal.pow("2", "100") -> exact 100-digit result
Data *__decimal_pow__(Data *a_data, Data *exp_data)
{
   // Use mpz for integer bases and exponents for exact results
   if ((a_data->type == TYPE_INT || a_data->type == TYPE_STR) &&
       (exp_data->type == TYPE_INT || exp_data->type == TYPE_STR))
   {
      mpz_t base, result;
      if (!data_to_mpz(a_data, base, "decimal.pow"))
         return createData(TYPE_NONE, NULL);

      unsigned long exp = 0;
      if (exp_data->type == TYPE_INT)
         exp = (unsigned long)*(int *)exp_data->atom;
      else
         exp = strtoul(exp_data->str, NULL, 10);

      mpz_init(result);
      mpz_pow_ui(result, base, exp);

      char *s = mpz_get_str(NULL, 10, result);
      Data *res = createData(TYPE_STR, s);
      mpz_clear(base);
      mpz_clear(result);
      return res;
   }

   // Float fallback
   mpf_t a, result;
   if (!data_to_mpf(a_data, a, "decimal.pow"))
      return createData(TYPE_NONE, NULL);

   double exp_d = exp_data->type == TYPE_FLOAT
                      ? *(double *)exp_data->real
                      : (double)*(int *)exp_data->atom;

   mpf_init2(result, g_precision * 4);

   // mpf doesn't have pow — use exp(exp * log(base))
   double base_d = mpf_get_d(a);
   double res_d = pow(base_d, exp_d);
   mpf_set_d(result, res_d);

   char *s = mpf_to_str(result, g_precision);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   mpf_clear(result);
   return res;
}

// abs(a) -> str
// Returns the absolute value.
// Example: decimal.abs("-3.14") -> "3.14"
Data *__decimal_abs__(Data *a_data)
{
   mpf_t a, result;
   if (!data_to_mpf(a_data, a, "decimal.abs"))
      return createData(TYPE_NONE, NULL);

   mpf_init2(result, g_precision * 4);
   mpf_abs(result, a);

   char *s = mpf_to_str(result, g_precision);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   mpf_clear(result);
   return res;
}

// neg(a) -> str
// Negates the value.
// Example: decimal.neg("3.14") -> "-3.14"
Data *__decimal_neg__(Data *a_data)
{
   mpf_t a, result;
   if (!data_to_mpf(a_data, a, "decimal.neg"))
      return createData(TYPE_NONE, NULL);

   mpf_init2(result, g_precision * 4);
   mpf_neg(result, a);

   char *s = mpf_to_str(result, g_precision);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   mpf_clear(result);
   return res;
}

// floor(a) -> str
// Returns the largest integer <= a.
// Example: decimal.floor("3.7")  -> "3"
// Example: decimal.floor("-3.2") -> "-4"
Data *__decimal_floor__(Data *a_data)
{
   mpf_t a, result;
   if (!data_to_mpf(a_data, a, "decimal.floor"))
      return createData(TYPE_NONE, NULL);

   mpf_init2(result, g_precision * 4);
   mpf_floor(result, a);

   char *s = mpf_to_str(result, 0);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   mpf_clear(result);
   return res;
}

// ceil(a) -> str
// Returns the smallest integer >= a.
// Example: decimal.ceil("3.2")  -> "4"
// Example: decimal.ceil("-3.7") -> "-3"
Data *__decimal_ceil__(Data *a_data)
{
   mpf_t a, result;
   if (!data_to_mpf(a_data, a, "decimal.ceil"))
      return createData(TYPE_NONE, NULL);

   mpf_init2(result, g_precision * 4);
   mpf_ceil(result, a);

   char *s = mpf_to_str(result, 0);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   mpf_clear(result);
   return res;
}

// round(a, places=0) -> str
// Rounds to the given number of decimal places.
// Example: decimal.round("3.14159", 2) -> "3.14"
Data *__decimal_round__(Data *a_data, Data *places_data)
{
   int places = 0;
   if (places_data && places_data->type == TYPE_INT)
      places = *(int *)places_data->atom;

   mpf_t a;
   if (!data_to_mpf(a_data, a, "decimal.round"))
      return createData(TYPE_NONE, NULL);

   // Shift, round, shift back
   mpf_t shift, shifted, result;
   mpf_init2(shift, g_precision * 4);
   mpf_init2(shifted, g_precision * 4);
   mpf_init2(result, g_precision * 4);

   mpf_set_ui(shift, 10);
   mpf_pow_ui(shift, shift, (unsigned long)places);

   mpf_mul(shifted, a, shift);

   // Add 0.5 and floor for rounding
   mpf_t half;
   mpf_init2(half, g_precision * 4);
   mpf_set_d(half, mpf_sgn(a) >= 0 ? 0.5 : -0.5);
   mpf_add(shifted, shifted, half);
   mpf_floor(result, shifted);
   mpf_div(result, result, shift);

   char *s = mpf_to_str(result, places);
   Data *res = createData(TYPE_STR, s);

   mpf_clear(a);
   mpf_clear(shift);
   mpf_clear(shifted);
   mpf_clear(result);
   mpf_clear(half);
   return res;
}

// sqrt(a, precision=28) -> str
// Returns the square root to arbitrary precision.
// Example: decimal.sqrt("2") -> "1.4142135623730950488..."
Data *__decimal_sqrt__(Data *a_data, Data *prec_data)
{
   int precision = g_precision;
   if (prec_data && prec_data->type == TYPE_INT)
      precision = *(int *)prec_data->atom;

   mpf_t a, result;
   if (!data_to_mpf(a_data, a, "decimal.sqrt"))
      return createData(TYPE_NONE, NULL);

   if (mpf_sgn(a) < 0)
   {
      mpf_clear(a);
      throw_error(ERROR_VALUE, "decimal.sqrt(): cannot take square root of negative number");
      return createData(TYPE_NONE, NULL);
   }

   mpf_init2(result, precision * 4);
   mpf_sqrt(result, a);

   char *s = mpf_to_str(result, precision);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   mpf_clear(result);
   return res;
}

// factorial(n) -> str
// Returns the exact factorial of n.
// Example: decimal.factorial("100") -> exact 158-digit result
Data *__decimal_factorial__(Data *n_data)
{
   mpz_t n, result;
   if (!data_to_mpz(n_data, n, "decimal.factorial"))
      return createData(TYPE_NONE, NULL);

   if (mpz_sgn(n) < 0)
   {
      mpz_clear(n);
      throw_error(ERROR_VALUE, "decimal.factorial(): argument must be non-negative");
      return createData(TYPE_NONE, NULL);
   }

   unsigned long n_ul = mpz_get_ui(n);
   mpz_init(result);
   mpz_fac_ui(result, n_ul);

   char *s = mpz_get_str(NULL, 10, result);
   Data *res = createData(TYPE_STR, s);
   mpz_clear(n);
   mpz_clear(result);
   return res;
}

// gcd(a, b) -> str
// Returns the greatest common divisor.
// Example: decimal.gcd("48", "18") -> "6"
Data *__decimal_gcd__(Data *a_data, Data *b_data)
{
   mpz_t a, b, result;
   if (!data_to_mpz(a_data, a, "decimal.gcd"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpz(b_data, b, "decimal.gcd"))
   {
      mpz_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   mpz_init(result);
   mpz_gcd(result, a, b);

   char *s = mpz_get_str(NULL, 10, result);
   Data *res = createData(TYPE_STR, s);
   mpz_clear(a);
   mpz_clear(b);
   mpz_clear(result);
   return res;
}

// lcm(a, b) -> str
// Returns the least common multiple.
// Example: decimal.lcm("4", "6") -> "12"
Data *__decimal_lcm__(Data *a_data, Data *b_data)
{
   mpz_t a, b, result;
   if (!data_to_mpz(a_data, a, "decimal.lcm"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpz(b_data, b, "decimal.lcm"))
   {
      mpz_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   mpz_init(result);
   mpz_lcm(result, a, b);

   char *s = mpz_get_str(NULL, 10, result);
   Data *res = createData(TYPE_STR, s);
   mpz_clear(a);
   mpz_clear(b);
   mpz_clear(result);
   return res;
}

// cmp(a, b) -> int
Data *__decimal_cmp__(Data *a_data, Data *b_data)
{
   mpf_t a, b;
   if (!data_to_mpf(a_data, a, "decimal.cmp"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.cmp"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   int result = mpf_cmp(a, b);
   result = (result > 0) - (result < 0); // normalize to -1, 0, 1
   mpf_clear(a);
   mpf_clear(b);
   return createData(TYPE_INT, &result);
}

// eq(a, b) -> bool
Data *__decimal_eq__(Data *a_data, Data *b_data)
{
   mpf_t a, b;
   if (!data_to_mpf(a_data, a, "decimal.eq"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.eq"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   int result = mpf_cmp(a, b) == 0;
   mpf_clear(a);
   mpf_clear(b);
   return createData(TYPE_BOOL, &result);
}

// lt(a, b) -> bool
Data *__decimal_lt__(Data *a_data, Data *b_data)
{
   mpf_t a, b;
   if (!data_to_mpf(a_data, a, "decimal.lt"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.lt"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   int result = mpf_cmp(a, b) < 0;
   mpf_clear(a);
   mpf_clear(b);
   return createData(TYPE_BOOL, &result);
}

// gt(a, b) -> bool
Data *__decimal_gt__(Data *a_data, Data *b_data)
{
   mpf_t a, b;
   if (!data_to_mpf(a_data, a, "decimal.gt"))
      return createData(TYPE_NONE, NULL);
   if (!data_to_mpf(b_data, b, "decimal.gt"))
   {
      mpf_clear(a);
      return createData(TYPE_NONE, NULL);
   }

   int result = mpf_cmp(a, b) > 0;
   mpf_clear(a);
   mpf_clear(b);
   return createData(TYPE_BOOL, &result);
}

// tostr(a) -> str
Data *__decimal_tostr__(Data *a_data)
{
   mpf_t a;
   if (!data_to_mpf(a_data, a, "decimal.tostr"))
      return createData(TYPE_NONE, NULL);
   char *s = mpf_to_str(a, g_precision);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   return res;
}

// tofloat(a) -> float
Data *__decimal_tofloat__(Data *a_data)
{
   mpf_t a;
   if (!data_to_mpf(a_data, a, "decimal.tofloat"))
      return createData(TYPE_NONE, NULL);
   double result = mpf_get_d(a);
   mpf_clear(a);
   return createData(TYPE_FLOAT, &result);
}

// toint(a) -> int
Data *__decimal_toint__(Data *a_data)
{
   mpf_t a;
   if (!data_to_mpf(a_data, a, "decimal.toint"))
      return createData(TYPE_NONE, NULL);
   int result = (int)mpf_get_si(a);
   mpf_clear(a);
   return createData(TYPE_INT, &result);
}

// fromstr(s) -> str
Data *__decimal_fromstr__(Data *s_data)
{
   mpf_t a;
   if (!data_to_mpf(s_data, a, "decimal.fromstr"))
      return createData(TYPE_NONE, NULL);
   char *s = mpf_to_str(a, g_precision);
   Data *res = createData(TYPE_STR, s);
   mpf_clear(a);
   return res;
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeDecimalModule(Runtime *mod_rt)
{
   for (int i = 0; i < DECIMAL_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)DECIMAL_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&DECIMAL_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchDecimalBuiltin(DecimalType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case DECIMAL_ADD:
      return __decimal_add__(ARG(0), ARG(1));
   case DECIMAL_SUB:
      return __decimal_sub__(ARG(0), ARG(1));
   case DECIMAL_MUL:
      return __decimal_mul__(ARG(0), ARG(1));
   case DECIMAL_DIV:
      return __decimal_div__(ARG(0), ARG(1), ARG(2));
   case DECIMAL_MOD:
      return __decimal_mod__(ARG(0), ARG(1));
   case DECIMAL_POW:
      return __decimal_pow__(ARG(0), ARG(1));
   case DECIMAL_ABS:
      return __decimal_abs__(ARG(0));
   case DECIMAL_NEG:
      return __decimal_neg__(ARG(0));
   case DECIMAL_FLOOR:
      return __decimal_floor__(ARG(0));
   case DECIMAL_CEIL:
      return __decimal_ceil__(ARG(0));
   case DECIMAL_ROUND:
      return __decimal_round__(ARG(0), ARG(1));
   case DECIMAL_CMP:
      return __decimal_cmp__(ARG(0), ARG(1));
   case DECIMAL_EQ:
      return __decimal_eq__(ARG(0), ARG(1));
   case DECIMAL_LT:
      return __decimal_lt__(ARG(0), ARG(1));
   case DECIMAL_GT:
      return __decimal_gt__(ARG(0), ARG(1));
   case DECIMAL_TOSTR:
      return __decimal_tostr__(ARG(0));
   case DECIMAL_TOFLOAT:
      return __decimal_tofloat__(ARG(0));
   case DECIMAL_TOINT:
      return __decimal_toint__(ARG(0));
   case DECIMAL_FROMSTR:
      return __decimal_fromstr__(ARG(0));
   case DECIMAL_SQRT:
      return __decimal_sqrt__(ARG(0), ARG(1));
   case DECIMAL_FACTORIAL:
      return __decimal_factorial__(ARG(0));
   case DECIMAL_GCD:
      return __decimal_gcd__(ARG(0), ARG(1));
   case DECIMAL_LCM:
      return __decimal_lcm__(ARG(0), ARG(1));
   default:
      throw_error(ERROR_SYNTAX, "unknown decimal builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}