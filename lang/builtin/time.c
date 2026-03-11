#include <time.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "./lib/time.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo TIME_MODULE[] = {
    {MODULE_TIME, TIME_NOW, "__time_now__", 0, 0},
    {MODULE_TIME, TIME_HOUR, "__time_hour__", 0, 0},
    {MODULE_TIME, TIME_MINUTE, "__time_minute__", 0, 0},
    {MODULE_TIME, TIME_SECOND, "__time_second__", 0, 0},
    {MODULE_TIME, TIME_YEAR, "__time_year__", 0, 0},
    {MODULE_TIME, TIME_MONTH, "__time_month__", 0, 0},
    {MODULE_TIME, TIME_DAY, "__time_day__", 0, 0},
    {MODULE_TIME, TIME_TIMESTAMP, "__time_timestamp__", 0, 0},
    {MODULE_TIME, TIME_SLEEP, "__time_sleep__", 1, 1},
    {MODULE_TIME, TIME_DIFF, "__time_diff__", 2, 2},
    {MODULE_TIME, TIME_FORMAT, "__time_format__", 1, 1},
    {MODULE_TIME, TIME_CLOCK, "__time_clock__", 0, 0},
};

const int TIME_MODULE_COUNT = sizeof(TIME_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Helpers
//=========================================================

static struct tm *get_local_time(void)
{
   time_t now = time(NULL);
   return localtime(&now);
}

//=========================================================
//  Current Time
//=========================================================

// Returns a dict with all time fields: {hour, minute, second, year, month, day}
Data *__time_now__(void)
{
   struct tm *t = get_local_time();
   Dict *d = dict_create(6);

   struct
   {
      const char *key;
      int val;
   } fields[] = {
       {"hour", t->tm_hour},
       {"minute", t->tm_min},
       {"second", t->tm_sec},
       {"year", t->tm_year + 1900},
       {"month", t->tm_mon + 1},
       {"day", t->tm_mday},
   };

   for (int i = 0; i < 6; i++)
   {
      Data *k = createData(TYPE_STR, strdup(fields[i].key));
      Data *v = createData(TYPE_INT, &(int){fields[i].val});
      dict_insert(k, v, d);
   }

   return createData(TYPE_DICT, d);
}

Data *__time_hour__(void)
{
   return createData(TYPE_INT, &(int){get_local_time()->tm_hour});
}

Data *__time_minute__(void)
{
   return createData(TYPE_INT, &(int){get_local_time()->tm_min});
}

Data *__time_second__(void)
{
   return createData(TYPE_INT, &(int){get_local_time()->tm_sec});
}

//=========================================================
//  Current Date
//=========================================================

Data *__time_year__(void)
{
   return createData(TYPE_INT, &(int){get_local_time()->tm_year + 1900});
}

Data *__time_month__(void)
{
   return createData(TYPE_INT, &(int){get_local_time()->tm_mon + 1});
}

Data *__time_day__(void)
{
   return createData(TYPE_INT, &(int){get_local_time()->tm_mday});
}

//=========================================================
//  Timestamp
//=========================================================

// Returns unix epoch as float (seconds since Jan 1 1970)
Data *__time_timestamp__(void)
{
   double ts = (double)time(NULL);
   return createData(TYPE_FLOAT, &ts);
}

// Returns high-resolution clock for timing (seconds as float)
Data *__time_clock__(void)
{
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC, &ts);
   double t = ts.tv_sec + ts.tv_nsec / 1e9;
   return createData(TYPE_FLOAT, &t);
}

//=========================================================
//  Sleep
//=========================================================

// sleep(seconds) — accepts int or float
Data *__time_sleep__(Data *seconds)
{
   if (!seconds || (seconds->type != TYPE_INT && seconds->type != TYPE_FLOAT))
   {
      throw_error(ERROR_TYPE, "time.sleep() argument must be a number");
      return createData(TYPE_NONE, NULL);
   }

   double secs = seconds->type == TYPE_FLOAT
                     ? *(double *)seconds->real
                     : (double)*(int *)seconds->atom;

   if (secs < 0)
   {
      throw_error(ERROR_VALUE, "time.sleep() argument must be >= 0");
      return createData(TYPE_NONE, NULL);
   }

   struct timespec ts;
   ts.tv_sec = (time_t)secs;
   ts.tv_nsec = (long)((secs - ts.tv_sec) * 1e9);
   nanosleep(&ts, NULL);

   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Time Difference
//=========================================================

// diff(t1, t2) — both are unix timestamps (floats), returns seconds as float
Data *__time_diff__(Data *t1, Data *t2)
{
   if (!t1 || t1->type == TYPE_NONE || !t2 || t2->type == TYPE_NONE)
   {
      throw_error(ERROR_TYPE, "time.diff() requires two timestamp arguments");
      return createData(TYPE_NONE, NULL);
   }
   double v1 = t1->type == TYPE_FLOAT ? *(double *)t1->real : (double)*(int *)t1->atom;
   double v2 = t2->type == TYPE_FLOAT ? *(double *)t2->real : (double)*(int *)t2->atom;
   double diff = v2 - v1;

   return createData(TYPE_FLOAT, &diff);
}

//=========================================================
//  Format
//=========================================================

// format(fmt) — formats current time using strftime-style string
// e.g. time.format("%Y-%m-%d %H:%M:%S")
Data *__time_format__(Data *fmt)
{
   if (!fmt || fmt->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "time.format() argument must be a string");
      return createData(TYPE_NONE, NULL);
   }

   struct tm *t = get_local_time();
   char buffer[256];
   strftime(buffer, sizeof(buffer), fmt->str, t);

   return createData(TYPE_STR, strdup(buffer));
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeTimeModule(Runtime *mod_rt)
{
   for (int i = 0; i < TIME_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)TIME_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&TIME_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchTimeBuiltin(TimeType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case TIME_NOW:
      return __time_now__();
   case TIME_HOUR:
      return __time_hour__();
   case TIME_MINUTE:
      return __time_minute__();
   case TIME_SECOND:
      return __time_second__();
   case TIME_YEAR:
      return __time_year__();
   case TIME_MONTH:
      return __time_month__();
   case TIME_DAY:
      return __time_day__();
   case TIME_TIMESTAMP:
      return __time_timestamp__();
   case TIME_CLOCK:
      return __time_clock__();
   case TIME_SLEEP:
      return __time_sleep__(ARG(0));
   case TIME_DIFF:
      return __time_diff__(ARG(0), ARG(1));
   case TIME_FORMAT:
      return __time_format__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown time builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}