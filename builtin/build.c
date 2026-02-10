//=========================================================
//  Built-in Function System (with TYPE_ATTRIBUTE support)
//=========================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../lib/type.h"
#include "../lib/dict.h"
#include "../lib/utils.h"
#include "../lib/list.h"
#include "../lib/set.h"
#include "../lib/maloc.h"
#include "../lib/daloc.h"
#include "../lib/error.h"
#include "../lib/build.h"
#include "../lib/exec.h"
#include "../lib/str.h"
#include "../lib/class.h"
#include "../lib/format.h"
#include "methods.h"

//=========================================================
//  Argument Extraction Helpers
//=========================================================

static inline Data *arg_data(List *args, int i)
{
   if (!args || i >= args->length)
      return NULL;
   return (Data *)args->items[i];
}

static inline int arg_int(List *args, int i, int def)
{
   Data *d = arg_data(args, i);
   return (d && d->type == TYPE_INT && d->integer.atom) ? *(int *)d->integer.atom : def;
}

static inline const char *arg_str(List *args, int i, const char *def)
{
   Data *d = arg_data(args, i);
   return (d && d->type == TYPE_STR && d->str.string) ? d->str.string : def;
}

//=========================================================
//  HELPER: Initialize Single Registry into Dict
//=========================================================

void initializeRegistry(const BuiltinInfo *registry, Dict *dict)
{
   if (!dict || !registry)
      return;

   size_t len = sizeof(registry) / sizeof(BuiltinInfo);
   size_t i = 0;

   while (i < len)
   {
      char *name = strdup(registry[i].name);
      void *method = (void *)&registry[i];

      Data *key = createData(TYPE_STR, name);
      Data *val = createData(TYPE_BUILTIN, method);

      if (key && val)
      {
         dict_insert(key, val, dict);
      }
      else
      {
         if (key)
            freeData(key);
         if (val)
            freeData(val);
      }
      ++i;
   }
   /****************************************************
     Initailization of object methods base on object type
   ************************************************** */
}

//=========================================================
//  MAIN INITIALIZATION FUNCTION
//=========================================================

void initializeBuiltins(Runtime *rt)
{
   if (!rt || !rt->methods)
      return;

   Method *methods = rt->methods;

   // Initialize core builtins (print, len, int, etc.)
   initializeRegistry(methods->builtin, BUILTIN_REGISTRY);

   // Initialize list methodszz
   initializeRegistry(methods->list, LIST_REGISTRY);

   // Initialize dict methods
   initializeRegistry(methods->dict, DICT_REGISTRY);

   // Initialize set methods
   initializeRegistry(methods->set, SET_REGISTRY);

   // Initialize string methods
   initializeRegistry(methods->string, STR_REGISTRY);

   // Initialize int/bool methods
   initializeRegistry(methods->atom, INT_REGISTRY);

   // Initialize float methods
   initializeRegistry(methods->real, FLOAT_REGISTRY);

   // Initialize range methods
   initializeRegistry(methods->range, RANGE_REGISTRY);
}

//=========================================================
//  RUNTIME CREATION WITH INITIALIZATION
//=========================================================

Runtime *createRuntime(void)
{
   Runtime *rt = (Runtime *)malloc(sizeof(Runtime));
   if (!rt)
      return NULL;

   // Create global environment
   rt->env = env_create(NULL);
   if (!rt->env)
   {
      free(rt);
      return NULL;
   }

   // Allocate methods structure
   rt->methods = (Builtin *)malloc(sizeof(Builtin));
   if (!rt->methods)
   {
      env_free(rt->env);
      free(rt);
      return NULL;
   }

   // Create all method dictionaries with appropriate sizes
   rt->methods->builtins = createDict(BUILTIN_COUNT);
   rt->methods->lists = createDict(LIST_COUNT);
   rt->methods->dicts = createDict(DICT_COUNT);
   rt->methods->sets = createDict(SET_COUNT);
   rt->methods->strings = createDict(STR_COUNT);
   rt->methods->atoms = createDict(INT_COUNT);
   rt->methods->reals = createDict(FLOAT_COUNT);
   rt->methods->ranges = createDict(RANGE_COUNT);
   rt->methods->none = createDict(1); // Placeholder

   // Check if all dictionaries were created successfully
   if (!rt->methods->builtins || !rt->methods->lists ||
       !rt->methods->dicts || !rt->methods->sets ||
       !rt->methods->strings || !rt->methods->atoms ||
       !rt->methods->reals || !rt->methods->ranges)
   {
      fprintf(stderr, "Error: Failed to create method dictionaries\n");
      freeRuntime(rt);
      return NULL;
   }

   // Initialize modules
   rt->modules = NULL;

   // **THIS IS WHERE ALL METHODS GET INITIALIZED**
   initializeAllMethods(rt);

   return rt;
}

//=========================================================
//  CLEANUP
//=========================================================

void freeRuntime(Runtime *rt)
{
   if (!rt)
      return;

   if (rt->env)
      env_free(rt->env);

   if (rt->methods)
   {
      if (rt->methods->builtins)
         freeDict(rt->methods->builtins);
      if (rt->methods->lists)
         freeDict(rt->methods->lists);
      if (rt->methods->dicts)
         freeDict(rt->methods->dicts);
      if (rt->methods->sets)
         freeDict(rt->methods->sets);
      if (rt->methods->strings)
         freeDict(rt->methods->strings);
      if (rt->methods->atoms)
         freeDict(rt->methods->atoms);
      if (rt->methods->reals)
         freeDict(rt->methods->reals);
      if (rt->methods->ranges)
         freeDict(rt->methods->ranges);
      if (rt->methods->none)
         freeDict(rt->methods->none);

      free(rt->methods);
   }

   if (rt->modules)
      freeDict(rt->modules);

   free(rt);
}

//=========================================================
//  METHOD LOOKUP FUNCTION
//=========================================================

Data *getBuiltinMethod(DataType type, Data *methodName, Runtime *rt)
{
   if (!methodName || methodName->type != TYPE_STR || !rt || !rt->methods)
      return NULL;

   Dict *methodDict = NULL;

   // Select the appropriate method dictionary based on type
   switch (type)
   {
   case TYPE_LIST:
      methodDict = rt->methods->lists;
      break;
   case TYPE_DICT:
      methodDict = rt->methods->dicts;
      break;
   case TYPE_SET:
      methodDict = rt->methods->sets;
      break;
   case TYPE_STR:
      methodDict = rt->methods->strings;
      break;
   case TYPE_INT:
   case TYPE_BOOL:
      methodDict = rt->methods->atoms;
      break;
   case TYPE_FLOAT:
      methodDict = rt->methods->reals;
      break;
   case TYPE_RANGE:
      methodDict = rt->methods->ranges;
      break;
   default:
      return NULL;
   }

   if (!methodDict)
      return NULL;

   // Look up the method in the dictionary
   Data *method = dict_get(methodName, methodDict, dataCompare);

   if (method)
      return cloneData(method);

   return NULL;
}

//=========================================================
//  CORE BUILTIN DISPATCHER
//=========================================================

Data *dispatchBuiltin(BuiltinType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(rt);

   if (!args)
      args = createList(PARSE_SIZE);

   switch (type)
   {
   case BUILTIN_PRINT:
      return builtin_print(args, kwargs);
   case BUILTIN_INPUT:
      return builtin_input(ARG_STR(0, NULL));
   case BUILTIN_TYPE:
      return builtin_type(ARG(0));
   case BUILTIN_INT:
      return builtin_int(ARG(0));
   case BUILTIN_FLOAT:
      return builtin_float(ARG(0));
   case BUILTIN_STR:
      return builtin_str(ARG(0));
   case BUILTIN_BOOL:
      return builtin_bool(ARG(0));
   case BUILTIN_LIST:
      return builtin_list(ARG(0));
   case BUILTIN_SET:
      return builtin_set(ARG(0));
   case BUILTIN_RANGE:
      return builtin_range(ARG(0), ARG(1), ARG(2));
   case BUILTIN_LEN:
      return builtin_len(ARG(0));
   case BUILTIN_SORTED:
      return builtin_sorted(ARG(0));
   case BUILTIN_REVERSED:
      return builtin_reversed(ARG(0));
   case BUILTIN_ABS:
      return builtin_abs(ARG(0));
   case BUILTIN_POW:
      return builtin_pow(ARG(0), ARG(1));
   case BUILTIN_ROUND:
      return builtin_round(ARG(0), ARG_INT(1, 0));

   case BUILTIN_MIN:
   case BUILTIN_MAX:
   case BUILTIN_SUM:
   case BUILTIN_DICT:
      throw_error(ERROR_SYNTAX, "builtin '%s' not yet implemented",
                  BUILTIN_REGISTRY[type].name);
      return createData(TYPE_NONE, NULL);

   default:
      throw_error(ERROR_SYNTAX, "unknown builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}

//=========================================================
//  LIST METHOD DISPATCHER
//=========================================================

Data *dispatchListMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   if (!object || object->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "list method called on non-list object");
      return createData(TYPE_NONE, NULL);
   }

   List *list = LIST_PTR(object);

   switch (type)
   {
   case LIST_APPEND:
      if (list_append(ARG(0), list) == LIST_ERROR)
      {
         throw_error(ERROR_RUNTIME, "failed to append to list");
      }
      return createData(TYPE_NONE, NULL);

   case LIST_EXTEND:
   {
      Data *other = ARG(0);
      if (!other || other->type != TYPE_LIST)
      {
         throw_error(ERROR_TYPE, "extend() argument must be a list");
         return createData(TYPE_NONE, NULL);
      }
      list_extend(list, LIST_PTR(other));
      return createData(TYPE_NONE, NULL);
   }

   case LIST_INSERT:
   {
      int index = ARG_INT(0, 0);
      void *value = ARG(1);
      if (list_insert(list, index, value) == LIST_ERROR)
      {
         throw_error(ERROR_RUNTIME, "failed to insert into list");
      }
      return createData(TYPE_NONE, NULL);
   }

   case LIST_REMOVE:
   {
      void *value = ARG(0);
      if (list_remove(value, list, freeData) == LIST_ERROR)
      {
         throw_error(
             ERROR_VALUE, "%s not in list",
             dataTostring((Data *)value));
      }
      return createData(TYPE_NONE, NULL);
   }

   case LIST_POP:
   {
      int index = ARG_INT(0, -1);
      if (index == -1 || index == list->length - 1)
      {
         void *value = list_pop(list);
         return value ? (Data *)value : createData(TYPE_NONE, NULL);
      }
      else
      {
         void *value = list_get(index, list);
         if (!value)
         {
            throw_error(ERROR_INDEX, "pop index out of range");
            return createData(TYPE_NONE, NULL);
         }
         list_remove(value, list, freeData);
         return (Data *)value;
      }
   }

   case LIST_CLEAR:
      list_clear(list);
      return createData(TYPE_NONE, NULL);

   case LIST_INDEX:
   {
      void *value = ARG(0);
      int idx = list_index(value, list, dataCompare);
      if (idx == -1)
      {
         throw_error(ERROR_VALUE, "%s not in list",
                     dataTostring((Data *)value));
         return createData(TYPE_NONE, NULL);
      }
      return createData(TYPE_INT, (void *)(long)idx);
   }

   case LIST_REVERSE:
      list_reverse(list);
      return createData(TYPE_NONE, NULL);

   case LIST_COPY:
   {
      List *copy = list_clone(list);
      return createData(TYPE_LIST, copy);
   }

   default:
      throw_error(ERROR_SYNTAX, "unknown list method type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}

//=========================================================
//  DICT METHOD DISPATCHER
//=========================================================

Data *dispatchDictMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt)
{
   Dict *dict = DICT_PTR(object);

   switch (type)
   {
   case DICT_GET:
   {
      void *key = ARG(0);
      void *default_val = ARG(1);
      void *result = dict_get(key, dict, dataCompare);
      if (result)
         return (Data *)result;
      return default_val ? (Data *)default_val : createData(TYPE_NONE, NULL);
   }

   case DICT_KEYS:
   {
      void *keys = dict_keys(dict);
      return keys ? (Data *)keys : createData(TYPE_LIST, createList(0));
   }

   case DICT_VALUES:
   {
      void *values = dict_values(dict);
      return values ? (Data *)values : createData(TYPE_LIST, createList(0));
   }

   case DICT_ITEMS:
   {
      void *items = dict_items(dict);
      return items ? (Data *)items : createData(TYPE_LIST, createList(0));
   }

   case DICT_POP:
   {
      void *key = ARG(0);
      void *result = dict_pop(key, dict, dataCompare);
      if (result)
         return (Data *)result;

      throw_error(ERROR_KEY, "%s", dataTostring((Data *)key));
      return createData(TYPE_NONE, NULL);
   }

   case DICT_CLEAR:
      dict_clear(dict);
      return createData(TYPE_NONE, NULL);

   case DICT_UPDATE:
   {
      Data *other = ARG(0);
      if (!other || other->type != TYPE_DICT)
      {
         throw_error(ERROR_TYPE, "update() argument must be a dict");
         return createData(TYPE_NONE, NULL);
      }
      dict_update(dict, DICT_PTR(other));
      return createData(TYPE_NONE, NULL);
   }

   case DICT_SETDEFAULT:
   {
      void *val = ARG(1) ? ARG(1) : createData(TYPE_NONE, NULL);
      return createData(TYPE_INT, &(int){dict_setdefault(ARG(0), val, dict)});
   }

   case DICT_COPY:
   {
      Dict *copy = dict_clone(dict, cloneData);
      return createData(TYPE_DICT, copy);
   }

   default:
      throw_error(ERROR_SYNTAX, "unknown dict method type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}

//=========================================================
//  SET METHOD DISPATCHER
//=========================================================

Data *dispatchSetMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   Set *set = SET_PTR(object);
   Compare cmp = dataCompare;
   Clone cln = cloneData;

   switch (type)
   {
   case SET_ADD:
      set_add(ARG(0), set, dataCompare);
      return createData(TYPE_NONE, NULL);

   case SET_REMOVE:
   {
      set_remove(ARG(0), set, dataCompare);
      return createData(TYPE_NONE, NULL);
   }

   case SET_DISCARD:
      set_remove(ARG(0), set, dataCompare);
      return createData(TYPE_NONE, NULL);

   case SET_CLEAR:
      set_clear(set);
      return createData(TYPE_NONE, NULL);

   case SET_UNION:
   {
      Data *other = ARG(0);
      if (!other || other->type != TYPE_SET)
      {
         throw_error(ERROR_TYPE, "union() argument must be a set");
         return createData(TYPE_NONE, NULL);
      }
      Set *result = set_union(set, SET_PTR(other), cmp, cln);
      return createData(TYPE_SET, result);
   }

   case SET_INTERSECTION:
   {
      Data *other = ARG(0);
      if (!other || other->type != TYPE_SET)
      {
         throw_error(ERROR_TYPE, "intersection() argument must be a set");
         return createData(TYPE_NONE, NULL);
      }
      Set *result = set_intersection(set, SET_PTR(other), cmp, cln);
      return createData(TYPE_SET, result);
   }

   case SET_DIFFERENCE:
   {
      Data *other = ARG(0);
      if (!other || other->type != TYPE_SET)
      {
         throw_error(ERROR_TYPE, "difference() argument must be a set");
         return createData(TYPE_NONE, NULL);
      }
      Set *result = set_difference(set, SET_PTR(other), cmp, cln);
      return createData(TYPE_SET, result);
   }

   case SET_SYMMETRIC_DIFFERENCE:
   {
      Data *other = ARG(0);
      if (!other || other->type != TYPE_SET)
      {
         throw_error(ERROR_TYPE, "symmetric_difference() argument must be a set");
         return createData(TYPE_NONE, NULL);
      }
      Set *result = set_symdiff(set, SET_PTR(other), cmp, cln);
      return createData(TYPE_SET, result);
   }

   case SET_COPY:
   {
      Set *copy = set_clone(set, dataCompare, cloneData);
      return createData(TYPE_SET, copy);
   }

   case SET_HAS:
   {
      void *value = ARG(0);
      int has = set_has(value, set, dataCompare);
      return createData(TYPE_BOOL, (void *)(long)has);
   }

   default:
      throw_error(ERROR_SYNTAX, "unknown set method type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}

//=========================================================
//  STRING METHOD DISPATCHER
//=========================================================

Data *dispatchStrMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   if (!object || object->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "string method called on non-string object");
      return createData(TYPE_NONE, NULL);
   }

   char *str = object->str.string;

   switch (type)
   {
   case STR_UPPER:
   {
      char *result = str_to_uppercase(str);
      return createData(TYPE_STR, result);
   }

   case STR_LOWER:
   {
      char *result = str_to_lowercase(str);
      return createData(TYPE_STR, result);
   }

   case STR_CAPITALIZE:
   {
      char *result = str_capitalize(str);
      return createData(TYPE_STR, result);
   }

   case STR_TITLE:
   {
      char *result = str_title_case(str);
      return createData(TYPE_STR, result);
   }

   case STR_STRIP:
   {
      char *result = str_trim(str);
      return createData(TYPE_STR, result);
   }

   case STR_LSTRIP:
   {
      char *result = str_trim_start(str);
      return createData(TYPE_STR, result);
   }

   case STR_RSTRIP:
   {
      char *result = str_trim_end(str);
      return createData(TYPE_STR, result);
   }

   case STR_REPLACE:
   {
      char *old_str = ARG_STR(0, "");
      char *new_str = ARG_STR(1, "");
      char *result = str_replace_all(str, old_str, new_str);
      return createData(TYPE_STR, result);
   }

   case STR_SPLIT:
   {
      // TODO: Implement str_split function
      // For now, return empty list
      return createData(TYPE_LIST, createList(0));
   }

   case STR_JOIN:
   {
      Data *iterable = ARG(0);
      if (!iterable || iterable->type != TYPE_LIST)
      {
         throw_error(ERROR_TYPE, "join() argument must be a list");
         return createData(TYPE_NONE, NULL);
      }

      List *list = LIST_PTR(iterable);
      if (list->length == 0)
         return createData(TYPE_STR, strdup(""));

      // Calculate total length
      size_t total_len = 0;
      size_t sep_len = strlen(str);
      for (int i = 0; i < list->length; i++)
      {
         Data *item = (Data *)list->items[i];
         if (item->type == TYPE_STR)
            total_len += strlen(STR_PTR(item));
      }
      total_len += sep_len * (list->length - 1);

      // Build result
      char *result = malloc(total_len + 1);
      result[0] = '\0';

      for (int i = 0; i < list->length; i++)
      {
         Data *item = (Data *)list->items[i];
         if (item->type == TYPE_STR)
         {
            strcat(result, STR_PTR(item));
            if (i < list->length - 1)
               strcat(result, str);
         }
      }

      return createData(TYPE_STR, result);
   }

   case STR_STARTSWITH:
   {
      char *prefix = ARG_STR(0, "");
      int result = str_starts_with(str, prefix);
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   case STR_ENDSWITH:
   {
      char *suffix = ARG_STR(0, "");
      int result = str_ends_with(str, suffix);
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   case STR_FIND:
   {
      char *substr = ARG_STR(0, "");
      int result = str_index_of(str, substr);
      return createData(TYPE_INT, (void *)(long)result);
   }

   case STR_INDEX:
   {
      char *substr = ARG_STR(0, "");
      int result = str_index_of(str, substr);
      if (result == -1)
      {
         throw_error(ERROR_VALUE, "substring not found");
         return createData(TYPE_NONE, NULL);
      }
      return createData(TYPE_INT, (void *)(long)result);
   }

   case STR_ISALPHA:
   {
      int result = str_is_alpha(str);
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   case STR_ISDIGIT:
   {
      int result = str_is_digit(str);
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   case STR_ISALNUM:
   {
      int result = str_is_alpha_numeric(str);
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   case STR_ISSPACE:
   {
      // TODO: Implement str_is_space
      return createData(TYPE_BOOL, (void *)0L);
   }

   case STR_ISUPPER:
   {
      int result = str_is_upper(str);
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   case STR_ISLOWER:
   {
      int result = str_is_lower(str);
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   default:
      throw_error(ERROR_SYNTAX, "unknown string method type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}

//=========================================================
//  NUMERIC METHOD DISPATCHER
//=========================================================

Data *dispatchNumMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt)
{
   switch (type)
   {
   case NUM_BIT_LENGTH:
   {
      if (object->type != TYPE_INT && object != TYPE_BOOL)
      {
         throw_error(ERROR_TYPE, "bit_length() requires int");
         return createData(TYPE_NONE, NULL);
      }
      long val = (long)*(int *)object->integer.atom;
      if (val < 0)
         val = -val;

      int length = 0;
      while (val > 0)
      {
         val >>= 1;
         length++;
      }
      return createData(TYPE_INT, (void *)(long)length);
   }

   case NUM_BIT_COUNT:
   {
      if (object->type != TYPE_INT && object != TYPE_BOOL)
      {
         throw_error(ERROR_TYPE, "bit_count() requires int");
         return createData(TYPE_NONE, NULL);
      }
      long val = (long)*(int *)object->integer.atom;
      if (val < 0)
         val = -val;

      int count = 0;
      while (val > 0)
      {
         if (val & 1)
            count++;
         val >>= 1;
      }
      return createData(TYPE_INT, (void *)(long)count);
   }

   case NUM_IS_INTEGER:
   {
      if (object->type != TYPE_FLOAT)
      {
         throw_error(ERROR_TYPE, "is_integer() requires float");
         return createData(TYPE_NONE, NULL);
      }
      double val = *(double *)object->decimal.real;
      int result = (val == floor(val));
      return createData(TYPE_BOOL, (void *)(long)result);
   }

   case NUM_HEX:
   {
      if (object->type != TYPE_FLOAT)
      {
         throw_error(ERROR_TYPE, "hex() requires float");
         return createData(TYPE_NONE, NULL);
      }
      // Convert float to hex representation
      char buffer[32];
      double val = *(double *)object->decimal.real;
      snprintf(buffer, sizeof(buffer), "%a", val);
      return createData(TYPE_STR, strdup(buffer));
   }

   default:
      throw_error(ERROR_SYNTAX, "unknown numeric method type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}

//=========================================================
//  RANGE METHOD DISPATCHER
//=========================================================

Data *dispatchRangeMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   if (!object || object->type != TYPE_RANGE)
   {
      throw_error(ERROR_TYPE, "range method called on non-range object");
      return createData(TYPE_NONE, NULL);
   }

   Range *range = RANGE_PTR(object);

   switch (type)
   {
   case RANGE_START:
      return createData(TYPE_INT, (void *)range->start);

   case RANGE_STOP:
      return createData(TYPE_INT, (void *)range->stop);

   case RANGE_STEP:
      return createData(TYPE_INT, (void *)range->step);

   case RANGE_COUNT_METHOD:
   {
      // Count occurrences of a value in the range
      Data *value = ARG(0);
      if (!value || value->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "range.count() argument must be an integer");
         return createData(TYPE_NONE, NULL);
      }

      long search_val = (long)*(int *)value->integer.atom;
      int count = 0;

      if ((search_val - range->start) % range->step == 0)
         count = 1;

      return createData(TYPE_INT, &(long){count});
   }

   case RANGE_INDEX:
   {
      // Find index of a value in the range
      Data *value = ARG(0);
      if (!value || value->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "range.index() argument must be an integer");
         return createData(TYPE_NONE, NULL);
      }

      long search_val = (long)*(int *)value->integer.atom;

      // Check if value is in range and find its index
      if (range->step > 0)
      {
         if (search_val >= range->start && search_val < range->stop)
         {
            long offset = search_val - range->start;
            if (offset % range->step == 0)
            {
               long index = offset / range->step;
               return createData(TYPE_INT, (void *)index);
            }
         }
      }
      else if (range->step < 0)
      {
         if (search_val <= range->start && search_val > range->stop)
         {
            long offset = search_val - range->start;
            if (offset % range->step == 0)
            {
               long index = offset / range->step;
               return createData(TYPE_INT, (void *)index);
            }
         }
      }

      throw_error(ERROR_VALUE, "value is not in range");
      return createData(TYPE_NONE, NULL);
   }

   default:
      throw_error(ERROR_SYNTAX, "unknown range method type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}

Data *dispatchNoneMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(type);
   UNUSED(object);
   UNUSED(args);
   UNUSED(kwargs);
   UNUSED(rt);

   // None type has no methods
   // If we somehow get here, it's an error
   throw_error(ERROR_ATTRIBUTE, "'NoneType' object has no methods");
   return createData(TYPE_NONE, NULL);
}

Data *dispatchMethod(DataType obj_type, BuiltinType method_type, Data *object,
                     List *args, Dict *kwargs, Runtime *rt)
{
   switch (obj_type)
   {
   case TYPE_LIST:
      return dispatchListMethod(method_type, object, args, kwargs, rt);
   case TYPE_DICT:
      return dispatchDictMethod(method_type, object, args, kwargs, rt);
   case TYPE_SET:
      return dispatchSetMethod(method_type, object, args, kwargs, rt);
   case TYPE_STR:
      return dispatchStrMethod(method_type, object, args, kwargs, rt);
   case TYPE_INT:
   case TYPE_BOOL:
   case TYPE_FLOAT:
      return dispatchNumMethod(method_type, object, args, kwargs, rt);
   case TYPE_RANGE:
      return dispatchRangeMethod(method_type, object, args, kwargs, rt);
   case TYPE_NONE:
      return dispatchNoneMethod(method_type, object, args, kwargs, rt);
   default:
      throw_error(ERROR_TYPE, "unsupported type for method dispatch");
      return createData(TYPE_NONE, NULL);
   }
}