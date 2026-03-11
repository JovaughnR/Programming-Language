#ifndef CLASS_H
#define CLASS_H

#include "type.h"
#include "list.h"
#include "dict.h"

void defineFunction(Function *func, Runtime *rt);
void defineClass(Class *class, Runtime *rt);

Data *executeInvoked(Invoked *caller, Runtime *rt);
Data *executeFunction(Data *func_data, List *args, Runtime *rt);

Data *handle_module_call_vm(const void *info_ptr, List *args, Dict *kwargs, Runtime *rt);
Data *handle_class_call_vm(Data *callee, List *args, Dict *kwargs, Runtime *rt);
Data *call_function_kw(Data *func_data, List *args, Dict *kwargs, Runtime *rt);
#endif // CLASS_H