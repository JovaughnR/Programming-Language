#include <stdlib.h>
#include "./lib/error.h"
#include "./lib/list.h"
#include "./lib/dict.h"
#include "./lib/type.h"
#include "./lib/set.h"

static Instance *cloneInstance(DataType type, const Instance *src)
{
   Instance *copy = malloc(sizeof(Instance));
   switch (type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
      copy->integer = malloc(sizeof(int));
      *copy->integer = *src->integer;
      break;

   case TYPE_FLOAT:
      copy->decimal = malloc(sizeof(double));
      *copy->decimal = *src->decimal;
      break;

   case TYPE_STR:
      copy->string = strdup(src->string);
      break;

   case TYPE_RANGE:
   {
      Range *r = src->range;
      copy->range = createRange(r->start, r->stop, r->step);
      break;
   }
   case TYPE_LIST:
   case TYPE_DICT:
   case TYPE_SET:
   case TYPE_FUNCTION:
   case TYPE_CLASS:
      copy->ref = src->ref;
      src->ref->references++;
      break;
   }

   copy->attributes = src->attributes;
   return copy;
}

Data *cloneData(const Data *src)
{
   if (!src)
      return NULL;

   Data *copy = malloc(sizeof(Data));
   if (!copy)
      return NULL;

   copy->type = src->type;

   switch (src->type)
   {
   case TYPE_LOOKUP:
      copy->var = src->var;
      break;

   case TYPE_INDEX:
      copy->index = src->index;
      break;

   case TYPE_BUILTIN:
      copy->builtin = src->builtin;
      break;

   case TYPE_ATTRIBUTE:
      copy->attribute = src->attribute;
      break;

   case TYPE_INVOKED:
      copy->invoked = src->invoked;
      break;

   case TYPE_INSTANCE:
      copy->instance =
          cloneInstance(src->type, src->instance);
      break;

   default:
      copy->any = src->any;
      break;
   }
   return copy;
}

int isInt(Data *value)
{
   return (value->type == TYPE_INT || value->type == TYPE_BOOL);
}

int isInteger(double value)
{
   return (int)value == value;
}

int isNumeric(DataType type)
{
   return (type == TYPE_INT || type == TYPE_FLOAT || type == TYPE_BOOL);
}

int isHashable(DataType type)
{
   return (isNumeric(type) || type == TYPE_RANGE || type == TYPE_STR);
}

int isInstance(Instance *a, DataType type)
{
   return a->type == type;
}

/**
 * @brief Test if two Instance objects share the same Datatype.
 */
int isSameType(Instance *a, Instance *b)
{
   return a->type == b->type;
}

double valueToFloat(Instance *instance)
{
   if (instance->type == TYPE_FLOAT)
      return *(double *)instance->decimal;
   else
      return (double)*(int *)instance->integer;
}

unsigned long hash(void *key)
{
   unsigned long h = 0x811C9DC5;
   Instance *inst = (Instance *)key;

   switch (inst->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
      h ^= (unsigned long)(*inst->integer);
      return h * 0x01000193;

   case TYPE_FLOAT:
   {
      unsigned long x;
      memcpy(&x, inst->decimal, sizeof(double));
      h ^= x;
      return h * 0x01000193;
   }

   case TYPE_STR:
   {
      unsigned char *s = (unsigned char *)inst->string;
      s = (unsigned char *)inst->string;
      while (*s)
      {
         h ^= *s++;
         h *= 0x01000193;
      }
      return h;
   }
   case TYPE_RANGE:
   {
      Range *range = inst->range;
      h ^= range->start;
      h ^= range->stop;
      h ^= range->step;
      return h * 0x01000193;
   }

   default:
      const char *dt = getDataType(inst->type);
      throw_error(ERROR_TYPE, "'%s' type is not unhashable", dt);
   }
   return -1;
}

static int ranges_equal(Range *a, Range *b)
{
   return (a->start == b->start && a->step == b->step && a->stop == b->stop);
}

int instanceCompare(const Instance *a, const Instance *b)
{
   if (!a || !b)
      return 0;

   if (isNumeric(a->type) && isNumeric(b->type))
      return valueToFloat(a) == valueToFloat(b);

   if (a->type != b->type)
      return 0;

   int (*cmp)(const void *, const void *) = instanceCompare;

   switch (a->type)
   {
   case TYPE_STR:
      return strcmp(a->string, b->string) == 0;

   case TYPE_LIST:
      return lists_equal(LIST_PTR(a), LIST_PTR(b), cmp);

   case TYPE_SET:
      return sets_equal(SET_PTR(a), SET_PTR(b), cmp);

   case TYPE_RANGE:
      return ranges_equals(a->range, b->range);

   case TYPE_DICT:
      return dicts_equal(DICT_PTR(a), DICT_PTR(b), cmp);

   case TYPE_FUNCTION:
   case TYPE_CLASS:
      return a->ref->object == b->ref->object;

   default:
      fprintf(stderr, "Comparing Invalid DataType\n");
      break;
   }
   return 0;
}

int dataCompare(Data *a, Data *b)
{
   if (!a || !b)
      return 0;

   if (a->type != b->type)
      return 0;

   switch (a->type)
   {
   case TYPE_INSTANCE:
      return instanceCompare(a->instance, b->instance);

   case TYPE_BUILTIN:
      return a->builtin == b->builtin;

   case TYPE_LOOKUP:
      return a->var == b->var;

   case TYPE_OPERATOR:
      return a->op == b->op;

   case TYPE_ATTRIBUTE:
      return a->attribute == b->attribute;

   case TYPE_INDEX:
      return a->index == b->index;

   case TYPE_INVOKED:
      return a->invoked == b->invoked;

   default:
      return a->any == b->any;
   }
}

int saveToEnvironment(Data *key, Data *value, Environment *env)
{
   if (!key || !value || !env)
      return;

   Data *key_clone = cloneData(key);

   // Priority 1: Check if marked as GLOBAL in current scope
   if (dict_has(key, env->global_vars))
   {
      Environment *globalEnv = env;
      while (globalEnv->parent != NULL)
         globalEnv = globalEnv->parent;

      Dict *dir = globalEnv->vars;
      return dict_insert(key_clone, value, dir);
   }

   // Priority 2: Check if marked as NONLOCAL in current scope
   if (dict_has(key, env->nonlocal_vars))
   {
      Environment *e;
      for (e = env->parent; e && e->parent; e = e->parent)
      {
         Dict *dir = e->vars;
         if (dict_has(key, dir))
            return dict_insert(key_clone, value, dir);
      }
   }

   // Priority 3: DEFAULT BEHAVIOR
   return dict_insert(key_clone, value, env->vars);
}

Data *getFromEnvironemnt(Instance *key, Environment *env)
{
   if (!key || !env)
      return NULL;

   Environment *e = env;

   for (e = env; e; e = e->parent)
      if (dict_has(key, e->vars))
      {
         void *res = dict_get(key, e->vars);
         return cloneData((Data *)res);
      }

   if (env->builtins && dict_has(key, env->builtins))
      return (Data *)dict_get(key, env->builtins);

   throw_error(ERROR_NAME,
               "name '%s' not defined", key->string);
   return NULL;
}
