#include <string.h>
#include <stdio.h>
#include "./lib/list.h"
#include "./lib/dict.h"
#include "./lib/type.h"
#include "./lib/build.h"
#include "./lib/set.h"
#include "./lib/utils.h"
#include "./lib/format.h"
#include "./lib/error.h"
#include "./lib/str.h"
#include "./lib/maloc.h"

// Function Prototypes
static char *dataToRepr(Data *data);
static char *builtinToString(Data *builtin);

char *getDataType(DataType type)
{
   switch (type)
   {
   case TYPE_INT:
      return "int";
   case TYPE_FLOAT:
      return "float";
   case TYPE_STR:
      return "string";
   case TYPE_BOOL:
      return "bool";
   case TYPE_FUNCTION:
      return "function";
   case TYPE_LIST:
      return "list";
   case TYPE_DICT:
      return "dict";
   case TYPE_SET:
      return "set";
   case TYPE_NONE:
      return "none";
   case TYPE_RANGE:
      return "range";
   case TYPE_CLASS:
      return "class";
   case TYPE_INSTANCE:
      return "class object";
   default:
      return "unknown";
   }
}

const char *getOperator(Operator op)
{
   switch (op)
   {
   case MULTIPLY:
      return "*";
   case DIVIDE:
      return "/";
   case PLUS:
      return "+";
   case MINUS:
      return "-";
   case MODULO:
      return "%";
   case FLOOR:
      return "//";
   case EXPONENT:
      return "**";
   case XOR:
      return "^";
   case BIT_OR:
      return "|";
   case BIT_AND:
      return "&";
   case LOGICAL_AND:
      return "&&";
   case LOGICAL_OR:
      return "||";
   case EQUAL:
      return "==";
   case NEQ:
      return "!=";
   case LEQ:
      return "<=";
   case GEQ:
      return ">=";
   case LST:
      return "<";
   case GRT:
      return ">";
   case NOT:
      return "!";
   case INTO:
      return "=";
   case RIGHT_SHIFT:
      return ">>";
   case LEFT_SHIFT:
      return "<<";
   case BIT_NOT:
      return "~";
   default:
      return "<?>";
   }
}

static char *listTostring(List *list)
{
   if (!list || !list->length)
      return strdup("[]");

   char *result = strdup("[");
   for (int i = 0; i < list->length; i++)
   {
      char *itemStr = dataToRepr(list->items[i]);
      result = realloc(result, strlen(result) + strlen(itemStr) + 4);

      strcat(result, itemStr);
      if (i < list->length - 1)
         strcat(result, ", ");

      free(itemStr);
   }
   strcat(result, "]");
   return result;
}

static char *dictTostring(Dict *dict)
{
   if (!dict || !dict->capacity)
      return strdup("{}");

   char *result = strdup("{");
   int count = 0;

   for (int i = 0; i < dict->size; i++)
   {
      Pair *pair = dict->buckets[i];
      while (pair)
      {
         char *keyStr = dataToRepr((Data *)pair->key);
         char *valStr = dataToRepr(pair->value);

         result = realloc(result, strlen(result) + strlen(keyStr) + strlen(valStr) + 8);
         strcat(result, keyStr);
         strcat(result, ": ");
         strcat(result, valStr);

         if (count < dict->capacity - 1)
            strcat(result, ", ");

         free(keyStr);
         free(valStr);

         count++;
         pair = pair->next;
      }
   }

   strcat(result, "}");
   return result;
}

static char *setTostring(Set *set)
{
   char *buf = malloc(2048);
   strcpy(buf, "{");
   int count = 0;

   for (int i = 0; i < set->size; i++)
   {
      if (!set->bucket[i])
         continue;

      if (set->bucket[i] == TOMBSTONE)
         continue;

      char *s = dataToRepr(set->bucket[i]);
      strcat(buf, s);
      free(s);

      count++;
      if (count < set->capacity)
         strcat(buf, ", ");
   }

   strcat(buf, "}");
   return buf;
}

static char *rangeTostring(Range *range)
{
   if (!range)
      return strdup("range(None)");

   char buffer[128];
   if (range->step == 1)
   {
      if (range->start == 0)
         snprintf(buffer, sizeof(buffer), "range(%d)", range->stop);
      else
         snprintf(buffer, sizeof(buffer), "range(%d, %d)", range->start, range->stop);
   }
   else
   {
      snprintf(buffer, sizeof(buffer), "range(%d, %d, %d)",
               range->start, range->stop, range->step);
   }
   return strdup(buffer);
}

static char *functionTostring(Function *func)
{
   if (!func)
      return strdup("<function: None>");

   char buffer[256];
   if (func->name)
      snprintf(buffer, sizeof(buffer),
               "<function %s at %p>", func->name->str, (void *)func);
   else
      snprintf(buffer, sizeof(buffer), "<lambda at %p>", (void *)func);

   return strdup(buffer);
}

static char *classToString(Class *cls)
{
   if (!cls)
      return strdup("<class 'None'>");

   if (!cls->name)
      return strdup("<class '?'>");

   // Format: <class 'ClassName'>
   size_t len = strlen(cls->name->str) + 20;
   char *result = malloc(len);
   if (!result)
      return strdup("<class>");

   snprintf(result, len, "<class '%s'>", cls->name->str);
   return result;
}

char *instanceToString(Instance *inst)
{
   if (!inst)
      return strdup("<NULL instance>");

   // Check if class pointer is valid
   if (!inst->class)
      return strdup("<instance with NULL class>");

   Class *cls = inst->class;

   // Check if class has a name
   if (!cls || !cls->name || !cls->name->str)
      return strdup("<instance of unnamed class>");

   // Build the string: <ClassName object at ADDRESS>
   char buffer[256];
   snprintf(buffer, sizeof(buffer), "<%s object at %p>",
            cls->name->str, (void *)inst);

   return strdup(buffer);
}

static char *dataToRepr(Data *data)
{
   if (!data)
      return strdup("<NULL dataance>");

   char buffer[64];
   switch (data->type)
   {
   case TYPE_INT:
      snprintf(buffer, sizeof(buffer), "%d", *(int *)data->atom);
      return strdup(buffer);

   case TYPE_STR:
   {
      size_t len = strlen(data->str);
      char *str = malloc(len + 3); // ' + text + ' + '\0'
      if (!str)
         return NULL;

      str[0] = '\'';
      strcpy(str + 1, data->str);
      str[len + 1] = '\'';
      str[len + 2] = '\0';
      return str;
   }

   case TYPE_FLOAT:
      snprintf(buffer, sizeof(buffer), "%g", *(double *)data->real);
      return strdup(buffer);

   case TYPE_BOOL:
      return strdup(*(int *)data->atom ? "true" : "false");

   case TYPE_LIST:
      return listTostring(LIST_PTR(data));
   case TYPE_SET:
      return setTostring(SET_PTR(data));

   case TYPE_DICT:
      return dictTostring(DICT_PTR(data));

   case TYPE_FUNCTION:
      return functionTostring(FUNCTION_PTR(data));

   case TYPE_RANGE:
      return rangeTostring((Range *)data->range);

   case TYPE_CLASS:
      return classToString(CLASS_PTR(data));

   case TYPE_INSTANCE:
      return instanceToString(INST_PTR(data));

   case TYPE_BUILTIN:
      return builtinToString(data->any);

   case TYPE_NONE:
   default:
      return strdup("none");
   }
}

char *dataToString(Data *data)
{
   if (!data)
      return strdup("null");

   // For strings, return without quotes (for direct printing)
   if (data->type == TYPE_STR)
      return strdup(data->str);

   // For everything else, use repr-like formatting
   return dataToRepr(data);
}

static char *builtinToString(Data *builtin)
{
   if (!builtin || builtin->type != TYPE_BUILTIN)
      return strdup("<built-in function>");

   const BuiltinInfo *info = (const BuiltinInfo *)builtin->any;

   if (!info || !info->name)
      return strdup("<built-in function>");

   // Format: <built-in function 'name'>
   size_t len = strlen(info->name) + 30;
   char *result = malloc(len);
   if (!result)
      return strdup("<built-in function>");

   snprintf(result, len, "<built-in function '%s'>", info->name);
   return result;
}

char *dataTostring(Data *d)
{
   if (!d)
      return strdup("null");

   // For strings, return without quotes (for direct printing)
   if (d->type == TYPE_STR)
      return strdup(d->str);

   // For everything else, use repr-like formatting
   return dataToRepr(d);
}

int dataToBool(Data *data)
{
   if (!data)
      return 0;

   switch (data->type)
   {
   case TYPE_BOOL:
      return *(int *)data->atom;
   case TYPE_INT:
      return *(int *)data->atom != 0;
   case TYPE_FLOAT:
      return *(double *)data->real != 0.0;
   case TYPE_STR:
      return data->str && strlen(data->str) > 0;
   case TYPE_LIST:
      if (LIST_PTR(data))
         return LIST_PTR(data)->length > 0;
      return 0;
   case TYPE_SET:
      if (SET_PTR(data))
         return SET_PTR(data)->capacity > 0;
      return 0;
   case TYPE_DICT:
      if (DICT_PTR(data))
         return DICT_PTR(data)->size > 0;
      return 0;

   case TYPE_INSTANCE:
   case TYPE_CLASS:
   case TYPE_FUNCTION:
   case TYPE_RANGE:
      return 1; // Functions are always truthy
   default:
      return 0;
   }
}

static List *strToList(char *str)
{
   List *list = list_create(strlen(str));
   if (!list)
      return NULL;

   size_t len = strlen(str);

   for (size_t i = 0; i < len; i++)
   {
      char *character = str_char_at(str, (int)i);
      Data *ch = createData(TYPE_STR, character);
      list_append(ch, list);
   }

   return list;
}

static List *setToList(Set *set)
{
   if (!set)
      return NULL;

   List *list = list_create(set->capacity);

   for (int i = 0; i < set->size; i++)
   {
      Data *d = (Data *)set->bucket[i];
      if (d != TOMBSTONE && d != NULL)
         list_append(cloneData(d), list);
   }
   return list;
}

static List *rangeToList(Range *r)
{
   List *list = list_create(abs((r->stop - r->start) / r->step) + 1);
   int value = r->start;

   while (value != r->stop)
   {
      Data *elem = createData(TYPE_INT, &value);
      list_append(elem, list);
      value += r->step;
   }

   return list;
}

List *resolveIterable(Data *iterable)
{
   if (!iterable)
      return list_create(__size__);

   switch (iterable->type)
   {
   case TYPE_LIST:
      return list_clone(LIST_PTR(iterable));
   case TYPE_SET:
      return setToList(SET_PTR(iterable));
   case TYPE_RANGE:
      return rangeToList(iterable->range);
   case TYPE_STR:
      return strToList(iterable->str);
   default:
      throw_error(
          ERROR_TYPE,
          "'%s' is not iterable",
          getDataType(iterable->type));
      break;
   }
   return NULL;
}