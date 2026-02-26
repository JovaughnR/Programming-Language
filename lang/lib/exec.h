#ifndef EXEC_H
#define EXEC_H

#include "type.h"

// Function prototypes
Status executeStatement(Statement *stmt, void **returns, Runtime *rt);
Status executeBody(struct List *body, void **returns, Runtime *rt);
Data *executeAST(ASTnode *tree, Runtime *rt);
#endif // EXEC_H
