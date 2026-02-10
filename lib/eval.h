#ifndef EVAL_H
#define EVAL_H
#include "type.h"

// Function prototypes

/**
 * Handles unary operators (currently only NOT).
 *
 * @param tree Pointer to the AST node containing the unary operator.
 * @param operand Pointer to the evaluated operand Data.
 * @return Pointer to resulting Data object.
 */
Instance *handleUnaryOperation(Instance *operand, Operator op);
/**
 * Routes binary operations to the appropriate handler based on operand types.
 *
 * @param leftVal Pointer to left operand Data.
 * @param rightVal Pointer to right operand Data.
 * @param op The operator.
 * @return Pointer to resulting Data object.
 */
Instance *handleBinaryOperation(Instance *x, Instance *y, Operator op);
Instance *evalListInDepth(Instance *list_instance, Runtime *rt, ExecFn exec);
Instance *evalSetInDepth(Instance *set_instance, Runtime *rt, ExecFn exec);
Instance *evalDictInDepth(Instance *dict_instance, Runtime *rt, ExecFn exec);
#endif // EVAL_H