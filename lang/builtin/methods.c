//=========================================================
//  Built-in Functions for Custom Language
//  ---------------------------------------------------------
//  Provides core functionality like print, len, type, etc.
//=========================================================

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "../lib/type.h"
#include "../lib/dict.h"
#include "../lib/utils.h"
#include "../lib/list.h"
#include "../lib/maloc.h"
#include "../lib/daloc.h"
#include "../lib/set.h"
#include "../lib/str.h"
#include "../lib/error.h"
#include "../lib/format.h"

extern FILE *g_input_stream;

//=========================================================
//  I/O Functions
//=========================================================

// print(*args, sep=' ', end='\n')
// Prints values to stdout with customizable separator and ending
Data *builtin_print(List *args, Dict *kwargs)
{
   // Extract sep and end from kwargs if provided
   char *sep = " ";
   char *end = "\n";

   if (kwargs)
   {
      Data *sep_key = createData(TYPE_STR, "sep");
      Data *sep_data = dict_get(sep_key, kwargs);

      if (sep_data && sep_data->type == TYPE_STR)
         sep = sep_data->str;

      Data *end_key = createData(TYPE_STR, "end");
      Data *end_data = dict_get(end_key, kwargs);

      if (end_data && end_data->type == TYPE_STR)
         end = end_data->str;

      data_free(sep_key);
      data_free(end_key);
   }

   // Print all arguments
   for (int i = 0; i < args->length; i++)
   {
      if (i > 0)
         printf("%s", sep);

      Data *arg = (Data *)args->items[i];
      printf("%s", dataTostring(arg));
   }

   printf("%s", end);
   return createData(TYPE_NONE, NULL);
}

static int evalRangePart(Data *val)
{
   if (!isInt(val))
   {
      throw_error(ERROR_TYPE, "'%s' object cannot be interpreted as an integer", getDataType(val->type));
      data_free(val);
      return 0;
   }

   return *(int *)val->atom;
}

// input(prompt="")
// Reads a line from stdin and returns as string
Data *builtin_input(const char *prompt)
{
   if (prompt)
   {
      printf("%s", prompt);
      fflush(stdout);
   }

   char buffer[1024];

   // Try to read from stdin
   if (fgets(buffer, sizeof(buffer), stdin) != NULL)
   {
      // Remove trailing newline
      size_t len = strlen(buffer);
      if (len > 0 && buffer[len - 1] == '\n')
      {
         buffer[len - 1] = '\0';
      }
      return createData(TYPE_STR, strdup(buffer));
   }

   // If fgets fails (EOF or error), return empty string
   return createData(TYPE_STR, strdup(""));
}

//=========================================================
//  Type Checking and Conversion Functions
//=========================================================

// type(obj)
// Returns the type of an object as a string
Data *builtin_type(Data *obj)
{
   char *type_name = "";

   if (obj->type == TYPE_INSTANCE)
   {
      Instance *inst = INST_PTR(obj);
      char buffer[256];
      snprintf(buffer, sizeof(buffer), "<class '%s'>", inst->class->name->str);
      type_name = strdup(buffer);
   }
   else
      type_name = strdup(getDataType(obj->type));

   return createData(TYPE_STR, type_name);
}

// int(x)
// Converts value to integer
Data *builtin_int(Data *obj)
{
   int result = 0;
   switch (obj->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
      result = *(int *)obj->atom;
      break;
   case TYPE_FLOAT:
      result = (int)(*(double *)obj->real);
      break;
   case TYPE_STR:
      result = atoi((char *)obj->str);
      break;
   default:
      fprintf(stderr, "Error: Cannot convert type to int\n");
      return createData(TYPE_INT, &result);
   }
   return createData(TYPE_INT, &result);
}

// float(x)
// Converts value to float
Data *builtin_float(Data *obj)
{
   double result = 0.0;
   switch (obj->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
      result = (double)(*(int *)obj->atom);
      break;
   case TYPE_FLOAT:
      result = *(double *)obj->real;
      break;
   case TYPE_STR:
      result = atof((char *)obj->str);
      break;
   default:
      fprintf(stderr, "Error: Cannot convert type to float\n");
      return createData(TYPE_FLOAT, &result);
   }
   return createData(TYPE_FLOAT, &result);
}

// str(x)
// Converts value to string
Data *builtin_str(Data *obj)
{
   return createData(TYPE_STR, dataTostring(obj));
}

// bool(x)
// Converts value to boolean
Data *builtin_bool(Data *obj)
{
   int result = dataToBool(obj);
   return createData(TYPE_BOOL, &result);
}

//=========================================================
//  Collection Functions
//=========================================================

static int rangelen(const Range *r)
{
   return (int)ceil((r->stop - r->start) / r->step);
}

// len(collection)
// Returns the length of a collection
Data *builtin_len(Data *obj)
{
   int length = 0;

   switch (obj->type)
   {
   case TYPE_STR:
      length = strlen((char *)obj->str);
      break;
   case TYPE_LIST:
   {
      List *list = LIST_PTR(obj);
      length = list->length;
      break;
   }
   case TYPE_DICT:
   {
      Dict *dict = DICT_PTR(obj);
      length = dict->capacity;
      break;
   }
   case TYPE_SET:
   {
      Set *set = SET_PTR(obj);
      length = set->capacity;
      break;
   }

   case TYPE_RANGE:
      length = rangelen(obj->range);
      break;

   default:
      fprintf(stderr, "Error: Type has no length\n");
      return createData(TYPE_INT, &length);
   }
   return createData(TYPE_INT, &length);
}

//=========================================================
//  Mathematical Functions
//=========================================================

// abs(x)
// Returns absolute value
Data *builtin_abs(Data *obj)
{
   if (obj->type == TYPE_INT)
   {
      int value = abs(*(int *)obj->atom);
      return createData(TYPE_INT, &value);
   }
   else if (obj->type == TYPE_FLOAT)
   {
      double value = fabs(*(double *)obj->atom);
      return createData(TYPE_FLOAT, &value);
   }

   throw_error(ERROR_TYPE, "Error: abs() requires numeric type\n");
   return obj;
}

// pow(base, exp)
// Returns base raised to exp
Data *builtin_pow(Data *base, Data *exp)
{
   double b = 0.0, e = 0.0;

   if (base->type == TYPE_INT)
      b = (double)(*(int *)base->atom);
   else if (base->type == TYPE_FLOAT)
      b = *(double *)base->real;
   else
   {
      fprintf(stderr, "Error: pow() requires numeric types\n");
      return createData(TYPE_NONE, NULL);
   }

   if (exp->type == TYPE_INT)
      e = (double)(*(int *)exp->atom);
   else if (exp->type == TYPE_FLOAT)
      e = *(double *)exp->real;
   else
   {
      fprintf(stderr, "Error: pow() requires numeric types\n");
      return createData(TYPE_NONE, NULL);
   }

   double result = pow(b, e);
   return createData(TYPE_FLOAT, &result);
}

// round(x, ndigits=0)
// Rounds float to given precision
Data *builtin_round(Data *obj, int ndigits)
{
   double value = 0.0;

   if (obj->type == TYPE_INT)
      return obj;

   else if (obj->type == TYPE_FLOAT)
      value = *(double *)obj->real;

   else
   {
      throw_error(ERROR_TYPE, "round() requires numeric type\n");
      return NULL;
   }

   double multiplier = pow(10.0, ndigits);
   value = round(value * multiplier) / multiplier;

   if (ndigits == 0)
   {
      int int_val = (int)value;
      return createData(TYPE_INT, &int_val);
   }

   return createData(TYPE_FLOAT, &value);
}

//=========================================================
//  String Functions
//=========================================================

// split(str, sep=' ')
// Splits string into list
Data *builtin_split(Data *str_data, const char *sep)
{
   if (str_data->type != TYPE_STR)
   {
      fprintf(stderr, "Error: split() requires a string\n");
      return createData(TYPE_LIST, list_create(__len__));
   }

   if (sep == NULL)
      sep = " ";

   char *str = strdup((char *)str_data->str);
   List *result = list_create(__len__);

   char *token = strtok(str, sep);
   while (token != NULL)
   {
      Data *elem = createData(TYPE_STR, strdup(token));
      list_append(elem, result);
      token = strtok(NULL, sep);
   }

   free(str);
   return createData(TYPE_LIST, result);
}

// join(list, sep='')
// Joins list of strings with separator
Data *builtin_join(Data *list_data, const char *sep)
{
   if (list_data->type != TYPE_LIST)
   {
      fprintf(stderr, "Error: join() requires a list\n");
      return createData(TYPE_STR, strdup(""));
   }

   if (sep == NULL)
      sep = "";

   List *list = LIST_PTR(list_data);
   size_t total_len = 0;

   // Calculate total length
   for (int i = 0; i < list->length; i++)
   {
      if (((Data *)list->items[i])->type == TYPE_STR)
         total_len += strlen((char *)((Data *)(list->items[i]))->str);

      if (i < list->length - 1)
         total_len += strlen(sep);
   }

   char *result = malloc(total_len + 1);
   result[0] = '\0';

   for (int i = 0; i < list->length; i++)
   {
      if (((Data *)list->items[i])->type == TYPE_STR)
      {
         strcat(result, (char *)((Data *)list->items[i])->str);
      }
      if (i < list->length - 1)
      {
         strcat(result, sep);
      }
   }

   return createData(TYPE_STR, result);
}

// reversed(list)
// Returns reversed copy of list

Data *builtin_list(Data *data)
{
   // If no argument provided, return empty list
   if (!data || data->type == TYPE_NONE)
      return createData(TYPE_LIST, list_create(__len__));

   // Otherwise, convert the iterable to a list
   return createData(TYPE_LIST, resolveIterable(data));
}

Data *builtin_dict(Data *data)
{
   if (data->type != TYPE_DICT)
   {
      throw_error(ERROR_VALUE, "dictionary sequence can't be updated");
      return NULL;
   }
   Dict *dict = dict_clone(DICT_PTR(data));
   if (dict)
      return createData(TYPE_DICT, dict);

   return createData(TYPE_DICT, dict_create(256));
}

Data *builtin_set(Data *data)
{
   if (!data || data->type == TYPE_NONE)
      return createData(TYPE_SET, set_create());

   Set *set = NULL;
   if (data->type == TYPE_LIST)
   {
      set = set_create();
      List *list = LIST_PTR(data);

      for (int i = 0; i < list->length; i++)
         set_add(list->items[i], set);
   }

   else if (data->type == TYPE_DICT)
   {
      Dict *dict = DICT_PTR(data);
      set = set_create();

      for (int i = 0; i < dict->size; i++)
      {
         Pair *pair = dict->buckets[i];
         while (pair)
         {
            set_add(cloneData(pair->key), set);
            pair = pair->next;
         }
      }
   }

   else if (data->type == TYPE_STR)
   {
      set = set_create();
      const char *str = data->str;

      for (int i = 0; i < (int)strlen(str); i++)
      {
         char *character = str_char_at(str, i);
         set_add(createData(TYPE_STR, character), set);
      }
   }

   else if (data->type == TYPE_SET)
      set = set_clone(SET_PTR(data));

   else if (data->type == TYPE_RANGE)
   {
      set = set_create();
      Range *r = data->range;
      for (int i = r->start; i < r->stop; i += r->step)
         set_add(createData(TYPE_INT, &i), set);
   }

   else
   {
      throw_error(ERROR_TYPE, "'%s' object is not iterable");
      return NULL;
   }
   Data *object = createData(TYPE_SET, set);
   object->ref->isEvaluated = 1;
   return object;
}

Data *builtin_range(Data *arg1, Data *arg2, Data *arg3)
{
   int start = 0, stop = 0, step = 1;

   // Case 1: range(stop) - only arg1 provided
   if (arg1 && !arg2 && !arg3)
      stop = evalRangePart(arg1);

   // Case 2: range(start, stop) - arg1 and arg2 provided
   else if (arg1 && arg2 && !arg3)
   {
      start = evalRangePart(arg1);
      stop = evalRangePart(arg2);
   }
   // Case 3: range(start, stop, step) - all three provided
   else if (arg1 && arg2 && arg3)
   {
      start = evalRangePart(arg1);
      stop = evalRangePart(arg2);
      step = evalRangePart(arg3);
   }

   return createData(TYPE_RANGE, createRange(start, stop, step));
}

// ===============================================================
//       LIST METHODS
//================================================================
static int compare_data(const void *a, const void *b)
{
   Data *data_a = *(Data **)a;
   Data *data_b = *(Data **)b;

   // Handle NULL cases
   if (!data_a && !data_b)
      return 0;
   if (!data_a)
      return -1;
   if (!data_b)
      return 1;

   // If types are different, sort by type precedence
   // Order: None < Bool < Int < Float < Str < List < Dict < Set
   if (isNumeric(data_a->type) && isNumeric(data_b->type))
   {
      long val_a = valueToFloat(data_a);
      long val_b = valueToFloat(data_b);
      return (val_a > val_b) - (val_a < val_b);
   }

   if (data_a->type != data_b->type)
   {
      return data_a->type - data_b->type;
   }

   // Same type comparison
   switch (data_a->type)
   {
   case TYPE_NONE:
      return 0;

   case TYPE_STR:
   {
      char *str_a = (char *)data_a->str;
      char *str_b = (char *)data_b->str;
      if (!str_a && !str_b)
         return 0;
      if (!str_a)
         return -1;
      if (!str_b)
         return 1;
      return strcmp(str_a, str_b);
   }

   case TYPE_LIST:
   {
      // Lexicographic comparison for lists
      List *list_a = LIST_PTR(data_a);
      List *list_b = LIST_PTR(data_b);

      if (!list_a && !list_b)
         return 0;
      if (!list_a)
         return -1;
      if (!list_b)
         return 1;

      int min_len = (list_a->length < list_b->length) ? list_a->length : list_b->length;
      for (int i = 0; i < min_len; i++)
      {
         int cmp = compare_data(&list_a->items[i], &list_b->items[i]);
         if (cmp != 0)
            return cmp;
      }

      // If all elements are equal, shorter list comes first
      return (list_a->length > list_b->length) - (list_a->length < list_b->length);
   }

   default:
      return 0;
   }
}

Data *builtin_sorted(Data *object)
{
   List *list;
   if (object->type == TYPE_LIST)
      list = LIST_PTR(object);
   else
      list = resolveIterable(object);

   List *result = list_create(list->capacity);
   if (!result)
      return NULL;

   for (int i = 0; i < list->length; i++)
   {
      Data *cloned = cloneData(list->items[i]);
      if (!cloned)
      {
         // Cleanup on failure
         list_clear(result, data_free);
         free(result->items);
         free(result);
         return NULL;
      }
      if (list_append(cloned, result) != LIST_SUCCESS)
      {
         data_free(cloned);
         list_clear(result, data_free);
         free(result->items);
         free(result);
         return NULL;
      }
   }
   qsort(result->items, result->length, sizeof(void *), compare_data);
   return createData(TYPE_LIST, result);
}
