#ifndef DALLOC_H
#define DALLOC_H

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

/* Function prototypes */
void data_free(void *d);
// void freeList(List *list, ListType type);
// void freeDict(Dict *dict);
// void freeParamInfo(ParamInfo *info);
// void freeFunction(Function *func);
// void freeAST(Ast *ast);
// void freeWhileLoop(WhileLoop *loop);
// void freeForLoop(ForLoop *loop);
// void freeRange(Range *r);
// void freeSet(Set *set);
// void freeStatement(Statement *stmt);
// void freeWhileLoop(WhileLoop *loop);
// void freeFlow(Flow *stmt);
// void freeInvoked(Invoked *func);
// void freeIndexed(DataIndex *index);
// void freeReference(Object *rc);
// void env_free(Env *env);
// void freeRuntime(Runtime *rt);
// void freeVarDecl(VarDecl *decl);
#endif // DALLOC_H