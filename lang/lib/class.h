#ifndef CLASS_H
#define CLASS_H

#include "type.h"

void defineFunction(Function *func, Runtime *rt);
void defineClass(Class *class, Runtime *rt);

Data *executeInvoked(Invoked *caller, Runtime *rt);
#endif // CLASS_H