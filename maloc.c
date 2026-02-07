#include <stdlib.h>
#include "./lib/type.h"

extern Runtime *rt;

static Reference *reference(void *object)
{
   Reference *ref = (Reference *)malloc(sizeof(Reference));
   if (!ref)
      return NULL;

   ref->object = object;
   ref->references = 1;
   ref->isEvaluated = 0;

   return ref;
}

static void *setAttributes(DataType type, Instance *instance)
{
   switch (type)
   {
   case TYPE_LIST:
      instance->attributes = rt->listAttributes;
      break;

   case TYPE_DICT:
      instance->attributes = rt->dictAttributes;
      break;

   case TYPE_SET:
      instance->attributes = rt->setAttributes;
      break;

   case TYPE_INT:
   case TYPE_BOOL:
      instance->attributes = rt->intAttributes;
      break;

   case TYPE_FLOAT:
      instance->attributes = rt->floatAttributes;
      break;

   case TYPE_STR:
      instance->attributes = rt->strAttributes;

   default:
      instance->attributes = createDict(20);
      break;
   }
}

static Instance *createInstance(DataType type, void *value)
{
   Instance *instance = (Instance *)malloc(sizeof(Instance));
   instance->type = type;

   if (!instance)
      return NULL;

   switch (type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
   {
      int *val = malloc(sizeof(int));
      *val = *(int *)value;
      instance->integer = val;
      break;
   }
   case TYPE_FLOAT:
   {
      double *val = malloc(sizeof(double));
      *val = *(double *)value;
      instance->decimal = val;
      break;
   }

   case TYPE_STR:
      instance->string = strdup((char *)value);
      break;

   case TYPE_DICT:
   case TYPE_LIST:
   case TYPE_SET:
   case TYPE_FUNCTION:
   case TYPE_CLASS:
      instance->ref = reference((void *)value);
      break;
   case TYPE_RANGE:
      instance->range = (Range *)value;
      break;

   case TYPE_NONE:
   default:
      instance->none = NULL;
      break;
   }

   setAttributes(type, instance);
   return instance;
}

Data *createData(DataType type, void *value)
{
   Data *d = malloc(sizeof(Data));
   if (!d)
      return NULL;

   d->type = type;
   switch (type)
   {
   case TYPE_OPERATOR:
   {
      Operator *op = malloc(sizeof(Operator));
      *op = *(Operator *)value;
      d->op = op;
      break;
   }
   case TYPE_INDEX:
      d->index = (Indexed *)value;
      break;

   case TYPE_ATTRIBUTE:
      d->attribute = (Attribute *)value;
      break;

   case TYPE_LOOKUP:
      d->var = (Data *)value;
      break;

   case TYPE_INSTANCE:
      d->instance = createInstance(type, value);
   default:
      d->any = value;
      break;
   }

   return d;
}

List *createList(int size)
{
   List *list = (List *)malloc(sizeof(List));
   if (!list)
      return NULL;

   list->items = malloc(size * sizeof(void *));
   if (!list->items)
   {
      free(list);
      return NULL;
   }

   for (int i = 0; i < size; i++)
      list->items[i] = NULL;

   list->end = -1;
   list->length = 0;
   list->capacity = size;
   return list;
}