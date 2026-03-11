
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

#include "./lib/type.h"
#include "./lib/dict.h"
#include "./lib/list.h"
#include "./lib/utils.h"
#include "./lib/str.h"
#include "./lib/set.h"
#include "./lib/error.h"
#include "./lib/eval.h"
#include "./lib/maloc.h"
#include "./lib/module.h"
#include "./lib/class.h"
#include "./lib/daloc.h"
#include "./lib/format.h"

#include "../builtin/lib/async.h"

extern Data *builtin_print(List *args, Dict *kwargs);

// Function prototypes
Data *executeAST(ASTnode *node, Runtime *rt);
Status executeBody(List *body, void **returns, Runtime *rt);

/**
 * Marks a variable as global in the current scope
 */
static void global_exec(char *var, Runtime *rt)
{
   if (!var || !rt)
      return;

   Env *globalEnv = rt->env;

   while (globalEnv->parent)
      globalEnv = globalEnv->parent;

   Env *currentEnv = rt->env;

   // If we're already in global scope, nothing to do
   if (currentEnv == globalEnv)
      return;

   Data *key = createData(TYPE_STR, var);

   // Check if variable is already declared in local scope
   if (dict_has(key, currentEnv->vars))
   {
      throw_error(
          ERROR_SYNTAX,
          "name '%s' is assigned to before global declaration",
          var);
      return;
   }

   // Mark this variable as global in current scope
   Data *marker = createData(TYPE_INT, &(int){1});

   dict_insert(key, marker, currentEnv->global_vars);

   // Initialize in global scope if it doesn't exist
   if (!dict_has(key, globalEnv->vars))
   {
      Data *none = createData(TYPE_NONE, NULL);
      env_save(key, none, globalEnv);
   }
}

static void nonlocal_exec(char *var, Runtime *rt)
{
   if (!var || !rt)
      return;

   Env *currentEnv = rt->env;

   if (currentEnv->parent == NULL)
   {
      throw_error(ERROR_SYNTAX, "nonlocal declaration not allowed at module level");
      return;
   }

   Data *key = createData(TYPE_STR, var); // createData strdups var internally

   if (dict_has(key, currentEnv->vars))
   {
      throw_error(ERROR_SYNTAX, "name '%s' is assigned to before nonlocal declaration", key->str);
      data_free(key);
      return;
   }

   Env *enclosingScope = findEnclosingScope(key, rt);

   if (!enclosingScope)
   {
      throw_error(ERROR_SYNTAX, "no binding for nonlocal '%s' found", key->str);
      data_free(key);
      return;
   }

   Data *marker = createData(TYPE_INT, &(int){1});
   dict_insert(key, marker, currentEnv->nonlocal_vars);
}

static Status executeFlow(Flow *stmt, Runtime *rt, void **returns)
{
   if (!stmt)
      return FLOW_NORMAL;

   if (!stmt->condition)
   {
      if (stmt->body)
      {
         Status status = executeBody(stmt->body, returns, rt);
         return (status == FLOW_NORMAL) ? FLOW_TAKEN : status;
      }
      return FLOW_NORMAL;
   }

   // Evaluate the condition
   Data *cond = executeAST(stmt->condition, rt);
   int condValue = dataToBool(cond);

   data_free(cond);

   if (condValue)
   {
      Status status = executeBody(stmt->body, returns, rt);
      return (status == FLOW_NORMAL) ? FLOW_TAKEN : status;
   }

   // Condition is false, try elif chain
   if (stmt->elif_chain)
   {
      Status status = executeFlow(stmt->elif_chain, rt, returns);
      if (status != FLOW_NORMAL)
         return status; // Propagate FLOW_TAKEN, FLOW_RETURN, etc.
   }

   // Only execute else if no elif was taken (status == FLOW_NORMAL)
   if (stmt->else_block)
      return executeFlow(stmt->else_block, rt, returns);

   return FLOW_NORMAL;
}

static Status executeWhile(WhileLoop *loop, Runtime *rt, void **returns)
{
   if (!loop)
      return FLOW_NORMAL;

   while (1)
   {
      Data *cond = executeAST(loop->condition, rt);

      if (!cond)
         break;

      int condValue = 0;
      if (cond->type == TYPE_BOOL || cond->type == TYPE_INT)
         condValue = *(int *)cond->atom;

      data_free(cond);
      if (condValue == 0)
         break;

      Status status = executeBody(loop->body, returns, rt);

      if (status == FLOW_RETURN)
         return status;

      if (status == FLOW_BREAK)
         break;

      if (status == FLOW_CONTINUE)
         continue;
   }

   return FLOW_NORMAL;
}

/**
 * @brief Binds the loop iterator to the current element.
 */
static void bindIteratorValue(Data *iterator, Data *iter, Runtime *rt)
{
   if (!iterator || !iter || !rt)
      return;

   env_save(cloneData(iterator), cloneData(iter), rt->env);
}

// Helper function to handle loop body execution and flow control
static Status executeLoopIteration(ForLoop *loop, Data *item, void **returns, Runtime *rt, int *brk)
{
   bindIteratorValue(loop->iterator, item, rt);
   Status status = executeBody(loop->body, returns, rt);

   switch (status)
   {
   case FLOW_BREAK:
      *brk = 1;
      return FLOW_NORMAL;

   case FLOW_RETURN:
      return FLOW_RETURN;

   case FLOW_CONTINUE:
   case FLOW_NORMAL:
      return FLOW_NORMAL;

   default:
      return status;
   }
}

// Iterate over a list
static Status iterateList(ForLoop *loop, List *list, void **returns, Runtime *rt)
{
   int shouldBreak = 0;

   for (int i = 0; i < list->length && !shouldBreak; i++)
   {
      Data *item = (Data *)list->items[i];
      Status status = executeLoopIteration(loop, item, returns, rt, &shouldBreak);
      if (status == FLOW_RETURN)
         return status;
   }

   return FLOW_NORMAL;
}

// Iterate over a set
static Status iterateSet(ForLoop *loop, Set *set, void **returns, Runtime *rt)
{
   int shouldBreak = 0;
   for (int i = 0; i < set->size && !shouldBreak; i++)
   {
      Data *item = (Data *)set->bucket[i];
      if (item == NULL || item == TOMBSTONE)
         continue;

      Status status = executeLoopIteration(loop, item, returns, rt, &shouldBreak);
      if (status == FLOW_RETURN)
         return status;
   }
   return FLOW_NORMAL;
}

// Iterate over a range
static Status iterateRange(ForLoop *loop, Range *r, void **returns, Runtime *rt)
{
   int shouldBreak = 0;

   for (int i = r->start; i < r->stop; i += r->step)
   {
      if (shouldBreak)
         break;

      Data *item = createData(TYPE_INT, &i);
      Status status = executeLoopIteration(loop, item, returns, rt, &shouldBreak);

      if (status == FLOW_RETURN)
         return status;
   }

   return FLOW_NORMAL;
}

// Iterate over a string
static Status iterateString(ForLoop *loop, char *str, void **returns, Runtime *rt)
{
   int shouldBreak = 0;
   int len = strlen(str);

   for (int i = 0; i < len && !shouldBreak; i++)
   {
      Data *item = createData(TYPE_STR, str_char_at(str, i));
      Status status = executeLoopIteration(loop, item, returns, rt, &shouldBreak);
      data_free(item);

      if (status == FLOW_RETURN)
         return status;
   }

   return FLOW_NORMAL;
}

// Main for loop execution function
static Status executeFor(ForLoop *loop, Runtime *rt, void **returns)
{
   if (!loop)
      return FLOW_NORMAL;

   // Evaluate the iterable expression
   Data *iterable = executeAST(loop->iterable, rt);
   if (!iterable)
      return FLOW_NORMAL;

   switch (iterable->type)
   {
   case TYPE_LIST:
      return iterateList(loop, LIST_PTR(iterable), returns, rt);

   case TYPE_SET:
      return iterateSet(loop, SET_PTR(iterable), returns, rt);

   case TYPE_RANGE:
      return iterateRange(loop, iterable->range, returns, rt);

   case TYPE_STR:
      return iterateString(loop, iterable->str, returns, rt);

   default:
   {
      const char *typeName = getDataType(iterable->type);
      throw_error(ERROR_TYPE, "'%s' object is not iterable", typeName);
   }
   }
   return FLOW_NORMAL;
}

// For List and Strings
static Data *getFromIndex(Indexed *data, Runtime *rt)
{
   Data *object = executeAST(data->object, rt);
   Data *index = executeAST(data->value, rt);
   void *result = NULL;

   if (object->type == TYPE_DICT)
   {
      result = dict_get(index, DICT_PTR(object));
      const char *key = dataTostring(index);

      if (!result)
      {
         data_free(object);
         data_free(index);
         throw_error(ERROR_KEY, "'%s' key not found", key);
         return createData(TYPE_NONE, NULL);
      }

      Data *cloned = cloneData(result);
      data_free(object);
      data_free(index);
      return cloned;
   }

   int i = validateIndex(index, object);
   int len = seqlen(object);

   if (i >= len || (i < 0 && -i > len))
   {
      throw_error(ERROR_INDEX, "Index out of range");
      return createData(TYPE_NONE, NULL);
   }

   if (object->type == TYPE_LIST)
   {
      List *list = LIST_PTR(object);
      result = list_get((i < 0 ? len + i : i), list);
   }
   else // TYPE_STR
   {
      const char *str = object->str;
      char *ch = str_char_at(str, (i < 0 ? len + i : i));
      result = createData(TYPE_STR, ch);
      free(ch);
   }
   data_free(index);
   data_free(object);
   return (Data *)result;
}

static void indexedAssign(Indexed *index, Data *value, Runtime *rt)
{
   Data *idx = executeAST(index->value, rt);
   Data *obj = executeAST(index->object, rt);

   if (obj->type == TYPE_DICT)
      dict_insert(cloneData(idx), value, DICT_PTR(obj));

   else if (obj->type == TYPE_LIST)
   {
      int i = validateIndex(idx, obj);
      List *list = LIST_PTR(obj);
      int len = list->length;

      if (i >= len || (i < 0 && -i > len))
         throw_error(ERROR_INDEX, "Index out of range");

      list_set(i < 0 ? len + i : i, value, list, data_free);
   }
   else
   {
      throw_error(
          ERROR_TYPE, "'%s' object does not support item assignment",
          getDataType(obj->type));
   }

   data_free(idx);
   data_free(obj);
}

// Helper function to handle assignment to different target types
static void assignToTarget(ASTnode *target, Data *value, Runtime *rt)
{
   if (!target || !target->data || !value || !rt)
      return;

   Data *targetData = target->data;

   switch (targetData->type)
   {
   case TYPE_LOOKUP:
   {
      // Simple variable: x = value
      Data *key = createData(TYPE_STR, targetData->any);
      if (value->type == TYPE_LAMBDA)
      {
         Function *lambda = FUNC_PTR(value);
         lambda->name = cloneData(key);
         value->type = TYPE_FUNCTION;
         defineFunction(lambda, rt);
         data_free(key);
         break;
      }

      env_save(key, value, rt->env);
      data_free(key);
      break;
   }
   case TYPE_INDEX:
      // Indexed assignment: x[i] = value or obj.attr[i] = value
      indexedAssign((Indexed *)targetData->any, value, rt);
      break;

   case TYPE_ATTRIBUTE:
   {
      // Attribute assingment: mother.food = value
      Attribute *attr = (Attribute *)targetData->any;

      Data *object = executeAST(attr->object, rt);
      setAttribute(object, attr->attrib, value);

      data_free(object);
      break;
   }

   default:
      throw_error(ERROR_SYNTAX, "cannot assign to this expression");
      data_free(value);
      break;
   }
}

static void executeAssignment(Assignment *asmt, Runtime *rt)
{
   if (!asmt || !rt)
      return;

   List *vars = asmt->vars;
   List *values = asmt->values;

   // First, evaluate all the right-hand side values
   List *evaluatedValues = list_create(__len__);
   for (int i = 0; i < values->length; i++)
   {
      Data *val = executeAST((ASTnode *)values->items[i], rt);
      list_append(val, evaluatedValues);
   }

   // Check for unpacking errors
   if (vars->length < evaluatedValues->length)
   {
      throw_error(ERROR_VALUE,
                  "not enough values to unpack (expected %d, got %d)",
                  vars->length, evaluatedValues->length);
      list_free(evaluatedValues, data_free);
      return;
   }
   else if (vars->length > evaluatedValues->length)
   {
      throw_error(ERROR_VALUE,
                  "too many values to unpack (expected %d)", vars->length);
      list_free(evaluatedValues, data_free);
      return;
   }

   // Now handle each assignment
   for (int i = 0; i < vars->length; i++)
   {
      ASTnode *lhs = (ASTnode *)vars->items[i];
      Data *rhs = cloneData((Data *)evaluatedValues->items[i]);

      // Handle augmented assignment (+=, -=, etc.)
      if (asmt->op)
      {
         Data *currentValue = executeAST(lhs, rt);
         Operator op = *(Operator *)asmt->op->data->any;
         Data *newValue = handleBinaryOperation(currentValue, rhs, op);
         data_free(currentValue);
         data_free(rhs);
         rhs = newValue;
      }

      // Perform the assignment based on the LHS type
      assignToTarget(lhs, rhs, rt);
   }

   // Don't free the data, it's been assigned
   list_free(evaluatedValues, data_free);
}

static Status executeException(Exception *exception, void **returns, Runtime *rt)
{
   if (!exception || !exception->tried || !exception->catched)
      return FLOW_NORMAL;

   return handleException(exception, returns, rt);
}

static void executeEnum(Enum *e, Runtime *rt)
{
   if (!e || !rt)
      return;

   int counter = 0;

   for (int i = 0; i < e->items->length; i++)
   {
      EnumItem *item = (EnumItem *)e->items->items[i];

      if (item->hasValue)
      {
         // Evaluate the explicit value and update counter
         Data *val = executeAST(item->value, rt);
         if (val->type != TYPE_INT)
         {
            throw_error(ERROR_TYPE, "enum value must be an integer");
            data_free(val);
            return;
         }
         counter = *(int *)val->atom;
         data_free(val);
      }

      // Save the current counter value as a plain int variable
      Data *key = createData(TYPE_STR, item->name);
      Data *val = createData(TYPE_INT, &counter);
      env_save(key, val, rt->env);
      data_free(key);

      counter++;
   }
}

Status executeStatement(Statement *stmt, void **returns, Runtime *rt)
{
   if (!stmt)
      return FLOW_NORMAL;

   g_thrown.lineno = stmt->lineno;

   switch (stmt->type)
   {
   case STMT_ASMT:
      executeAssignment((Assignment *)stmt->data, rt);
      break;

   case STMT_EXPR:
   {
      Data *res = executeAST((ASTnode *)stmt->data, rt);
      if (returns && res)
         *returns = res;
      else
         data_free(res);
      break;
   }

   case STMT_FUNC:
      defineFunction((Function *)stmt->data, rt);
      break;

   case STMT_CLASS:
      defineClass((Class *)stmt->data, rt);
      break;

   case STMT_IMPORT:
      executeImport((Import *)stmt->data, rt);
      break;

   case STMT_RETURN:
      *returns = executeAST((ASTnode *)stmt->data, rt);
      return FLOW_RETURN;

   case STMT_FLOW:
   {
      Status status = executeFlow((Flow *)stmt->data, rt, returns);
      return (status == FLOW_TAKEN) ? FLOW_NORMAL : status;
   }

   case STMT_WHILE:
      return executeWhile((WhileLoop *)stmt->data, rt, returns);

   case STMT_FOR:
      return executeFor((ForLoop *)stmt->data, rt, returns);

   case STMT_BREAK:
      return FLOW_BREAK;

   case STMT_CONTINUE:
      return FLOW_CONTINUE;

   case STMT_EXCEPTION:
      return executeException((Exception *)stmt->data, returns, rt);

   case STMT_THROW:
   {
      Data *val = executeAST((ASTnode *)stmt->data, rt);
      const char *msg = val->type == TYPE_STR ? val->str : dataTostring(val);
      throw_error(ERROR_RUNTIME, "%s", msg);
      data_free(val);
      return FLOW_EXCEPTION;
   }

   case STMT_GLOBAL:
      global_exec((char *)stmt->data, rt);
      break;

   case STMT_NONLOCAL:
      nonlocal_exec((char *)stmt->data, rt);
      break;

   case STMT_ENUM:
      executeEnum((Enum *)stmt->data, rt);
      break;

   default:
      throw_error(ERROR_RUNTIME, "Unknown statement type at line %d", stmt->lineno);
      return -1;
   }
   return FLOW_NORMAL;
}

static Data *executeAttribute(Attribute *attr, Runtime *rt)
{
   if (!attr || !rt)
      return NULL;

   Data *object = executeAST(attr->object, rt);

   // Module attribute access (e.g. math.pi, math.sqrt)
   if (object->type == TYPE_RUNTIME)
   {
      Runtime *mod_rt = (Runtime *)object->any;
      Data *lookupKey = createData(TYPE_STR, attr->attrib->str);
      Data *result = getData(lookupKey, mod_rt);
      data_free(lookupKey);
      data_free(object);
      return result ? cloneData(result) : createData(TYPE_NONE, NULL);
   }

   Data *result = getAttribute(object, attr->attrib, rt);
   data_free(object);
   return cloneData(result);
}

static Data *sliceIndex(ASTnode *index, int def, Runtime *rt)
{
   if (index)
      return executeAST(index, rt);
   int value = def;
   return createData(TYPE_INT, &value);
}

static Data *executeSlice(Indexed *index, Runtime *rt)
{
   Data *obj = executeAST(index->object, rt);
   Slice *slice = (Slice *)index->value;

   if (!obj)
      return createData(TYPE_NONE, NULL);

   DataType obj_type = obj->type;

   // Evaluate step first to know direction
   Data *step = sliceIndex(slice->step, 1, rt);
   int step_val = step->atom ? *(int *)step->atom : 1;

   // Use sentinel values that str_slice/list_slice recognize
   Data *start, *stop;
   if (step_val < 0)
   {
      start = sliceIndex(slice->start, INT_MAX, rt);
      stop = sliceIndex(slice->stop, INT_MIN, rt);
   }
   else
   {
      start = sliceIndex(slice->start, INT_MIN, rt);
      stop = sliceIndex(slice->stop, INT_MAX, rt);
   }

   if (step_val == 0)
   {
      data_free(start);
      data_free(stop);
      data_free(step);
      data_free(obj);
      throw_error(ERROR_VALUE, "slice step cannot be zero");
      return createData(TYPE_NONE, NULL);
   }

   int (*get)(Data *, Data *) = validateIndex;

   Range *r = createRange(
       get(start, obj),
       get(stop, obj),
       get(step, obj));

   void *res = NULL;

   if (obj_type == TYPE_STR)
      res = str_slice(obj->str, r->start, r->stop, r->step);
   else if (obj_type == TYPE_LIST)
   {
      List *list = LIST_PTR(obj);
      res = list_slice(r->start, r->stop, r->step, list);
   }

   free(r);
   data_free(start);
   data_free(stop);
   data_free(step);
   data_free(obj);

   return createData(obj_type, res);
}

static Data *executeTernary(Ternary *stmt, Runtime *rt)
{
   if (!stmt || !rt)
      return createData(TYPE_NONE, NULL);

   Data *cond = executeAST(stmt->condition, rt);
   int condValue = dataToBool(cond);
   data_free(cond);
   return condValue ? executeAST(stmt->trueExpr, rt) : executeAST(stmt->falseExpr, rt);
}

static Data *evalFString(Data *leaf, Runtime *rt)
{
   const char *src = leaf->str;
   int srclen = strlen(src);

   int cap = srclen * 2 + 64;
   char *result = malloc(cap);
   int rlen = 0;

   int i = 0;
   while (i < srclen)
   {
      if (src[i] == '{' && i + 1 < srclen && src[i + 1] != '{')
      {
         // Find closing brace
         int j = i + 1;
         while (j < srclen && src[j] != '}')
            j++;

         if (j >= srclen)
         {
            throw_error(ERROR_SYNTAX, "f-string: missing closing '}'");
            free(result);
            return createData(TYPE_NONE, NULL);
         }

         // Extract expression string
         int exprlen = j - i - 1;
         char *expr = malloc(exprlen + 1);
         strncpy(expr, src + i + 1, exprlen);
         expr[exprlen] = '\0';

         // Parse and evaluate the expression
         Data *val = evalFStringExpr(expr, rt);
         free(expr);

         const char *valstr = dataTostring(val);
         int vallen = strlen(valstr);

         while (rlen + vallen + 2 >= cap)
         {
            cap *= 2;
            result = realloc(result, cap);
         }

         memcpy(result + rlen, valstr, vallen);
         rlen += vallen;
         data_free(val);
         i = j + 1;
      }
      else if (src[i] == '{' && i + 1 < srclen && src[i + 1] == '{')
      {
         if (rlen + 2 >= cap)
         {
            cap *= 2;
            result = realloc(result, cap);
         }
         result[rlen++] = '{';
         i += 2;
      }
      else if (src[i] == '}' && i + 1 < srclen && src[i + 1] == '}')
      {
         if (rlen + 2 >= cap)
         {
            cap *= 2;
            result = realloc(result, cap);
         }
         result[rlen++] = '}';
         i += 2;
      }
      else if (src[i] == '\\' && i + 1 < srclen)
      {
         if (rlen + 2 >= cap)
         {
            cap *= 2;
            result = realloc(result, cap);
         }
         i++;
         switch (src[i])
         {
         case 'n':
            result[rlen++] = '\n';
            break;
         case 't':
            result[rlen++] = '\t';
            break;
         case 'r':
            result[rlen++] = '\r';
            break;
         case '\\':
            result[rlen++] = '\\';
            break;
         case '"':
            result[rlen++] = '"';
            break;
         case '\'':
            result[rlen++] = '\'';
            break;
         case 'a':
            result[rlen++] = '\a';
            break;
         case 'b':
            result[rlen++] = '\b';
            break;
         case 'f':
            result[rlen++] = '\f';
            break;
         case 'v':
            result[rlen++] = '\v';
            break;
         case '0':
            result[rlen++] = '\0';
            break;
         case 'x':
         {
            if (i + 2 < srclen && isxdigit(src[i + 1]) && isxdigit(src[i + 2]))
            {
               char hex[3] = {src[i + 1], src[i + 2], '\0'};
               result[rlen++] = (char)strtol(hex, NULL, 16);
               i += 2;
            }
            else
               result[rlen++] = 'x';
            break;
         }
         default:
            result[rlen++] = '\\';
            result[rlen++] = src[i];
            break;
         }
         i++;
      }
      else
      {
         if (rlen + 2 >= cap)
         {
            cap *= 2;
            result = realloc(result, cap);
         }
         result[rlen++] = src[i++];
      }
   }

   result[rlen] = '\0';
   Data *out = createData(TYPE_STR, result);
   free(result);
   return out;
}

Data *executeLookUp(char *var, Runtime *rt)
{
   if (!var || !rt)
      return createData(TYPE_NONE, NULL);
   Data *key = createData(TYPE_STR, var);
   Data *result = getData(key, rt);
   data_free(key);
   return result;
}

static Data *executeData(Data *leaf, Runtime *rt)
{
   void *value = NULL;

   switch (leaf->type)
   {
   case TYPE_INVOKED:
      value = executeInvoked((Invoked *)leaf->any, rt);
      break;

   case TYPE_INDEX:
      value = getFromIndex((Indexed *)leaf->any, rt);
      break;

   case TYPE_LIST_EXPR:
      value = evalListValues((Data *)leaf, rt);
      break;

   case TYPE_DICT_EXPR:
      value = evalDictValues((Data *)leaf, rt);
      break;

   case TYPE_SET_EXPR:
      value = evalSetValues((Data *)leaf, rt);
      break;

   case TYPE_ATTRIBUTE:
      value = executeAttribute((Attribute *)leaf->any, rt);
      break;

   case TYPE_LOOKUP:
      value = executeLookUp(leaf->any, rt);
      break;

   case TYPE_SLICE:
      value = executeSlice((Indexed *)leaf->any, rt);
      break;

   case TYPE_INSTANCE:
      value = leaf;
      break;

   case TYPE_TERNARY:
      value = executeTernary((Ternary *)leaf->any, rt);
      break;

   case TYPE_AWAIT:
      value = executeAwait((Await *)leaf->any, rt);
      break;

   case TYPE_LAMBDA:
      value = cloneData(leaf);
      break;

   case TYPE_FSTRING:
      value = evalFString(leaf, rt);
      break;

   default:
      value = cloneData(leaf);
      break;
   }
   return (Data *)value;
}

Status executeBody(List *body, void **returns, Runtime *rt)
{
   if (!body)
      return FLOW_NORMAL;

   for (int i = 0; i < body->length; i++)
   {
      Statement *stmt = (Statement *)body->items[i];
      if (!stmt)
         continue;

      Status status = executeStatement(stmt, returns, rt);

      if (status != FLOW_NORMAL || g_thrown.error)
         return status;
   }

   return FLOW_NORMAL;
}

Data *executeAST(ASTnode *tree, Runtime *rt)
{
   if (!tree)
      return createData(TYPE_NONE, NULL);

   if (!tree->left && !tree->right && tree->data)
      return executeData(tree->data, rt);

   Data *left = (tree->left) ? executeAST(tree->left, rt) : NULL;
   Data *right = (tree->right) ? executeAST(tree->right, rt) : NULL;

   // Handle unary operations (e.g., NOT)
   if (!left && right)
   {
      Operator op = *(Operator *)tree->data->any;
      return handleUnaryOperation(right, op);
   }

   // Validate binary operation operands
   if (!left || !right)
   {
      if (left)
         data_free(left);
      if (right)
         data_free(right);
      throw_error(ERROR_RUNTIME, "insufficient operands");
      return NULL;
   }

   // Handle binary operations
   Operator op = *(Operator *)tree->data->any;
   Data *result = handleBinaryOperation(left, right, op);

   data_free(left);
   data_free(right);

   return result;
}

Data *getFromIndex_vm(Data *obj, Data *idx, Runtime *rt)
{
   Indexed indexed = {.object = NULL, .value = NULL};
   // Reuse existing getFromIndex logic directly
   if (obj->type == TYPE_LIST)
   {
      if (idx->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "list index must be an integer");
         return createData(TYPE_NONE, NULL);
      }
      List *l = LIST_PTR(obj);
      int i = *idx->atom;
      if (i < 0)
         i += l->length;
      if (i < 0 || i >= l->length)
      {
         throw_error(ERROR_INDEX, "list index out of range");
         return createData(TYPE_NONE, NULL);
      }
      return cloneData((Data *)l->items[i]);
   }
   if (obj->type == TYPE_DICT)
   {
      Data *result = dict_get(idx, DICT_PTR(obj));
      if (!result)
      {
         throw_error(ERROR_KEY, "'%s' key not found",
                     dataTostring(idx));
         return createData(TYPE_NONE, NULL);
      }
      return cloneData(result);
   }
   if (obj->type == TYPE_STR)
   {
      if (idx->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "string index must be an integer");
         return createData(TYPE_NONE, NULL);
      }
      int i = *idx->atom;
      int len = strlen(obj->str);
      if (i < 0)
         i += len;
      if (i < 0 || i >= len)
      {
         throw_error(ERROR_INDEX, "string index out of range");
         return createData(TYPE_NONE, NULL);
      }
      char ch[2] = {obj->str[i], '\0'};
      return createData(TYPE_STR, ch);
   }
   (void)indexed;
   (void)rt;
   throw_error(ERROR_TYPE, "'%s' object is not subscriptable",
               getDataType(obj->type));
   return createData(TYPE_NONE, NULL);
}

Data *setIndex_vm(Data *obj, Data *idx, Data *val, Runtime *rt)
{
   (void)rt;
   if (obj->type == TYPE_LIST)
   {
      if (idx->type != TYPE_INT)
      {
         throw_error(ERROR_TYPE, "list index must be an integer");
         return createData(TYPE_NONE, NULL);
      }
      List *l = LIST_PTR(obj);
      int i = *idx->atom;
      if (i < 0)
         i += l->length;
      if (i < 0 || i >= l->length)
      {
         throw_error(ERROR_INDEX, "list index out of range");
         return createData(TYPE_NONE, NULL);
      }
      data_free((Data *)l->items[i]);
      l->items[i] = cloneData(val);
      return createData(TYPE_NONE, NULL);
   }
   if (obj->type == TYPE_DICT)
   {
      dict_insert(cloneData(idx), cloneData(val), DICT_PTR(obj));
      return createData(TYPE_NONE, NULL);
   }
   throw_error(ERROR_TYPE, "'%s' object does not support item assignment",
               getDataType(obj->type));
   return createData(TYPE_NONE, NULL);
}

Data *setAttribute_vm(Data *obj, Data *name, Data *val, Runtime *rt)
{
   (void)rt;
   if (obj->type == TYPE_INSTANCE)
   {
      Instance *inst = INST_PTR(obj);
      dict_insert(cloneData(name), cloneData(val), inst->attributes);
      return createData(TYPE_NONE, NULL);
   }
   throw_error(ERROR_ATTRIBUTE,
               "cannot set attribute '%s' on '%s'",
               name->str, getDataType(obj->type));
   return createData(TYPE_NONE, NULL);
}

Data *executeImport_vm(const char *name, Runtime *rt)
{
   Data *mod_name = createData(TYPE_STR, (void *)name);
   Import imp = {.module = mod_name, .alias = NULL, .items = NULL, .importAll = 0};
   executeImport(&imp, rt);
   Data *result = getData(mod_name, rt);
   data_free(mod_name);
   return result;
}

Data *evalFString_vm(Data *fmt, Runtime *rt)
{
   return evalFString(fmt, rt);
}
