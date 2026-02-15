#include <stdlib.h>
#include <string.h>

#include "./lib/error.h"
#include "./lib/list.h"
#include "./lib/dict.h"
#include "./lib/type.h"
#include "./lib/set.h"
#include "./lib/maloc.h"
#include "./lib/daloc.h"
#include "./lib/format.h"

// Function Prototypes
int datacmp(const void *d1, const void *d2);

void *cloneData(const void *d)
{
   const Data *src = (Data *)d;
   Data *copy = malloc(sizeof(Data));

   if (!copy || !src)
      return NULL;

   copy->type = src->type;
   switch (src->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
      copy->atom = malloc(sizeof(int));
      *copy->atom = *src->atom;
      break;

   case TYPE_FLOAT:
      copy->real = malloc(sizeof(double));
      *copy->real = *src->real;
      break;

   case TYPE_STR:
      copy->str = strdup(src->str);
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

   default:
      copy->any = src->any;
      break;
   }

   return (void *)copy;
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

int isInstance(Data *a, DataType type)
{
   return a->type == type;
}

int isSequence(DataType type)
{
   return (type == TYPE_LIST || type == TYPE_STR);
}

/**
 * @brief Test if two Data objects share the same Datatype.
 */
int isSameType(Data *a, Data *b)
{
   return a->type == b->type;
}

double valueToFloat(Data *data)
{
   if (data->type == TYPE_FLOAT)
      return *(double *)data->real;
   else
      return (double)*(int *)data->atom;
}

unsigned long hash(void *key)
{
   unsigned long h = 0x811C9DC5;
   Data *data = (Data *)key;

   if (data->type == TYPE_INT ||
       data->type == TYPE_BOOL)
   {
      unsigned long x;
      memcpy(&x, &(double){*data->atom}, sizeof(double));
      return (h ^ x) * 0x01000193;
   }

   if (data->type == TYPE_FLOAT)
   {
      unsigned long x;
      memcpy(&x, data->real, sizeof(double));
      return (h ^ x) * 0x01000193;
   }

   if (data->type == TYPE_STR)
   {
      unsigned char *s = (unsigned char *)data->str;
      s = (unsigned char *)data->str;
      while (*s)
      {
         h ^= *s++;
         h *= 0x01000193;
      }
      return h;
   }

   if (data->type == TYPE_RANGE)
   {
      Range *range = data->range;
      h ^= range->start;
      h ^= range->stop;
      h ^= range->step;
      return h * 0x01000193;
   }

   throw_error(
       ERROR_TYPE, "'%s' type is not unhashable",
       getDataType(data->type));

   return -1;
}

static int range_equal(Range *a, Range *b)
{
   return (a->start == b->start && a->step == b->step && a->stop == b->stop);
}

int datacmp(const void *d1, const void *d2)
{
   Data *a = (Data *)d1;
   Data *b = (Data *)d2;

   if (!a || !b)
      return 0;

   if (isNumeric(a->type) && isNumeric(b->type))
      return valueToFloat(a) == valueToFloat(b);

   if (a->type != b->type)
      return 0;

   switch (a->type)
   {
   case TYPE_STR:
      return strcmp(a->str, b->str) == 0;

   case TYPE_LIST:
      return list_equal(LIST_PTR(a), LIST_PTR(b));

   case TYPE_SET:
      return set_equal(SET_PTR(a), SET_PTR(b));

   case TYPE_RANGE:
      return range_equal(a->range, b->range);

   case TYPE_DICT:
      return dict_equal(DICT_PTR(a), DICT_PTR(b));

   case TYPE_FUNCTION:
   case TYPE_CLASS:
   case TYPE_INSTANCE:
      return a->ref->object == b->ref->object;

   default:
      return a->any == b->any;
      break;
   }
   return 0;
}

int saveInEnvironment(Data *key, Data *value, Environment *env)
{
   if (!key || !value || !env)
      return 0;

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
         {
            int i = dict_insert(key_clone, value, dir);
            return i;
         }
      }
   }

   // Priority 3: DEFAULT BEHAVIOR
   return dict_insert(key_clone, value, env->vars);
}

Data *getData(Data *key, Environment *env)
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

   if (env->builtins)
      if (dict_has(key, env->builtins))
      {
         void *res = dict_get(key, env->builtins);
         return (Data *)res;
      }

   throw_error(
       ERROR_NAME,
       "name '%s' not defined", key->str);
   return NULL;
}

Environment *findEnclosingScope(Data *name, Runtime *rt)
{
   Environment *env = rt->env;

   if (env->parent == NULL)
      return NULL;

   // Search parent scopes (excluding global)
   Environment *e;
   for (e = env->parent; e->parent != NULL; e = env->parent)
   {
      if (dict_has(name, e->vars))
         return e;
   }

   return NULL;
}

size_t seqlen(const Data *object)
{
   switch (object->type)
   {
   case TYPE_LIST:
      return (size_t)LIST_PTR(object)->length;
   case TYPE_STR:
      return strlen(object->str);
   default:
      break;
   }
   return (size_t)-1;
}

int validateIndex(Data *index, Data *object)
{
   if (object && object->type != TYPE_LIST &&
       object->type != TYPE_STR)
   {
      throw_error(
          ERROR_TYPE,
          "'%s'object is not subscriptable",
          getDataType(object->type));
      return -1;
   }

   if (index->type != TYPE_BOOL &&
       index->type != TYPE_INT)
   {
      throw_error(
          ERROR_TYPE,
          "'%s' indices must be integers: not '%s'",
          getDataType(object->type),
          getDataType(index->type));
      return -1;
   }

   if (!index || !object)
      return 0;

   return *(int *)index->atom;
}

static Data *get_from_mro(Class *cls, Data *name)
{
   if (!cls || !name)
      return NULL;

   Dict *dir = cls->rt->env->vars;
   Data *attr = dict_get(name, dir);

   if (attr)

      return attr;

   if (!cls->mro)
      return NULL;

   for (int i = 0; i < cls->mro->length; i++)
   {
      Data *item = cls->mro->items[i];
      Class *ancestor = CLASS_PTR((Data *)item);

      dir = ancestor->rt->env->vars;
      attr = dict_get(name, dir);
      if (attr)
         return attr;
   }

   return NULL;
}

Data *getAttribute(Data *object, Data *name, Runtime *rt)
{
   if (!object || !name)
      return NULL;

   Dict *dir = NULL;
   Data *attri = NULL;

   switch (object->type)
   {
   case TYPE_CLASS:
      attri = get_from_mro(CLASS_PTR(object), name);
      break;

   case TYPE_INSTANCE:
      dir = ((Instance *)object->ref->object)->attributes;

   case TYPE_LIST:
      dir = rt->methods->list;
      break;

   case TYPE_DICT:
      dir = rt->methods->dict;
      break;

   case TYPE_SET:
      dir = rt->methods->set;
      break;

   case TYPE_INT:
   case TYPE_BOOL:
      dir = rt->methods->atom;
      break;

   case TYPE_RANGE:
      dir = rt->methods->range;
      break;

   case TYPE_STR:
      dir = rt->methods->range;
      break;

   case TYPE_NONE:
      dir = rt->methods->none;
      break;

   default:
      break;
   }

   if (!attri)
      attri = dict_get(name, dir);

   if (!attri)
   {
      throw_error(
          ERROR_ATTRIBUTE,
          "'%s' object has no attribute '%s'",
          getDataType(object->type),
          name->str);

      return createData(TYPE_NONE, attri);
   }

   return (Data *)attri;
}

void setAttribute(Data *object, Data *attrName, Data *value)
{
   Dict *dir = NULL;

   switch (object->type)
   {
   case TYPE_CLASS:
      dir = (CLASS_PTR(object))->rt->env->vars;
      break;

   case TYPE_INSTANCE:
      dir = ((Instance *)object->ref->object)->attributes;
      break;

   default:
      throw_error(
          ERROR_TYPE,
          "cannot set '%s' attribute of immutable type '%s'",
          dataTostring(attrName),
          getDataType(object->type));
      return;
   }

   dict_insert(attrName, value, dir);
}

int compare_numeric_data(const void *a, const void *b)
{
   Data *data_a = *(Data **)a;
   Data *data_b = *(Data **)b;

   // Handle NULL cases
   if (!data_a || !data_b)
      return 0;

   // Only compare if both are numeric
   if (!isNumeric(data_a->type) || !isNumeric(data_b->type))
      return 0;

   double val_a = valueToFloat(data_a);
   double val_b = valueToFloat(data_b);

   if (val_a < val_b)
      return -1;
   if (val_a > val_b)
      return 1;
   return 0;
}