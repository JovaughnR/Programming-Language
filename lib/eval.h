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
Data *handleUnaryOperation(Data *operand, Operator op);
/**
 * Routes binary operations to the appropriate handler based on operand types.
 *
 * @param leftVal Pointer to left operand Data.
 * @param rightVal Pointer to right operand Data.
 * @param op The operator.
 * @return Pointer to resulting Data object.
 */
Data *handleBinaryOperation(Data *x, Data *y, Operator op);
Data *evalListValues(Data *list_Data, Runtime *rt);
Data *evalSetValues(Data *set_Data, Runtime *rt);
Data *evalDictValues(Data *dict_Data, Runtime *rt);

#endif // EVAL_H