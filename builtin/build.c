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

// Core builtins
const BuiltinInfo BUILTIN_REGISTRY[] = {
    {BUILTIN_PRINT, "print", 0, -1},
    {BUILTIN_INPUT, "input", 0, 1},
    {BUILTIN_TYPE, "type", 1, 1},
    {BUILTIN_INT, "int", 1, 2},
    {BUILTIN_FLOAT, "float", 1, 1},
    {BUILTIN_STR, "str", 1, 1},
    {BUILTIN_BOOL, "bool", 1, 1},
    {BUILTIN_LIST, "list", 0, 1},
    {BUILTIN_DICT, "dict", 0, 1},
    {BUILTIN_SET, "set", 0, 1},
    {BUILTIN_RANGE, "range", 1, 3},
    {BUILTIN_LEN, "len", 1, 1},
    {BUILTIN_ABS, "abs", 1, 1},
    {BUILTIN_MIN, "min", 1, -1},
    {BUILTIN_MAX, "max", 1, -1},
    {BUILTIN_SUM, "sum", 1, 2},
    {BUILTIN_POW, "pow", 2, 3},
    {BUILTIN_ROUND, "round", 1, 2},
    {BUILTIN_SORTED, "sorted", 1, 2},
    {BUILTIN_REVERSED, "reversed", 1, 1},
};

// Type-specific method registries
// List Methods
const BuiltinInfo LIST_REGISTRY[] = {
    {LIST_APPEND, "append", 1, 1},
    {LIST_EXTEND, "extend", 1, 1},
    {LIST_INSERT, "insert", 2, 2},
    {LIST_REMOVE, "remove", 1, 1},
    {LIST_POP, "pop", 0, 1},
    {LIST_CLEAR, "clear", 0, 0},
    {LIST_INDEX, "index", 1, 3},
    {LIST_REVERSE, "reverse", 0, 0},
    {LIST_COPY, "copy", 0, 0},
};

// Dict Methods
const BuiltinInfo DICT_REGISTRY[] = {
    {DICT_GET, "get", 1, 2},
    {DICT_KEYS, "keys", 0, 0},
    {DICT_VALUES, "values", 0, 0},
    {DICT_ITEMS, "items", 0, 0},
    {DICT_POP, "pop", 1, 2},
    {DICT_POPITEM, "popitem", 0, 0},
    {DICT_CLEAR, "clear", 0, 0},
    {DICT_UPDATE, "update", 1, 1},
    {DICT_SETDEFAULT, "setdefault", 1, 2},
    {DICT_COPY, "copy", 0, 0},
};

// Set Methods
const BuiltinInfo SET_REGISTRY[] = {
    {SET_ADD, "add", 1, 1},
    {SET_REMOVE, "remove", 1, 1},
    {SET_DISCARD, "discard", 1, 1},
    {SET_CLEAR, "clear", 0, 0},
    {SET_UNION, "union", 1, -1},
    {SET_INTERSECTION, "intersection", 1, -1},
    {SET_DIFFERENCE, "difference", 1, -1},
    {SET_SYMMETRIC_DIFFERENCE, "symmetric_difference", 1, 1},
    {SET_COPY, "copy", 0, 0},
    {SET_HAS, "has", 1, 1},
};

// String Methods
const BuiltinInfo STR_REGISTRY[] = {
    {STR_UPPER, "upper", 0, 0},
    {STR_LOWER, "lower", 0, 0},
    {STR_CAPITALIZE, "capitalize", 0, 0},
    {STR_TITLE, "title", 0, 0},
    {STR_STRIP, "strip", 0, 1},
    {STR_LSTRIP, "lstrip", 0, 1},
    {STR_RSTRIP, "rstrip", 0, 1},
    {STR_REPLACE, "replace", 2, 3},
    {STR_SPLIT, "split", 0, 2},
    {STR_JOIN, "join", 1, 1},
    {STR_STARTSWITH, "startswith", 1, 3},
    {STR_ENDSWITH, "endswith", 1, 3},
    {STR_FIND, "find", 1, 3},
    {STR_INDEX, "index", 1, 3},
    {STR_ISALPHA, "isalpha", 0, 0},
    {STR_ISDIGIT, "isdigit", 0, 0},
    {STR_ISALNUM, "isalnum", 0, 0},
    {STR_ISSPACE, "isspace", 0, 0},
    {STR_ISUPPER, "isupper", 0, 0},
    {STR_ISLOWER, "islower", 0, 0},
};

// Int/Bool Methods
const BuiltinInfo INT_REGISTRY[] = {
    {NUM_BIT_LENGTH, "bit_length", 0, 0},
    {NUM_BIT_COUNT, "bit_count", 0, 0},
};

// Float Methods
const BuiltinInfo FLOAT_REGISTRY[] = {
    {NUM_IS_INTEGER, "is_integer", 0, 0},
    {NUM_HEX, "hex", 0, 0},
};

// Range Methods
const BuiltinInfo RANGE_REGISTRY[] = {
    {RANGE_COUNT_METHOD, "count", 1, 1},
    {RANGE_INDEX, "index", 1, 1},
};

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
   return (d && d->type == TYPE_INT && d->atom) ? *(int *)d->atom : def;
}

static inline const char *arg_str(List *args, int i, const char *def)
{
   Data *d = arg_data(args, i);
   return (d && d->type == TYPE_STR && d->str) ? d->str : def;
}

//=========================================================
//  HELPER: Initialize Single Registry into Dict
//=========================================================

void initializeRegistry(Dict *dict, const BuiltinInfo *registry)
{
   if (!dict || !registry)
      return;

   size_t i = 0;

   while (registry[i].name != NULL)
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
            data_free(key);
         if (val)
            data_free(val);
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
   initializeRegistry(methods->str, STR_REGISTRY);

   // Initialize int/bool methods
   initializeRegistry(methods->atom, INT_REGISTRY);

   // Initialize float methods
   initializeRegistry(methods->real, FLOAT_REGISTRY);

   // Initialize range methods
   initializeRegistry(methods->range, RANGE_REGISTRY);

   // Initialize error classes
   initializeErrorClass(rt);
}

//=========================================================
//  CORE BUILTIN DISPATCHER
//=========================================================

Data *dispatchBuiltin(BuiltinType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(rt);

   if (!args)
      args = list_create(__size__);

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
      if (list_remove(value, list, data_free) == LIST_ERROR)
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
         list_remove(value, list, data_free);
         return (Data *)value;
      }
   }

   case LIST_CLEAR:
      list_clear(list, data_free);
      return createData(TYPE_NONE, NULL);

   case LIST_INDEX:
   {
      void *value = ARG(0);
      int idx = list_index(value, list);
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
      void *result = dict_get(key, dict);
      if (result)
         return (Data *)result;
      return default_val ? (Data *)default_val : createData(TYPE_NONE, NULL);
   }

   case DICT_KEYS:
   {
      List *keys = list_create(dict->capacity);
      dict_keys(dict, keys->items);
      return createData(TYPE_LIST, keys);
   }

   case DICT_VALUES:
   {
      List *values = list_create(dict->capacity);
      dict_values(dict, values->items);
      return createData(TYPE_LIST, values);
   }

   case DICT_ITEMS:
   {
      List *items = list_create(dict->capacity);
      dict_items(dict, items->items);
      return createData(TYPE_LIST, items);
   }

   case DICT_POP:
   {
      void *key = ARG(0);
      void *result = dict_pop(key, dict);
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
      int def = dict_setdefault(ARG(0), val, dict);
      return createData(TYPE_INT, &def);
   }

   case DICT_COPY:
   {
      Dict *copy = dict_clone(dict);
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

   switch (type)
   {
   case SET_ADD:
      set_add(ARG(0), set);
      return createData(TYPE_NONE, NULL);

   case SET_REMOVE:
   {
      set_remove(ARG(0), set);
      return createData(TYPE_NONE, NULL);
   }

   case SET_DISCARD:
      set_remove(ARG(0), set);
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
      Set *result = set_union(set, SET_PTR(other));
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
      Set *result = set_intersection(set, SET_PTR(other));
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
      Set *result = set_difference(set, SET_PTR(other));
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
      Set *result = set_symdiff(set, SET_PTR(other));
      return createData(TYPE_SET, result);
   }

   case SET_COPY:
   {
      Set *copy = set_clone(set);
      return createData(TYPE_SET, copy);
   }

   case SET_HAS:
   {
      void *value = ARG(0);
      int has = set_has(value, set);
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

   const char *str = object->str;

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
      const char *old_str = ARG_STR(0, "");
      const char *new_str = ARG_STR(1, "");
      char *result = str_replace_all(str, old_str, new_str);
      return createData(TYPE_STR, result);
   }

   case STR_SPLIT:
   {
      // TODO: Implement str_split function
      // For now, return empty list
      return createData(TYPE_LIST, list_create(0));
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
            total_len += strlen(item->str);
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
            strcat(result, item->str);
            if (i < list->length - 1)
               strcat(result, str);
         }
      }

      return createData(TYPE_STR, result);
   }

   case STR_STARTSWITH:
   {
      const char *prefix = ARG_STR(0, "");
      int result = str_starts_with(str, prefix);
      return createData(TYPE_BOOL, &result);
   }

   case STR_ENDSWITH:
   {
      const char *suffix = ARG_STR(0, "");
      int result = str_ends_with(str, suffix);
      return createData(TYPE_BOOL, &result);
   }

   case STR_FIND:
   {
      const char *substr = ARG_STR(0, "");
      int result = str_index_of(str, substr);
      return createData(TYPE_INT, &result);
   }

   case STR_INDEX:
   {
      const char *substr = ARG_STR(0, "");
      int result = str_index_of(str, substr);
      if (result == -1)
      {
         throw_error(ERROR_VALUE, "substring not found");
         return createData(TYPE_NONE, NULL);
      }
      return createData(TYPE_INT, &result);
   }

   case STR_ISALPHA:
   {
      int result = str_is_alpha(str);
      return createData(TYPE_BOOL, &result);
   }

   case STR_ISDIGIT:
   {
      int result = str_is_digit(str);
      return createData(TYPE_BOOL, &result);
   }

   case STR_ISALNUM:
   {
      int result = str_is_alpha_numeric(str);
      return createData(TYPE_BOOL, &result);
   }

   case STR_ISSPACE:
   {
      // TODO: Implement str_is_space
      return createData(TYPE_BOOL, (void *)0L);
   }

   case STR_ISUPPER:
   {
      int result = str_is_upper(str);
      return createData(TYPE_BOOL, &result);
   }

   case STR_ISLOWER:
   {
      int result = str_is_lower(str);
      return createData(TYPE_BOOL, &result);
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
      if (object->type != TYPE_INT && object->type != TYPE_BOOL)
      {
         throw_error(ERROR_TYPE, "bit_length() requires int");
         return createData(TYPE_NONE, NULL);
      }
      long val = (long)*(int *)object->atom;
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
      if (object->type != TYPE_INT && object->type != TYPE_BOOL)
      {
         throw_error(ERROR_TYPE, "bit_count() requires int");
         return createData(TYPE_NONE, NULL);
      }
      long val = (long)*(int *)object->atom;
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
      double val = *(double *)object->real;
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
      double val = *(double *)object->real;
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

   Range *range = object->range;

   switch (type)
   {
   case RANGE_START:
      return createData(TYPE_INT, &(int){range->start});

   case RANGE_STOP:
      return createData(TYPE_INT, &(int){range->stop});

   case RANGE_STEP:
      return createData(TYPE_INT, &(int){range->step});

   case RANGE_COUNT_METHOD:
   {
      // Count occurrences of a value in the range
      Data *value = ARG(0);
      if (!value || value->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "range.count() argument must be an integer");
         return createData(TYPE_NONE, NULL);
      }

      long search_val = (long)*(int *)value->atom;
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

      long search_val = (long)*(int *)value->atom;

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

static const BuiltinInfo *handleBuiltin(Data *object, Data *method, List *args)
{
   if (!method || !object)
      return NULL;

   const BuiltinInfo *info = (const BuiltinInfo *)method->any;

   if (!info)
      return NULL;

   int arglen = args ? args->length : 0;
   if (arglen < info->min_args)
   {
      throw_error(
          ERROR_TYPE, "%s.%s() takes at least %d argument(s), %d given",
          getDataType(object->type), info->name, info->min_args, arglen);
      return NULL;
   }

   if (info->max_args != -1 && arglen > info->max_args)
   {
      throw_error(
          ERROR_TYPE, "%s.%s() takes at most %d argument(s), %d given",
          getDataType(object->type), info->name, info->max_args, arglen);
      return NULL;
   }

   return (const BuiltinInfo *)info;
}

Data *dispatchMethod(Data *object, Data *method, List *args, Dict *kwargs, Runtime *rt)
{
   const BuiltinInfo *info = handleBuiltin(object, method, args);

   if (!info)
      return createData(TYPE_NONE, NULL);

   switch (object->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
   case TYPE_FLOAT:
      return dispatchNumMethod(info->type, object, args, kwargs, rt);
   case TYPE_LIST:
      return dispatchListMethod(info->type, object, args, kwargs, rt);
   case TYPE_DICT:
      return dispatchDictMethod(info->type, object, args, kwargs, rt);
   case TYPE_SET:
      return dispatchSetMethod(info->type, object, args, kwargs, rt);
   case TYPE_STR:
      return dispatchStrMethod(info->type, object, args, kwargs, rt);
   case TYPE_RANGE:
      return dispatchRangeMethod(info->type, object, args, kwargs, rt);
   case TYPE_NONE:
      return dispatchNoneMethod(info->type, object, args, kwargs, rt);
   default:
      throw_error(ERROR_TYPE, "unsupported type for method dispatch");
      return createData(TYPE_NONE, NULL);
   }
}
