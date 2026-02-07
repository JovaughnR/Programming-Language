#ifndef UTILS_H
#define UTILS_H

#include "./lib/type.h"

Data *cloneData(const Data *src);
int isInt(Data *value);
int isInteger(double value);
int isNumeric(DataType type);
int isHashable(DataType type);
int isInstance(Instance *a, DataType type);
int isSameType(Instance *a, Instance *b);
double valueToFloat(Instance *instance);
unsigned long hash(void *key);
int instanceCompare(const Instance *a, const Instance *b);
int dataCompare(Data *a, Data *b);

#endif
