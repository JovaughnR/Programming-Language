#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "./lib/type.h"
#include "./lib/list.h"
#include "./lib/set.h"
#include "./lib/dict.h"
#include "./lib/str.h"
#include "./lib/maloc.h"
#include "./lib/daloc.h"
#include "./lib/error.h"
#include "./lib/utils.h"
#include "./lib/eval.h"

// Function prototypes
double floorDivision(double val1, double val2);
double divide(double val1, double val2);
double modulo(double val1, double val2);
Instance *handleUnaryOperation(ASTnode *tree, Instance *operand);
Instance *handleBinaryOperation(Instance *x, Instance *y, Operator op);

int matchTypesAnyOrder(Instance *a, Instance *b, DataType t1, DataType t2)
{
   if (!a || !b)
      return 0;
   return (a->type == t1 && b->type == t2) ||
          (a->type == t2 && b->type == t1);
}

/**
 * Generic comparator for Instance objects.
 *
 * @param a Pointer to first Instance object.
 * @param b Pointer to second Instance object.
 * @return <0 if a < b, 0 if a == b, >0 if a > b
 */

/**
 * Evaluates a string operation.
 *
 * @param s1 First string.
 * @param s2 Second string.
 * @param op Operator to apply.
 * @return Pointer to resulting Instance object.
 */
Instance *evalString(char *s1, char *s2, Operator op)
{
   int resultBool;
   Instance *res = NULL;

   switch (op)
   {
   case PLUS:
      res = createInstance(TYPE_STR, concat(s1, s2));
      break;
   case EQUAL:
      resultBool = isSameString(s1, s2);
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case NEQ:
      resultBool = !isSameString(s1, s2);
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case GRT:
      resultBool = lexicographicalCompare(s1, s2) > 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case LST:
      resultBool = lexicographicalCompare(s1, s2) < 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case GEQ:
      resultBool = lexicographicalCompare(s1, s2) >= 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case LEQ:
      resultBool = lexicographicalCompare(s1, s2) <= 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case INTO:
      resultBool = isSubstring(s1, s2);
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   default:
      fprintf(stderr, "Unsupported string operator.\n");
      res = createInstance(TYPE_NONE, NULL);
   }
   return res;
}

/**
 * Evaluates a numeric expression between two doubles using the given operator.
 *
 * @param v1 First operand.
 * @param v2 Second operand.
 * @param op Operator to apply.
 * @return Pointer to resulting Instance object.
 */
Instance *evalExpression(double v1, double v2, Operator op)
{
   double *resPtr = malloc(sizeof(double));
   if (!resPtr)
      return createInstance(TYPE_NONE, NULL);

   switch (op)
   {
   case PLUS:
      *resPtr = v1 + v2;
      break;
   case MINUS:
      *resPtr = v1 - v2;
      break;
   case MULTIPLY:
      *resPtr = v1 * v2;
      break;
   case DIVIDE:
      *resPtr = divide(v1, v2);
      break;
   case FLOOR:
      *resPtr = floorDivision(v1, v2);
      break;
   case MODULO:
      *resPtr = modulo(v1, v2);
      break;
   case EXPONENT:
      *resPtr = pow(v1, v2);
      break;
   case XOR:
      *resPtr = (int)v1 ^ (int)v2;
      break;
   case BIT_OR:
      *resPtr = (int)v1 | (int)v2;
      break;
   case BIT_AND:
      *resPtr = (int)v1 & (int)v2;
      break;
   case LOGICAL_OR:
      *resPtr = v1 || v2;
      break;
   case LOGICAL_AND:
      *resPtr = v1 && v2;
      break;
   case EQUAL:
      *resPtr = (v1 == v2);
      break;
   case NEQ:
      *resPtr = (v1 != v2);
      break;
   case LST:
      *resPtr = (v1 < v2);
      break;
   case GRT:
      *resPtr = (v1 > v2);
      break;
   case LEQ:
      *resPtr = (v1 <= v2);
      break;
   case GEQ:
      *resPtr = (v1 >= v2);
      break;
   case RIGHT_SHIFT:
      *resPtr = (int)v1 >> (int)v2;
      break;
   case LEFT_SHIFT:
      *resPtr = (int)v1 << (int)v2;
      break;
   default:
      fprintf(stderr, "Unknown operator in evalExpression()\n");
      free(resPtr);
      return createInstance(TYPE_NONE, NULL);
   }

   if (isInteger(*resPtr))
   {
      int *intPtr = malloc(sizeof(int));
      *intPtr = (int)*resPtr;
      free(resPtr); // Free the double pointer
      return createInstance(TYPE_INT, intPtr);
   }
   return createInstance(TYPE_FLOAT, resPtr);
}

/**
 * Evaluates a list operation based on the operator.
 *
 * @param l1 Pointer to the first list (List).
 * @param l2 Pointer to the second list.
 * @param op Operator to apply.
 * @param cmp Comparator function for list elements.
 * @return Pointer to resulting Instance object.
 */
Instance *evalList(List *l1, List *l2, Operator op, int (*cmp)(const void *, const void *))
{
   switch (op)
   {
   case PLUS:
      return createInstance(TYPE_LIST, list_concat(l1, l2));
   case EQUAL:
      return createInstance(TYPE_BOOL, &(int){lists_equal(l1, l2, cmp)});
   case NEQ:
      return createInstance(TYPE_BOOL, &(int){list_not_equal(l1, l2, cmp)});
   case LST:
      return createInstance(TYPE_BOOL, &(int){list_less(l1, l2, cmp)});
   case LEQ:
      return createInstance(TYPE_BOOL, &(int){list_less_equal(l1, l2, cmp)});
   case GRT:
      return createInstance(TYPE_BOOL, &(int){list_greater(l1, l2, cmp)});
   case GEQ:
      return createInstance(TYPE_BOOL, &(int){list_greater_equal(l1, l2, cmp)});
   default:
      fprintf(stderr, "Error: Unsupported operator for lists\n");
      return createInstance(TYPE_NONE, NULL);
   }
}

Instance *handleNumericOperation(Instance *x, Instance *y, Operator op)
{
   double v1 = valueToFloat(x);
   double v2 = valueToFloat(y);

   // Check if either operand is a float type
   int shouldBeFloat = (x->type == TYPE_FLOAT || y->type == TYPE_FLOAT);

   Instance *result = evalExpression(v1, v2, op);

   // Only preserve float type for arithmetic operations, not comparisons/logical
   int isArithmetic =
       (op == PLUS || op == MINUS || op == MULTIPLY ||
        op == DIVIDE || op == MODULO || op == FLOOR ||
        op == EXPONENT);

   // If either operand was float and result is int and it's arithmetic, convert to float
   if (shouldBeFloat && isArithmetic && result->type == TYPE_INT)
   {
      double *floatVal = malloc(sizeof(double));
      *floatVal = (double)(*result->integer);
      free(result->integer);
      result->decimal = floatVal;
      result->type = TYPE_FLOAT;
   }

   return result;
}
/**
 * Handles list operations: concatenation, repetition, comparisons.
 *
 * @param left Pointer to the left operand Instance.
 * @param right Pointer to the right operand Instance.
 * @param op Operator.
 * @return New Instance object containing the result.
 */
Instance *handleListOperation(Instance *a, Instance *b, Operator op)
{
   // Determine which operand is list and which is int (for repetition)
   if (isSameType(a, b))
      return evalList((List *)a->ref->object, (List *)b->ref->object, op, instanceCompare);

   if (op == INTO)
      return createInstance(TYPE_BOOL, &(int){list_contains(a, list_ptr(b))});

   List *list = a->type == TYPE_LIST ? (List *)a->ref->object : (List *)b->ref->object;
   double times = a->type == TYPE_LIST ? valueToFloat(b) : valueToFloat(a);
   List *replicated = list_repeat(list, (int)times);

   return createInstance(TYPE_LIST, replicated);
}

/**
 * Handles string operations: concatenation, repetition, and comparisons.
 *
 * @param left Pointer to the left operand Instance.
 * @param right Pointer to the right operand Instance.
 * @param op Operator.
 * @return New Instance object containing the result.
 */
Instance *handleStringOperation(Instance *a, Instance *b, Operator op)
{
   if (matchTypesAnyOrder(a, b, TYPE_STR, TYPE_STR))
      return evalString(a->string, b->string, op);

   // String repetition
   char *s = a->type == TYPE_STR ? a->string : b->string;
   double times = a->type == TYPE_STR ? valueToFloat(b) : valueToFloat(a);
   char *replicated = replicateString(s, (int)times);

   return createInstance(TYPE_STR, replicated);
}

Instance *evalSet(Set *s1, Set *s2, Operator op)
{
   switch (op)
   {
   case BIT_AND:
      return createInstance(TYPE_SET, set_intersection(s1, s2));
   case MINUS:
      return createInstance(TYPE_SET, set_difference(s1, s2));
   case XOR:
      return createInstance(TYPE_SET, set_symdiff(s1, s2));
   case BIT_OR:
      return createInstance(TYPE_SET, set_union(s1, s2));
   default:
      fprintf(stderr, "Error: Unsupported operator for sets\n");
      return createInstance(TYPE_NONE, NULL);
   }
}

/**
 * Handles unary operators (currently only NOT).
 *
 * @param tree Pointer to the AST node containing the unary operator.
 * @param operand Pointer to the evaluated operand Instance.
 * @return Pointer to resulting Instance object.
 */
Instance *handleUnaryOperation(ASTnode *tree, Instance *operand)
{
   if (!tree->data || tree->data->type != TYPE_OPERATOR)
   {
      fprintf(stderr, "Error: Invalid unary operator\n");
      return createInstance(TYPE_NONE, NULL);
   }

   Operator op = *(Operator *)tree->data->op; // Fixed: dereference the operator pointer

   if (op == NOT)
   {
      int boolVal = !InstanceToBool(operand);
      return createInstance(TYPE_BOOL, &boolVal);
   }

   fprintf(stderr, "Error: Unsupported unary operator\n");
   return createInstance(TYPE_NONE, NULL);
}

/**
 * Routes binary operations to the appropriate handler based on operand types.
 *
 * @param leftVal Pointer to left operand Instance.
 * @param rightVal Pointer to right operand Instance.
 * @param op The operator.
 * @return Pointer to resulting Instance object.
 */
Instance *handleBinaryOperation(Instance *x, Instance *y, Operator op)
{
   Instance *result = NULL;

   // TODO into operation for other Instance types
   if (op == INTO)
   {
      if (y->type == TYPE_DICT)
         return createInstance(TYPE_BOOL, &(int){dict_has(x, dict_ptr(y))});

      if (y->type == TYPE_SET)
         return createInstance(TYPE_BOOL, &(int){set_has(x, set_ptr(y))});
   }

   if ((isSameType(x, y) && isInstance(x, TYPE_STR)) ||
       matchTypesAnyOrder(x, y, TYPE_STR, TYPE_INT) ||
       matchTypesAnyOrder(x, y, TYPE_STR, TYPE_BOOL))
   {
      result = handleStringOperation(x, y, op);
   }

   // Numeric operations
   else if (isNumeric(x->type) && isNumeric(y->type))
      result = handleNumericOperation(x, y, op);

   // List operations
   else if ((isSameType(x, y) && isInstance(x, TYPE_LIST)) ||
            matchTypesAnyOrder(x, y, TYPE_LIST, TYPE_INT) ||
            matchTypesAnyOrder(x, y, TYPE_LIST, TYPE_BOOL))
   {
      result = handleListOperation(x, y, op);
   }

   // Set operations
   else if (isSameType(x, y) && isInstance(x, TYPE_SET))
      result = evalSet(set_ptr(x), set_ptr(y), op);

   else
   {
      fprintf(stderr, "Error: unsupported type combination (left: %d, right: %d)\n",
              x->type, y->type);
      result = createInstance(TYPE_NONE, NULL);
   }
   return result;
}

/**
 * Safe modulo operation.
 */
double modulo(double val1, double val2)
{
   if (val2 == 0.0)
   {
      throw_error(ERROR_VALUE, "modulo by zero");
      return 0.0;
   }
   return val1 - val2 * floor(val1 / val2);
}

/**
 * Safe division operation.
 */
double divide(double val1, double val2)
{
   if (val2 == 0.0)
   {
      throw_error(ERROR_ZERO_DIVISION, "Division by zero");
      return 0.0;
   }
   return val1 / val2;
}

/**
 * Safe floor division operation.
 */
double floorDivision(double val1, double val2)
{
   if (val2 == 0.0)
   {
      throw_error(ERROR_ZERO_DIVISION, "Division by zero");
      return 0.0;
   }
   return floor(val1 / val2);
}

void evalListInDepth(Instance *listObj, Runtime *rt, ExecFn exec)
{
   if (listObj->ref->isEvaluated)
      return;

   List *list = list_ptr(listObj);
   List *newList = createList(RUNTIME_SIZE);

   for (int i = 0; i < list->length; i++)
   {
      ASTnode *ast = list->items[i];
      Instance *value = exec(ast, rt);
      append(value, newList);
      freeAST(ast);
   }

   free(list->items);
   free(list);

   listObj->ref->object = newList;
   listObj->ref->isEvaluated = 1;
}

void evalSetInDepth(Instance *set, Runtime *rt, ExecFn exec)
{
   if (set->ref->isEvaluated)
      return;

   Set *evaluatedSet = createSet();
   List *setItems = (List *)set->ref->object;

   for (int i = 0; i < setItems->length; i++)
   {
      ASTnode *ast = (ASTnode *)setItems->items[i];
      Instance *item = exec(ast, rt);
      freeAST(ast);
      set_add(item, evaluatedSet);
   }

   free(setItems->items);
   free(setItems);
   set->ref->object = evaluatedSet;
   set->ref->isEvaluated = 1;
}

void evalDictInDepth(Instance *dict, Runtime *rt, ExecFn exec)
{
   if (dict->ref->isEvaluated)
      return;

   Dict *evaluatedDict = createDict(256);
   List *dictItems = (List *)dict->ref->object;

   for (int i = 0; i < dictItems->length; i++)
   {
      Pair *pair = (Pair *)dictItems->items[i]; // Access by index, don't pop

      // Evaluate key and value
      Instance *evaluatedKey = exec((ASTnode *)pair->key, rt);
      Instance *evaluatedValue = exec((ASTnode *)pair->value, rt);

      // Insert into the new evaluated dict
      insert(evaluatedKey, evaluatedValue, evaluatedDict);

      // Free the original AST nodes (they're separate allocations)
      freeAST((ASTnode *)pair->key);
      freeAST((ASTnode *)pair->value);

      // Free the pair structure
      free(pair);
   }

   free(dictItems->items);
   free(dictItems);

   dict->ref->object = evaluatedDict;
   dict->ref->isEvaluated = 1;
}
