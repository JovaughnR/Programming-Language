#ifndef UTILS_H
#define UTILS_H

#include "type.h"

void *cloneData(const void *d);
int isInt(Data *value);
int isInteger(double value);
int isNumeric(DataType type);
int isHashable(DataType type);
int isInstance(Data *a, DataType type);
int isSequence(DataType type);
int isSameType(Data *a, Data *b);
double valueToFloat(Data *instance);
unsigned long hash(void *key);
int datacmp(const void *d1, const void *d2);
int env_save(Data *key, Data *value, Env *env);
Data *getData(Data *key, Runtime *rt);
Env *findEnclosingScope(Data *name, Runtime *rt);
size_t seqlen(const Data *obj);
int validateIndex(Data *index, Data *obj);
Data *getAttribute(Data *object, Data *name, Runtime *rt);
void setAttribute(Data *object, Data *attrName, Data *value);
int compare_numeric_data(const void *a, const void *b);
int validArgsProvided(int min, int max, int len, const char *name);
#endif
