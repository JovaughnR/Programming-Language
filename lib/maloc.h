#ifndef ALLOC_H
#define ALLOC_H
#include "type.h"

typedef enum
{
   TYPE_DATA,
   TYPE_PARAMINFO,
   TYPE_STATEMENT,
   TYPE_NODATA,
   TYPE_ENV,
   TYPE_ASTs
} ListType;

Instance *createInstance(DataType type, void *value);
Data *createData(HelperType type, void *value);
#endif
