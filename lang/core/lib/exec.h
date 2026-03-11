#ifndef EXEC_H
#define EXEC_H

#include "type.h"
#include "dict.h"

// Function prototypes
Status executeStatement(Statement *stmt, void **returns, Runtime *rt);
Status executeBody(struct List *body, void **returns, Runtime *rt);
Data *executeAST(ASTnode *tree, Runtime *rt);

// VM bridge functions
Data *getFromIndex_vm(Data *obj, Data *idx, Runtime *rt);
Data *setIndex_vm(Data *obj, Data *idx, Data *val, Runtime *rt);
Data *setAttribute_vm(Data *obj, Data *name, Data *val, Runtime *rt);
Data *executeImport_vm(const char *name, Runtime *rt);
Data *evalFString_vm(Data *fmt, Runtime *rt);
Data *handle_module_call_vm(const void *info, List *args, Dict *kwargs, Runtime *rt);
Data *handle_class_call_vm(Data *callee, List *args, Dict *kwargs, Runtime *rt);
Data *call_function_kw(Data *func, List *args, Dict *kwargs, Runtime *rt);
#endif // EXEC_H
