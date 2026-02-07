#include <string.h>
#include <stdio.h>
#include "./lib/type.h"

// Function Prototypes
static char *instanceToRepr(Instance *inst);

const char *getDataType(DataType type)
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
      return "instance";
   case TYPE_INVOKED:
      return "invoked";
   case TYPE_OPERATOR:
      return "operator";
   case TYPE_LOOKUP:
      return "lookup";
   case TYPE_INDEX:
      return "index";
   case TYPE_BUILTIN:
      return "builtin";
   case TYPE_AST:
      return "ast";
   default:
      return "unknown";
   }
}

static char *listTostring(List *list)
{
   if (!list || !list->length)
      return strdup("[]");

   char *result = strdup("[");
   for (int i = 0; i < list->length; i++)
   {
      char *itemStr = instanceToRepr(list->items[i]);
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
         char *keyStr =
             instanceToRepr((Instance *)pair->key);
         char *valStr =
             instanceToRepr(pair->value);

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

      char *s = instanceToRepr(set->bucket[i]);
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
               "<function %s at %p>", func->name, (void *)func);
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
   size_t len = strlen(cls->name) + 20;
   char *result = malloc(len);
   if (!result)
      return strdup("<class>");

   snprintf(result, len, "<class '%s'>", cls->name);
   return result;
}

static char *instanceToRepr(Instance *inst)
{
   if (!inst)
      return strdup("<NULL instance>");

   char buffer[64];
   switch (inst->type)
   {
   case TYPE_INT:
      snprintf(buffer, sizeof(buffer), "%d", *(int *)inst->integer);
      return strdup(buffer);

   case TYPE_STR:
   {
      size_t len = strlen(inst->string);
      char *str = malloc(len + 3); // ' + text + ' + '\0'
      if (!str)
         return NULL;

      str[0] = '\'';
      strcpy(str + 1, inst->string);
      str[len + 1] = '\'';
      str[len + 2] = '\0';
      return str;
   }

   case TYPE_FLOAT:
      snprintf(buffer, sizeof(buffer), "%g", *(double *)inst->decimal);
      return strdup(buffer);

   case TYPE_BOOL:
      return strdup(*(int *)inst->integer ? "true" : "false");

   case TYPE_LIST:
      return listTostring(LIST_PTR(inst));
   case TYPE_SET:
      return setTostring(SET_PTR(inst));

   case TYPE_DICT:
      return dictTostring(DICT_PTR(inst));

   case TYPE_FUNCTION:
      return functionTostring(FUNCTION_PTR(inst));

   case TYPE_RANGE:
      return rangeTostring((Range *)inst->range);

   case TYPE_CLASS:
      return classToString(CLASS_PTR(inst));

   case TYPE_NONE:
   default:
      return strdup("none");
   }
}

static char *instanceToString(Instance *inst)
{
   if (!inst)
      return strdup("null");

   // For strings, return without quotes (for direct printing)
   if (inst->type == TYPE_STR)
      return strdup(inst->string);

   // For everything else, use repr-like formatting
   return instanceToRepr(inst);
}

static char *builtinToString(Data *builtin)
{
   if (!builtin || builtin->type != TYPE_BUILTIN)
      return strdup("<built-in function>");

   const BuiltinInfo *info = (const BuiltinInfo *)builtin->builtin;

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
   if (d->type == TYPE_INSTANCE)
      return instanceToString(d->instance);

   if (d->type == TYPE_BUILTIN)
      return builtinToString(d->builtin);

   return strdup("<TYPE NOT IMPLEMENTED>");
}