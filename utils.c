#include <stdlib.h>
#include <string.h>

#include "./lib/error.h"
#include "./lib/list.h"
#include "./lib/dict.h"
#include "./lib/type.h"
#include "./lib/set.h"

static Int cloneInt(Int src)
{
   Int copy;
   int *v = malloc(sizeof(int));
   *v = *src.atom;

   copy.atom = v;
   copy.methods = src.methods;
   return copy;
}

static Float cloneFloat(Float src)
{
   Float copy;
   double *v = malloc(sizeof(float));
   *v = *src.real;

   copy.real = v;
   copy.methods = src.methods;
   return copy;
}

Data *cloneData(const Data *src)
{
   Data *copy = malloc(sizeof(Data));
   if (!copy || !src)
      return NULL;

   copy->type = src->type;
   switch (src->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
      copy->integer = cloneInt(src->integer);
      break;

   case TYPE_FLOAT:
      copy->decimal = cloneFloat(src->decimal);
      break;

   case TYPE_STR:
      copy->str.string = strdup(src->str.string);
      copy->str.methods = src->str.methods;
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

   case TYPE_NONE:
      copy->none = src->none;
      break;

   default:
      copy->any = src->any;
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

int isInstance(Data *a, DataType type)
{
   return a->type == type;
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
      return *(double *)data->decimal.real;
   else
      return (double)*(int *)data->integer.atom;
}

unsigned long hash(void *key)
{
   unsigned long h = 0x811C9DC5;
   Data *data = (Data *)key;

   switch (data->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
      h ^= (unsigned long)(*data->integer.atom);
      return h * 0x01000193;

   case TYPE_FLOAT:
   {
      unsigned long x;
      memcpy(&x, data->decimal.real, sizeof(double));
      h ^= x;
      return h * 0x01000193;
   }

   case TYPE_STR:
   {
      unsigned char *s = (unsigned char *)data->str.string;
      s = (unsigned char *)data->str.string;
      while (*s)
      {
         h ^= *s++;
         h *= 0x01000193;
      }
      return h;
   }
   case TYPE_RANGE:
   {
      Range *range = data->range;
      h ^= range->start;
      h ^= range->stop;
      h ^= range->step;
      return h * 0x01000193;
   }

   default:
      const char *t = getDataType(data->type);
      throw_error(ERROR_TYPE, "'%s' type is not unhashable", t);
   }
   return -1;
}

static int ranges_equal(Range *a, Range *b)
{
   return (a->start == b->start && a->step == b->step && a->stop == b->stop);
}

int dataCompare(const Data *a, const Data *b)
{
   if (!a || !b)
      return 0;

   if (isNumeric(a->type) && isNumeric(b->type))
      return valueToFloat(a) == valueToFloat(b);

   if (a->type != b->type)
      return 0;

   int (*cmp)(const void *, const void *) = dataCompare;

   switch (a->type)
   {
   case TYPE_STR:
      return strcmp(a->str.string, b->str.string) == 0;

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
   case TYPE_INSTANCE:
      return a->ref->object == b->ref->object;

   case TYPE_NONE:
      return a->none.null == b->none.null;

   default:
      return a->any == b->any;
      break;
   }
   return 0;
}

int saveInEnvironment(Data *key, Data *value, Environment *env)
{
   if (!key || !value || !env)
      return;

   Data *key_clone = cloneData(key);
   Compare cmp = dataCompare;

   // Priority 1: Check if marked as GLOBAL in current scope
   if (dict_has(key, env->global_vars, cmp))
   {
      Environment *globalEnv = env;
      while (globalEnv->parent != NULL)
         globalEnv = globalEnv->parent;

      Dict *dir = globalEnv->vars;
      return dict_insert(key_clone, value, dir);
   }

   // Priority 2: Check if marked as NONLOCAL in current scope
   if (dict_has(key, env->nonlocal_vars, cmp))
   {
      Environment *e;
      for (e = env->parent; e && e->parent; e = e->parent)
      {
         Dict *dir = e->vars;
         if (dict_has(key, dir, cmp))
            return dict_insert(key_clone, value, dir);
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
   Compare cmp = dataCompare;

   for (e = env; e; e = e->parent)
      if (dict_has(key, e->vars, cmp))
      {
         void *res = dict_get(key, e->vars, cmp);
         return cloneData((Data *)res);
      }

   if (env->builtins)
      if (dict_has(key, env->builtins, cmp))
      {
         void *res = dict_get(key, env->builtins, cmp);
         return (Data *)res;
      }

   throw_error(
       ERROR_NAME,
       "name '%s' not defined", key->str.string);
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
      if (dict_has(name, e->vars, dataCompare))
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
      return strlen(object->str.string);
   default:
      break;
   }
   return (size_t *)1;
}

int validateIndex(Data *index, Data *object)
{
   if (object->type == TYPE_DICT)
      return 0;

   if (object->type != TYPE_LIST &&
       object->type != TYPE_STR)
   {
      throw_error(
          ERROR_TYPE,
          "'%s'object is not subscriptable",
          getDataType(object->type));
      return 1;
   }

   if (index->type != TYPE_BOOL &&
       index->type != TYPE_INT)
   {
      throw_error(
          ERROR_TYPE,
          "'%s' indices must be integers: not '%s'",
          getDataType(object->type),
          getDataType(index->type));
      return 1;
   }

   size_t len = seqlen(object);
   size_t idx = *(int *)index->integer.atom;

   if (idx >= len || (idx < 0 && -idx > len))
   {
      throw_error(
          ERROR_INDEX,
          "Index out of range");
      return 1;
   }

   return (int)((idx < 0) ? (len + idx) : idx);
}

static Data *get_from_mro(Class *cls, Data *name)
{
   if (!cls || !name)
      return NULL;

   Dict *dir = cls->env->vars;
   Data *attr = dict_get(name, dir, dataCompare);

   if (attr)

      return attr;

   if (!cls->mro)
      return NULL;

   for (int i = 0; i < cls->mro->length; i++)
   {
      Data *item = cls->mro->items[i];
      Class *ancestor = CLASS_PTR((Data *)item);

      dir = ancestor->env->vars;
      attr = dict_get(name, dir, dataCompare);
      if (attr)
         return attr;
   }

   return NULL;
}

Data *getAttribute(Data *object, Data *name)
{
   if (!object || !name)
      return NULL;

   Dict *dir = NULL;
   switch (object->type)
   {
   case TYPE_CLASS:
      Class *class = CLASS_PTR(object);
      Data *res = get_from_mro(class, name);
      if (res)
         return res;
      break;

   case TYPE_LIST:
   case TYPE_DICT:
   case TYPE_SET:
   case TYPE_INSTANCE:
      dir = object->ref->methods;
      break;

   case TYPE_INT:
   case TYPE_BOOL:
      dir = object->integer.methods;
      break;

   case TYPE_RANGE:
      dir = object->range->methods;
      break;

   case TYPE_STR:
      dir = object->str.methods;
      break;

   case TYPE_NONE:
      dir = object->none.methods;
      break;

   default:
      break;
   }

   void *attri = dict_get(name, dir, dataCompare);

   if (!attri)
   {
      throw_error(
          ERROR_ATTRIBUTE,
          "'%s' object has no attribute '%s'",
          getDataType(object->type),
          name->str.string);

      return NULL;
   }
   return (Data *)attri;
}

void setAttribute(Data *object, Data *attrName, Data *value)
{
   Dict *dir = NULL;

   switch (object->type)
   {
   case TYPE_CLASS:
      Class *class = CLASS_PTR(object);
      dir = class->env->vars;
      break;

   case TYPE_INSTANCE:
      dir = object->ref->methods;
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
