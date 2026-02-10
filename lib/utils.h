#ifndef UTILS_H
#define UTILS_H

#include "./lib/type.h"

Data *cloneData(const Data *src);
int isInt(Data *value);
int isInteger(double value);
int isNumeric(DataType type);
int isHashable(DataType type);
int isInstance(Data *a, DataType type);
int isSameType(Data *a, Data *b);
double valueToFloat(Data *instance);
unsigned long hash(void *key);
int dataCompare(Data *a, Data *b);
int saveInEnvironment(Data *key, Data *value, Environment *env);
Data *getData(Data *key, Environment *env);
Environment *findEnclosingScope(Data *name, Runtime *rt);
size_t seqlen(const Data *obj);
int validateIndex(Data *index, Data *obj);
Data *getAttribute(Data *object, Data *name);
void setAttribute(Data *object, Data *attrName, Data *value);

#endif
