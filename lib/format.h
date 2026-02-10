#ifndef FORMAT_H
#define FORMAT_H

#include "type.h"

const char *getDataType(DataType type);
const char *getOperator(Operator op);
char *dataTostring(Data *d);
int dataToBool(Data *data);

List *resolveIterable(Data *iterable);
#endif