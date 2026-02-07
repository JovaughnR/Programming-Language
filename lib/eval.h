#ifndef EVAL_H
#define EVAL_H
#include "type.h"

// Function prototypes
double floorDivision(double val1, double val2);
double divide(double val1, double val2);
double modulo(double val1, double val2);
ASTnode *createASTnode(Data *value);
ASTnode *createASToperator(Operator op);
ASTnode *createASTexpr(ASTnode *root, ASTnode *left, ASTnode *right);
int matchTypesAnyOrder(Data *a, Data *b, Datatype t1, Datatype t2);
int dataComparator(Data *a, Data *b);
Data *evalString(char *s1, char *s2, Operator op);
Data *evalExpression(double v1, double v2, Operator op);
Data *evalList(List *l1, List *l2, Operator op, int (*cmp)(const void *, const void *));
Data *handleNumericOperation(Data *left, Data *right, Operator op);
Data *handleListOperation(Data *a, Data *b, Operator op);
Data *handleStringOperation(Data *a, Data *b, Operator op);
Data *evalSet(Set *s1, Set *s2, Operator op);
void evalListInDepth(Data *list, Runtime *rt, ExecFn exec);
void evalSetInDepth(Data *set, Runtime *rt, ExecFn);
void evalDictInDepth(Data *dict, Runtime *rt, ExecFn);

/**
 * Handles unary operators (currently only NOT).
 *
 * @param tree Pointer to the AST node containing the unary operator.
 * @param operand Pointer to the evaluated operand Data.
 * @return Pointer to resulting Data object.
 */
Data *handleUnaryOperation(ASTnode *tree, Data *operand);
/**
 * Routes binary operations to the appropriate handler based on operand types.
 *
 * @param leftVal Pointer to left operand Data.
 * @param rightVal Pointer to right operand Data.
 * @param op The operator.
 * @return Pointer to resulting Data object.
 */
Data *handleBinaryOperation(Data *x, Data *y, Operator op);

#endif // EVAL_H