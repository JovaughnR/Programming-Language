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

#include "../parse/parse.h"

// Function prototypes
double floorDivision(double val1, double val2);
double divide(double val1, double val2);
double modulo(double val1, double val2);
Data *handleUnaryOperation(Data *operand, Operator op);
Data *handleBinaryOperation(Data *x, Data *y, Operator op);

static Data *evalString(Data *x, Data *y, Operator op)
{
   const char *s1 = x->str;
   const char *s2 = y->str;
   int resultBool;

   Data *res = NULL;

   switch (op)
   {
   case ADD:
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

static Data *handleExpr(double v1, double v2, Operator op)
{
   double *resPtr = malloc(sizeof(double));
   if (!resPtr)
      return createData(TYPE_NONE, NULL);

   switch (op)
   {
   case ADD:
      *resPtr = v1 + v2;
      break;
   case SUB:
      *resPtr = v1 - v2;
      break;
   case MUL:
      *resPtr = v1 * v2;
      break;
   case DIV:
      *resPtr = divide(v1, v2);
      break;
   case FLDIV:
      *resPtr = floorDivision(v1, v2);
      break;
   case MOD:
      *resPtr = modulo(v1, v2);
      break;
   case EXP:
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
   case OR:
      *resPtr = v1 || v2;
      break;
   case AND:
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
      fprintf(stderr, "Unknown operator in handleExpr()\n");
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

Data *evalList(Data *x, Data *y, Operator op)
{
   List *l1 = LIST_PTR(x);
   List *l2 = LIST_PTR(y);

   switch (op)
   {
   case ADD:
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

static Data *evalExpr(Data *x, Data *y, Operator op)
{
   double v1 = valueToFloat(x);
   double v2 = valueToFloat(y);

   // Check if either operand is a float type
   int shouldBeFloat = (x->type == TYPE_FLOAT || y->type == TYPE_FLOAT);

   Data *result = handleExpr(v1, v2, op);

   // Only preserve float type for arithmetic operations, not comparisons/logical
   int isArithmetic =
       (op == ADD || op == SUB || op == MUL ||
        op == DIV || op == MOD || op == FLDIV ||
        op == EXP);

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

static Data *evalSet(Data *x, Data *y, Operator op)
{
   Set *s1 = SET_PTR(x);
   Set *s2 = SET_PTR(y);
   Set *set = NULL;

   switch (op)
   {
   case BIT_OR:
      set = set_union(s1, s2);
      return createData(TYPE_SET, set);

   case SUB:
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

static Data *handleReplication(Data *x, Data *y)
{
   double times = valueToFloat(y);

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

   if (isNumeric(x->type) && isNumeric(y->type))
      return evalExpr(x, y, op);

   if (op == MUL)
   {
      if (isNumeric(x->type) && isSequence(y->type))
         return handleReplication(y, x);

      if (isNumeric(y->type) && isSequence(x->type))
         return handleReplication(x, y);
   }

   if (x->type == TYPE_LIST && y->type == TYPE_LIST)
      return evalList(x, y, op);

   if (x->type == TYPE_STR && y->type == TYPE_STR)
      return evalString(x, y, op);

   if (x->type == TYPE_SET && y->type == TYPE_SET)
      return evalSet(x, y, op);

   if (x->type == TYPE_NONE || y->type == TYPE_NONE)
   {
      if (op == EQUAL)
         return createData(TYPE_BOOL, &(int){x->type == y->type ? 1 : 0});
      if (op == NEQ)
         return createData(TYPE_BOOL, &(int){x->type != y->type ? 1 : 0});
   }

   if (op == EQUAL)
      return createData(TYPE_BOOL, &(int){(void *)x->ref == (void *)y->ref ? 1 : 0});
   if (op == NEQ)
      return createData(TYPE_BOOL, &(int){(void *)x->ref != (void *)y->ref ? 1 : 0});

   throw_error(
       ERROR_TYPE,
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

Data *evalListValues(Data *leaf, Runtime *rt)
{
   List *src = (List *)leaf->any;
   List *result = list_create(src->length > 0 ? src->length : 1);

   for (int i = 0; i < src->length; i++)
   {
      Data *item = executeAST((ASTnode *)src->items[i], rt);
      list_append(item, result);
   }

   return createData(TYPE_LIST, result);
}

Data *evalDictValues(Data *leaf, Runtime *rt)
{
   List *src = (List *)leaf->any;
   Dict *result = dict_create(__len__);

   for (int i = 0; i < src->length; i++)
   {
      Pair *pair = (Pair *)src->items[i];
      Data *key = executeAST((ASTnode *)pair->key, rt);
      Data *value = executeAST((ASTnode *)pair->value, rt);

      if (!dict_insert(key, value, result))
      {
         data_free(key);
         data_free(value);
         dict_free(result);
         return createData(TYPE_NONE, NULL);
      }
   }
   return createData(TYPE_DICT, result);
}

Data *evalSetValues(Data *leaf, Runtime *rt)
{
   List *src = (List *)leaf->any;
   Set *result = set_create();

   for (int i = 0; i < src->length; i++)
   {
      Data *value = executeAST(src->items[i], rt);
      if (!set_add(value, result))
      {
         data_free(value);
         set_free(result);
         return createData(TYPE_NONE, NULL);
      }
   }
   return createData(TYPE_SET, result);
}

Data *evalFStringExpr(const char *expr, Runtime *rt)
{
   int len = strlen(expr);
   char *wrapped = malloc(len + 3);
   snprintf(wrapped, len + 3, "%s;\n", expr);

   // Save parser state
   List *saved_stmts = global_statements;
   global_statements = NULL;

   // Save outer lex buffer BEFORE creating inner one
   YY_BUFFER_STATE outer_buf = get_current_buffer();

   // Scan the expression string
   YY_BUFFER_STATE buf = yy_scan_string(wrapped);
   free(wrapped);

   int parse_result = yyparse();

   // Delete inner buffer and restore outer BEFORE returning
   yy_delete_buffer(buf);
   yy_switch_to_buffer(outer_buf);

   if (parse_result != 0 || !global_statements || global_statements->length == 0)
   {
      global_statements = saved_stmts;
      return createData(TYPE_NONE, NULL);
   }

   Statement *stmt = (Statement *)global_statements->items[0];
   Data *result = NULL;

   if (stmt->type == STMT_EXPR)
   {
      result = executeAST((ASTnode *)stmt->data, rt);
   }

   list_free(global_statements, statement_free);
   global_statements = saved_stmts;

   return result ? result : createData(TYPE_NONE, NULL);
}