#include <stdlib.h>
#include <string.h>
#include "./lib/dict.h"
#include "./lib/type.h"

#define __LEN__ 30

static Object *ref_create(void *object)
{
   Object *ref = (Object *)malloc(sizeof(Object));
   if (!ref)
      return NULL;

   ref->object = object;
   ref->references = 1;
   ref->isEvaluated = 0;

   return ref;
}

Data *data_create(DataType type, void *value)
{
   Data *d = malloc(sizeof(Data));
   if (!d)
      return NULL;

   d->type = type;
   switch (type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
   {
      int *val = malloc(sizeof(int));
      *val = *(int *)value;
      d->integer.atom = val;
      break;
   }
   case TYPE_FLOAT:
   {
      double *val = malloc(sizeof(double));
      *val = *(double *)value;
      d->decimal.real = val;
      break;
   }

   case TYPE_STR:
   case TYPE_LOOKUP:
      d->str.string = strdup((char *)value);
      break;

   case TYPE_OPERATOR:
   {
      Operator *op = malloc(sizeof(Operator));
      *op = *(Operator *)value;
      d->any = op;
      break;
   }
   case TYPE_INDEX:
      d->any = (Indexed *)value;
      break;

   case TYPE_ATTRIBUTE:
      d->any = (Attribute *)value;
      break;

   case TYPE_DICT:
   case TYPE_LIST:
   case TYPE_SET:
   case TYPE_FUNCTION:
   case TYPE_CLASS:
   case TYPE_INSTANCE:
      d->ref = ref_create(value);
      break;

   case TYPE_RANGE:
      d->range = (Range *)value;
      break;

   case TYPE_NONE:
      d->none.null = (None *)value;
      break;

   default:
      d->any = value;
      break;
   }

   return d;
}

static Method *methods_create(Runtime *rt)
{
   Method *method = (Method *)malloc(sizeof(Method));
   if (!method)
      return NULL;

   method->builtin = createDict(__LEN__);

   method->set = createDict(__LEN__);

   method->list = createDict(__LEN__);

   method->atom = createDict(__LEN__);

   method->real = createDict(__LEN__);

   method->dict = createDict(__LEN__);

   method->str = createDict(__LEN__);

   method->range = createDict(__LEN__);

   // TODO: whether or not to create method directory for TYPE 'none'
   return method;
}

Runtime *runtime_create(void)
{
   Runtime *rt = (Runtime *)malloc(sizeof(Runtime));
   if (!rt)
      return NULL;

   // global environment
   rt->env = env_create(NULL);
   rt->methods = methods_create(rt);
   rt->modules = NULL;

   return rt;
}

Attribute *attribute_create(ASTnode *object, char *attribute)
{
   Attribute *attr = (Attribute *)malloc(sizeof(Attribute));
   if (!attr)
      return NULL;

   attr->object = object;
   attr->attrib = data_create(TYPE_LOOKUP, attribute);
   return attr;
}
