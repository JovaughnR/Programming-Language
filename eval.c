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
Instance *handleUnaryOperation(Instance *operand, Operator op);
Instance *handleBinaryOperation(Instance *x, Instance *y, Operator op);

static Instance *evalString(char *s1, char *s2, Operator op)
{
   int resultBool;
   Instance *res = NULL;

   switch (op)
   {
   case PLUS:
      res = createInstance(TYPE_STR, str_concat(s1, s2));
      break;
   case EQUAL:
      resultBool = strcmp(s1, s2) == 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case NEQ:
      resultBool = strcmp(s1, s2) != 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case GRT:
      resultBool = str_lexicographical_compare(s1, s2) > 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case LST:
      resultBool = str_lexicographical_compare(s1, s2) < 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case GEQ:
      resultBool = str_lexicographical_compare(s1, s2) >= 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   case LEQ:
      resultBool = str_lexicographical_compare(s1, s2) <= 0;
      res = createInstance(TYPE_BOOL, &resultBool);
      break;
   default:
      fprintf(stderr, "Unsupported string operator.\n");
      res = createInstance(TYPE_NONE, NULL);
   }
   return res;
}

static Instance *evalExpression(double v1, double v2, Operator op)
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

static Instance *evalList(List *l1, List *l2, Operator op, int (*cmp)(const void *, const void *))
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

static Instance *handleNumericOperation(Instance *x, Instance *y, Operator op)
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

static Instance *evalSet(Set *s1, Set *s2, Operator op)
{
   Compare cmp = instanceCompare;
   Clone cln = cloneInstance;

   switch (op)
   {
   case BIT_OR:
      return createInstance(TYPE_SET, set_union(s1, s2, cmp, cln));
   case MINUS:
      return createInstance(TYPE_SET, set_difference(s1, s2, cmp, cln));
   case XOR:
      return createInstance(TYPE_SET, set_symdiff(s1, s2, cmp, cln));
   case BIT_AND:
      return createInstance(TYPE_SET, set_intersection(s1, s2, cmp, cln));
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
Instance *handleUnaryOperation(Instance *operand, Operator op)
{
   if (!operand)
      return NULL;

   if (op == NOT)
   {
      int boolVal = !InstanceToBool(operand);
      return createInstance(TYPE_BOOL, &boolVal);
   }

   if (op == BIT_NOT)
   {
      if (operand->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "bad operand type for unary ~: '%s'",
                     getDataType(operand->type));
         return NULL;
      }

      int value = *(int *)operand->integer;
      return createInstance(TYPE_INT, &(int){~value});
   }

   return NULL;
}

static Instance *handleIntoOperation(Instance *x, Instance *y)
{
   if (!x || !y)
      return createInstance(TYPE_NONE, NULL);

   if (x->type == TYPE_STR && y->type == TYPE_STR)
      return createInstance(TYPE_BOOL, str_is_substr(x->string, y->string));

   Compare cmp = instanceCompare;

   switch (y->type)
   {
   case TYPE_DICT:
      return createInstance(TYPE_BOOL, &(int){dict_has(x, DICT_PTR(y), cmp)});

   case TYPE_LIST:
      return createInstance(TYPE_BOOL, &(int){list_has(x, LIST_PTR(y), cmp)});

   case TYPE_SET:
      return createInstance(TYPE_BOOL, &(int){set_has(x, SET_PTR(y), cmp)});
   default:
      break;
   }

   return createInstance(TYPE_NONE, NULL);
}

static Instance *handleReplication(Instance *x, Instance *y)
{
   Instance *inst = isNumeric(x->type) ? y : x;
   double times = isNumeric(x->type) ? valueToFloat(x) : valueToFloat(y);

   if ((int)times != times)
      throw_error(ERROR_TYPE, "can't multiply sequence with non-integer");

   switch (inst->type)
   {
   case TYPE_LIST:
      return createInstance(TYPE_LIST, list_repeat((int)times, LIST_PTR(inst)));

   case TYPE_STR:
      return createInstance(TYPE_STR, str_replicate((int)times, inst->string));

   default:
      return createInstance(TYPE_NONE, NULL);
   }
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
   switch (x->type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
   case TYPE_FLOAT:
      if ((y->type == TYPE_LIST || y->type == TYPE_STR) && op == MULTIPLY)
         return handleReplication(x, y);
      if (!isNumeric(y->type))
         break;
      else
         return handleNumericOperation(x, y, op);

   case TYPE_LIST:
      if (isNumeric(y->type) && op == MULTIPLY)
         return handleReplication(x, y);
      if (y->type != TYPE_LIST)
         break;
      else
         return evalList(LIST_PTR(x), LIST_PTR(y), op, instanceCompare);

   case TYPE_SET:
      if (y->type != TYPE_SET)
         break;
      return evalSet(SET_PTR(x), SET_PTR(y), op);

   case TYPE_STR:
      if (isNumeric(y->type) && op == MULTIPLY)
         return handleReplication(x, y);
      if (y->type != TYPE_STR)
         break;
      return evalString(x->string, y->string, op);

   default:
      break;
   }
   throw_error(ERROR_TYPE,
               "unsupported operand type(s) for %s: '%s' and '%s'",
               getOperator(op), getDataType(x->type),
               getDataType(y->type));
   return createInstance(TYPE_NONE, NULL);
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

Instance *evalListInDepth(Instance *list_instance, Runtime *rt, ExecFn exec)
{
   if (list_instance->ref->isEvaluated)
      return list_instance;

   List *list = LIST_PTR(list_instance);
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

   list_instance->ref->object = newList;
   list_instance->ref->isEvaluated = 1;
   return list_instance;
}

Instance *evalSetInDepth(Instance *set_instance, Runtime *rt, ExecFn exec)
{
   if (set_instance->ref->isEvaluated)
      return;

   Set *evaluatedSet = createSet();
   List *setItems = (List *)set_instance->ref->object;

   for (int i = 0; i < setItems->length; i++)
   {
      ASTnode *ast = (ASTnode *)setItems->items[i];
      Instance *item = exec(ast, rt);
      freeAST(ast);
      set_add(item, evaluatedSet, instanceCompare);
   }

   free(setItems->items);
   free(setItems);

   set_instance->ref->object = evaluatedSet;
   set_instance->ref->isEvaluated = 1;
   return set_instance;
}

Instance *evalDictInDepth(Instance *dict_instance, Runtime *rt, ExecFn exec)
{
   if (dict_instance->ref->isEvaluated)
      return;

   Dict *evaluatedDict = createDict(256);
   List *dictItems = (List *)dict_instance->ref->object;

   for (int i = 0; i < dictItems->length; i++)
   {
      Pair *pair = (Pair *)dictItems->items[i];

      // Evaluate key and value
      Instance *key = exec((ASTnode *)pair->key, rt);
      Instance *value = exec((ASTnode *)pair->value, rt);

      // Insert into the new evaluated dict
      dict_insert(key, value, evaluatedDict);

      // Free the original AST nodes (they're separate allocations)
      freeAST((ASTnode *)pair->key);
      freeAST((ASTnode *)pair->value);

      // Free the pair structure
      free(pair);
   }

   free(dictItems->items);
   free(dictItems);

   dict_instance->ref->object = evaluatedDict;
   dict_instance->ref->isEvaluated = 1;
   return dict_instance;
}