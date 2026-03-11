#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "./lib/stats.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo STATS_MODULE[] = {
    {MODULE_STATS, STATS_MEAN, "__stats_mean__", 1, 1},
    {MODULE_STATS, STATS_MEDIAN, "__stats_median__", 1, 1},
    {MODULE_STATS, STATS_MODE, "__stats_mode__", 1, 1},
    {MODULE_STATS, STATS_VARIANCE, "__stats_variance__", 1, 1},
    {MODULE_STATS, STATS_STDEV, "__stats_stdev__", 1, 1},
    {MODULE_STATS, STATS_CORRELATION, "__stats_correlation__", 2, 2},
    {MODULE_STATS, STATS_MIN, "__stats_min__", 1, 1},
    {MODULE_STATS, STATS_MAX, "__stats_max__", 1, 1},
    {MODULE_STATS, STATS_RANGE, "__stats_range__", 1, 1},
    {MODULE_STATS, STATS_SUM, "__stats_sum__", 1, 1},
    {MODULE_STATS, STATS_COUNT, "__stats_count__", 1, 1},
    {MODULE_STATS, STATS_PERCENTILE, "__stats_percentile__", 2, 2},
    {MODULE_STATS, STATS_ZSCORE, "__stats_zscore__", 1, 1},
};

const int STATS_MODULE_COUNT = sizeof(STATS_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Helpers
//=========================================================

// Extract a numeric list into a C double array
static double *extract_doubles(Data *list_data, int *out_len, const char *funcname)
{
   if (!list_data || list_data->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a list", funcname);
      return NULL;
   }

   List *list = LIST_PTR(list_data);
   if (list->length == 0)
   {
      throw_error(ERROR_VALUE, "%s() list must not be empty", funcname);
      return NULL;
   }

   double *arr = malloc(list->length * sizeof(double));
   for (int i = 0; i < list->length; i++)
   {
      Data *item = (Data *)list->items[i];
      if (item->type == TYPE_INT)
         arr[i] = (double)*(int *)item->atom;
      else if (item->type == TYPE_FLOAT)
         arr[i] = *(double *)item->real;
      else
      {
         free(arr);
         throw_error(ERROR_TYPE, "%s() list must contain only numbers", funcname);
         return NULL;
      }
   }

   *out_len = list->length;
   return arr;
}

static int cmp_double(const void *a, const void *b)
{
   double da = *(double *)a;
   double db = *(double *)b;
   return (da > db) - (da < db);
}

//=========================================================
//  Implementations
//=========================================================

// mean(list) -> float
// Returns the arithmetic mean of a list of numbers.
// Example: stats.mean([1, 2, 3, 4, 5]) -> 3.0
Data *__stats_mean__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.mean");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double sum = 0.0;
   for (int i = 0; i < n; i++)
      sum += arr[i];
   free(arr);

   double result = sum / n;
   return createData(TYPE_FLOAT, &result);
}

// median(list) -> float
// Returns the middle value of a sorted list.
// For even-length lists returns the average of the two middle values.
// Example: stats.median([1, 2, 3, 4, 5]) -> 3.0
// Example: stats.median([1, 2, 3, 4])    -> 2.5
Data *__stats_median__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.median");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   qsort(arr, n, sizeof(double), cmp_double);

   double result;
   if (n % 2 == 1)
      result = arr[n / 2];
   else
      result = (arr[n / 2 - 1] + arr[n / 2]) / 2.0;

   free(arr);
   return createData(TYPE_FLOAT, &result);
}

// mode(list) -> float
// Returns the most frequently occurring value.
// If multiple values tie, returns the smallest.
// Example: stats.mode([1, 2, 2, 3, 3, 3]) -> 3.0
Data *__stats_mode__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.mode");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   qsort(arr, n, sizeof(double), cmp_double);

   double mode = arr[0];
   int max_count = 1;
   int count = 1;

   for (int i = 1; i < n; i++)
   {
      if (arr[i] == arr[i - 1])
      {
         count++;
         if (count > max_count)
         {
            max_count = count;
            mode = arr[i];
         }
      }
      else
         count = 1;
   }

   free(arr);
   return createData(TYPE_FLOAT, &mode);
}

// variance(list) -> float
// Returns the population variance of a list of numbers.
// Example: stats.variance([2, 4, 4, 4, 5, 5, 7, 9]) -> 4.0
Data *__stats_variance__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.variance");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double sum = 0.0;
   for (int i = 0; i < n; i++)
      sum += arr[i];
   double mean = sum / n;

   double variance = 0.0;
   for (int i = 0; i < n; i++)
      variance += (arr[i] - mean) * (arr[i] - mean);
   variance /= n;

   free(arr);
   return createData(TYPE_FLOAT, &variance);
}

// stdev(list) -> float
// Returns the population standard deviation.
// Example: stats.stdev([2, 4, 4, 4, 5, 5, 7, 9]) -> 2.0
Data *__stats_stdev__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.stdev");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double sum = 0.0;
   for (int i = 0; i < n; i++)
      sum += arr[i];
   double mean = sum / n;

   double variance = 0.0;
   for (int i = 0; i < n; i++)
      variance += (arr[i] - mean) * (arr[i] - mean);
   variance /= n;

   free(arr);
   double result = sqrt(variance);
   return createData(TYPE_FLOAT, &result);
}

// correlation(x, y) -> float
// Returns the Pearson correlation coefficient between two lists.
// Result is between -1.0 (perfect negative) and 1.0 (perfect positive).
// Example: stats.correlation([1,2,3,4,5], [2,4,6,8,10]) -> 1.0
Data *__stats_correlation__(Data *x_data, Data *y_data)
{
   int nx, ny;
   double *x = extract_doubles(x_data, &nx, "stats.correlation");
   double *y = extract_doubles(y_data, &ny, "stats.correlation");

   if (!x || !y)
   {
      free(x);
      free(y);
      return createData(TYPE_NONE, NULL);
   }

   if (nx != ny)
   {
      free(x);
      free(y);
      throw_error(ERROR_VALUE, "stats.correlation() both lists must have the same length");
      return createData(TYPE_NONE, NULL);
   }

   int n = nx;
   double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0, sum_y2 = 0;

   for (int i = 0; i < n; i++)
   {
      sum_x += x[i];
      sum_y += y[i];
      sum_xy += x[i] * y[i];
      sum_x2 += x[i] * x[i];
      sum_y2 += y[i] * y[i];
   }

   double num = n * sum_xy - sum_x * sum_y;
   double denom = sqrt((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));

   free(x);
   free(y);

   if (denom == 0.0)
      return createData(TYPE_FLOAT, &(double){0.0});
   double result = num / denom;
   return createData(TYPE_FLOAT, &result);
}

// min(list) -> float
// Returns the smallest value in the list.
// Example: stats.min([3, 1, 4, 1, 5]) -> 1.0
Data *__stats_min__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.min");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double min = arr[0];
   for (int i = 1; i < n; i++)
      if (arr[i] < min)
         min = arr[i];

   free(arr);
   return createData(TYPE_FLOAT, &min);
}

// max(list) -> float
// Returns the largest value in the list.
// Example: stats.max([3, 1, 4, 1, 5]) -> 5.0
Data *__stats_max__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.max");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double max = arr[0];
   for (int i = 1; i < n; i++)
      if (arr[i] > max)
         max = arr[i];

   free(arr);
   return createData(TYPE_FLOAT, &max);
}

// range(list) -> float
// Returns the difference between max and min values.
// Example: stats.range([1, 2, 3, 4, 5]) -> 4.0
Data *__stats_range__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.range");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double min = arr[0], max = arr[0];
   for (int i = 1; i < n; i++)
   {
      if (arr[i] < min)
         min = arr[i];
      if (arr[i] > max)
         max = arr[i];
   }

   free(arr);
   double result = max - min;
   return createData(TYPE_FLOAT, &result);
}

// sum(list) -> float
// Returns the sum of all values in the list.
// Example: stats.sum([1, 2, 3, 4, 5]) -> 15.0
Data *__stats_sum__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.sum");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double sum = 0.0;
   for (int i = 0; i < n; i++)
      sum += arr[i];
   free(arr);
   return createData(TYPE_FLOAT, &sum);
}

// count(list) -> int
// Returns the number of elements in the list.
// Example: stats.count([1, 2, 3, 4, 5]) -> 5
Data *__stats_count__(Data *list_data)
{
   if (!list_data || list_data->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "stats.count() argument must be a list");
      return createData(TYPE_NONE, NULL);
   }
   int n = LIST_PTR(list_data)->length;
   return createData(TYPE_INT, &n);
}

// percentile(list, p) -> float
// Returns the p-th percentile of the list (0-100).
// Example: stats.percentile([1,2,3,4,5,6,7,8,9,10], 90) -> 9.1
Data *__stats_percentile__(Data *list_data, Data *p_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.percentile");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   if (!p_data || (p_data->type != TYPE_INT && p_data->type != TYPE_FLOAT))
   {
      free(arr);
      throw_error(ERROR_TYPE, "stats.percentile() second argument must be a number");
      return createData(TYPE_NONE, NULL);
   }

   double p = p_data->type == TYPE_FLOAT
                  ? *(double *)p_data->real
                  : (double)*(int *)p_data->atom;

   if (p < 0.0 || p > 100.0)
   {
      free(arr);
      throw_error(ERROR_VALUE, "stats.percentile() p must be between 0 and 100");
      return createData(TYPE_NONE, NULL);
   }

   qsort(arr, n, sizeof(double), cmp_double);

   double idx = (p / 100.0) * (n - 1);
   int lo = (int)idx;
   int hi = lo + 1;
   double frac = idx - lo;

   double result = (hi >= n)
                       ? arr[lo]
                       : arr[lo] + frac * (arr[hi] - arr[lo]);

   free(arr);
   return createData(TYPE_FLOAT, &result);
}

// zscore(list) -> list
// Returns the z-scores of all values (how many stdevs from mean).
// Example: stats.zscore([2, 4, 4, 4, 5, 5, 7, 9]) -> [-1.5, -0.5, ...]
Data *__stats_zscore__(Data *list_data)
{
   int n;
   double *arr = extract_doubles(list_data, &n, "stats.zscore");
   if (!arr)
      return createData(TYPE_NONE, NULL);

   double sum = 0.0;
   for (int i = 0; i < n; i++)
      sum += arr[i];
   double mean = sum / n;

   double variance = 0.0;
   for (int i = 0; i < n; i++)
      variance += (arr[i] - mean) * (arr[i] - mean);
   double stdev = sqrt(variance / n);

   List *result = list_create(__size__);
   for (int i = 0; i < n; i++)
   {
      double z = stdev == 0.0 ? 0.0 : (arr[i] - mean) / stdev;
      list_append(createData(TYPE_FLOAT, &z), result);
   }

   free(arr);
   return createData(TYPE_LIST, result);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeStatsModule(Runtime *mod_rt)
{
   for (int i = 0; i < STATS_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)STATS_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&STATS_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchStatsBuiltin(StatsType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case STATS_MEAN:
      return __stats_mean__(ARG(0));
   case STATS_MEDIAN:
      return __stats_median__(ARG(0));
   case STATS_MODE:
      return __stats_mode__(ARG(0));
   case STATS_VARIANCE:
      return __stats_variance__(ARG(0));
   case STATS_STDEV:
      return __stats_stdev__(ARG(0));
   case STATS_CORRELATION:
      return __stats_correlation__(ARG(0), ARG(1));
   case STATS_MIN:
      return __stats_min__(ARG(0));
   case STATS_MAX:
      return __stats_max__(ARG(0));
   case STATS_RANGE:
      return __stats_range__(ARG(0));
   case STATS_SUM:
      return __stats_sum__(ARG(0));
   case STATS_COUNT:
      return __stats_count__(ARG(0));
   case STATS_PERCENTILE:
      return __stats_percentile__(ARG(0), ARG(1));
   case STATS_ZSCORE:
      return __stats_zscore__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown stats builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}