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
#include "./lib/format.h"
#include "./lib/exec.h"

// Function prototypes
double floorDivision(double val1, double val2);
double divide(double val1, double val2);
double modulo(double val1, double val2);
Data *handleUnaryOperation(Data *operand, Operator op);
Data *handleBinaryOperation(Data *x, Data *y, Operator op);

static Data *evalString(const char *s1, const char *s2, Operator op)
{
   int resultBool;
   Data *res = NULL;

   switch (op)
   {
   case PLUS:
      res = createData(TYPE_STR, str_concat(s1, s2));
      break;
   case EQUAL:
      resultBool = strcmp(s1, s2) == 0;
      res = createData(TYPE_BOOL, &resultBool);
      break;
   case NEQ:
      resultBool = strcmp(s1, s2) != 0;
      res = createData(TYPE_BOOL, &resultBool);
      break;
   case GRT:
      resultBool = str_lexicographical_compare(s1, s2) > 0;
      res = createData(TYPE_BOOL, &resultBool);
      break;
   case LST:
      resultBool = str_lexicographical_compare(s1, s2) < 0;
      res = createData(TYPE_BOOL, &resultBool);
      break;
   case GEQ:
      resultBool = str_lexicographical_compare(s1, s2) >= 0;
      res = createData(TYPE_BOOL, &resultBool);
      break;
   case LEQ:
      resultBool = str_lexicographical_compare(s1, s2) <= 0;
      res = createData(TYPE_BOOL, &resultBool);
      break;

   case INTO:
      resultBool = str_is_substr(s1, s2) == 1;
      res = createData(TYPE_BOOL, &resultBool);
      break;

   default:
      throw_error(ERROR_TYPE,
                  "unsupported operator '%s' for string operation",
                  getOperator(op));
      res = createData(TYPE_NONE, NULL);
   }
   return res;
}

static Data *evalExpression(double v1, double v2, Operator op)
{
   double *resPtr = malloc(sizeof(double));
   if (!resPtr)
      return createData(TYPE_NONE, NULL);

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
      return createData(TYPE_NONE, NULL);
   }

   if (isInteger(*resPtr))
   {
      int *intPtr = malloc(sizeof(int));
      *intPtr = (int)*resPtr;
      free(resPtr); // Free the double pointer
      return createData(TYPE_INT, intPtr);
   }
   return createData(TYPE_FLOAT, resPtr);
}

Data *evalList(List *l1, List *l2, Operator op)
{
   switch (op)
   {
   case PLUS:
      return createData(TYPE_LIST, list_concat(l1, l2));
   case EQUAL:
      return createData(TYPE_BOOL, &(int){list_equal(l1, l2)});
   case NEQ:
      return createData(TYPE_BOOL, &(int){list_not_equal(l1, l2)});
   case LST:
      return createData(TYPE_BOOL, &(int){list_less(l1, l2)});
   case LEQ:
      return createData(TYPE_BOOL, &(int){list_less_equal(l1, l2)});
   case GRT:
      return createData(TYPE_BOOL, &(int){list_greater(l1, l2)});
   case GEQ:
      return createData(TYPE_BOOL, &(int){list_greater_equal(l1, l2)});

   default:
      throw_error(
          ERROR_TYPE, "unsupported operator '%s' for lists\n",
          getOperator(op));
      return createData(TYPE_NONE, NULL);
   }
}

static Data *handleNumericOperation(Data *x, Data *y, Operator op)
{
   double v1 = valueToFloat(x);
   double v2 = valueToFloat(y);

   // Check if either operand is a float type
   int shouldBeFloat = (x->type == TYPE_FLOAT || y->type == TYPE_FLOAT);

   Data *result = evalExpression(v1, v2, op);

   // Only preserve float type for arithmetic operations, not comparisons/logical
   int isArithmetic =
       (op == PLUS || op == MINUS || op == MULTIPLY ||
        op == DIVIDE || op == MODULO || op == FLOOR ||
        op == EXPONENT);

   // If either operand was float and result is int and it's arithmetic, convert to float
   if (shouldBeFloat && isArithmetic && result->type == TYPE_INT)
   {
      double *floatVal = malloc(sizeof(double));
      *floatVal = (double)(*result->atom);
      free(result->atom);
      result->real = floatVal;
      result->type = TYPE_FLOAT;
   }

   return result;
}

static Data *evalSet(Set *s1, Set *s2, Operator op)
{
   Set *set = NULL;

   switch (op)
   {
   case BIT_OR:
      set = set_union(s1, s2);
      return createData(TYPE_SET, set);

   case MINUS:
      set = set_difference(s1, s2);
      return createData(TYPE_SET, set);

   case XOR:
      set = set_symdiff(s1, s2);
      return createData(TYPE_SET, set);

   case BIT_AND:
      set = set_intersection(s1, s2);
      return createData(TYPE_SET, set);

   default:
      throw_error(
          ERROR_TYPE,
          "Unsupported operator '%s' for sets\n",
          getOperator(op));

      return createData(TYPE_NONE, NULL);
   }
}

/**
 * Handles unary operators (currently only NOT).
 *
 * @param tree Pointer to the AST node containing the unary operator.
 * @param operand Pointer to the evaluated operand Data.
 * @return Pointer to resulting Data object.
 */
Data *handleUnaryOperation(Data *operand, Operator op)
{
   if (!operand)
      return NULL;

   if (op == NOT)
   {
      int boolVal = !dataToBool(operand);
      return createData(TYPE_BOOL, &boolVal);
   }

   if (op == BIT_NOT)
   {
      if (operand->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "bad operand type for unary ~: '%s'",
                     getDataType(operand->type));
         return NULL;
      }

      int value = *(int *)operand->atom;
      return createData(TYPE_INT, &(int){~value});
   }

   return NULL;
}

static Data *handleIntoOperation(Data *x, Data *y)
{
   if (!x || !y)
      return createData(TYPE_NONE, NULL);

   switch (y->type)
   {
   case TYPE_DICT:
      return createData(TYPE_BOOL, &(int){dict_has(x, DICT_PTR(y))});

   case TYPE_LIST:
      return createData(TYPE_BOOL, &(int){list_has(x, LIST_PTR(y))});

   case TYPE_SET:
      return createData(TYPE_BOOL, &(int){set_has(x, SET_PTR(y))});

   case TYPE_STR:
      return createData(TYPE_BOOL, &(int){str_is_substr(x->str, y->str)});

   default:
      throw_error(ERROR_TYPE, "type(%s) argument '%s' is not iterable",
                  getDataType(y->type), dataTostring(y));
      break;
   }

   return createData(TYPE_NONE, NULL);
}

static Data *handleReplication(Data *x, double times)
{
   if ((int)times != times)
   {
      throw_error(ERROR_TYPE, "can't multiply sequence with non-integer");
      return createData(TYPE_NONE, NULL);
   }

   switch (x->type)
   {
   case TYPE_LIST:
      return createData(TYPE_LIST, list_repeat((int)times, LIST_PTR(x)));

   case TYPE_STR:
      return createData(TYPE_STR, str_replicate(x->str, (int)times));

   default:
      return createData(TYPE_NONE, NULL);
   }
}

/**
 * Routes binary operations to the appropriate handler based on operand types.
 *
 * @param leftVal Pointer to left operand Data.
 * @param rightVal Pointer to right operand Data.
 * @param op The operator.
 * @return Pointer to resulting Data object.
 */
Data *handleBinaryOperation(Data *x, Data *y, Operator op)
{
   if (op == INTO)
      return handleIntoOperation(x, y);

   switch (x->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
   case TYPE_FLOAT:
      if (isSequence(y->type) && op == MULTIPLY)
         return handleReplication(y, valueToFloat(x));

      if (isNumeric(y->type))
         return handleNumericOperation(x, y, op);

      break;

   case TYPE_LIST:
   case TYPE_STR:
      if (isNumeric(y->type) && op == MULTIPLY)
         return handleReplication(x, valueToFloat(y));

      if (y->type == TYPE_LIST)
         return evalList(LIST_PTR(x), LIST_PTR(y), op);

      if (y->type == TYPE_STR)
         return evalString(x->str, y->str, op);

      break;

   case TYPE_SET:
      if (y->type == TYPE_SET)
         return evalSet(SET_PTR(x), SET_PTR(y), op);
      break;

   default:
      break;
   }
   throw_error(ERROR_TYPE,
               "unsupported operand type(s) for %s: '%s' and '%s'",
               getOperator(op), getDataType(x->type),
               getDataType(y->type));
   return createData(TYPE_NONE, NULL);
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

Data *evalListValues(Data *listData, Runtime *rt)
{
   if (listData->ref->isEvaluated)
      return listData;

   List *list = LIST_PTR(listData);
   List *newList = list_create(__size__);

   for (int i = 0; i < list->length; i++)
   {
      ASTnode *ast = list->items[i];
      Data *value = executeAST(ast, rt);
      list_append(value, newList);
      ast_free(ast);
   }

   free(list->items);
   free(list);

   listData->ref->object = newList;
   listData->ref->isEvaluated = 1;
   return listData;
}

Data *evalSetValues(Data *setData, Runtime *rt)
{
   if (setData->ref->isEvaluated)
      return setData;

   Set *evaluatedSet = set_create();
   List *setItems = (List *)setData->ref->object;

   for (int i = 0; i < setItems->length; i++)
   {
      ASTnode *ast = (ASTnode *)setItems->items[i];
      Data *item = executeAST(ast, rt);
      ast_free(ast);
      set_add(item, evaluatedSet);
   }

   free(setItems->items);
   free(setItems);

   setData->ref->object = evaluatedSet;
   setData->ref->isEvaluated = 1;
   return setData;
}

Data *evalDictValues(Data *dictData, Runtime *rt)
{
   if (dictData->ref->isEvaluated)
      return dictData;

   Dict *evaluatedDict = dict_create(8);
   List *dictItems = (List *)dictData->ref->object;

   for (int i = 0; i < dictItems->length; i++)
   {
      Pair *pair = (Pair *)dictItems->items[i];

      // Evaluate key and value
      Data *key = executeAST((ASTnode *)pair->key, rt);
      Data *value = executeAST((ASTnode *)pair->value, rt);

      // Insert into the new evaluated dict
      dict_insert(key, value, evaluatedDict);

      // Free the original AST nodes (they're separate allocations)
      ast_free((ASTnode *)pair->key);
      ast_free((ASTnode *)pair->value);

      // Free the pair structure
      free(pair);
   }

   free(dictItems->items);
   free(dictItems);

   dictData->ref->object = evaluatedDict;
   dictData->ref->isEvaluated = 1;
   return dictData;
}