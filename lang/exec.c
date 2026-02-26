
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

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

   Environment *globalEnv = rt->env;

   while (globalEnv->parent)
      globalEnv = globalEnv->parent;

   Environment *currentEnv = rt->env;

   // If we're already in global scope, nothing to do
   if (currentEnv == globalEnv)
      return;

   // Check if variable is already declared in local scope
   if (dict_has(var, currentEnv->vars))
   {
      throw_error(
          ERROR_SYNTAX,
          "name '%s' is assigned to before global declaration",
          var);
      return;
   }

   // Mark this variable as global in current scope
   Data *marker = createData(TYPE_INT, &(int){1});
   Data *name = createData(TYPE_STR, var);

   dict_insert(name, marker, currentEnv->global_vars);

   // Initialize in global scope if it doesn't exist
   if (!dict_has(name, globalEnv->vars))
   {
      Data *none = createData(TYPE_NONE, NULL);
      saveInEnvironment(name, none, globalEnv);
   }

   data_free(name);
   free(var);
}

static void nonlocal_exec(char *var, Runtime *rt)
{
   if (!var || !rt)
      return;

   Environment *currentEnv = rt->env;

   // Check if we're in global scope
   if (currentEnv->parent == NULL)
   {
      throw_error(
          ERROR_SYNTAX,
          "nonlocal declaration not allowed at module level");
      return;
   }

   // Check if variable is already declared in local scope
   if (dict_has(var, currentEnv->vars))
   {
      throw_error(
          ERROR_SYNTAX,
          "name '%s' is assigned to before nonlocal declaration",
          var);
      return;
   }

   // Find the variable in an enclosing scope
   Data *name = createData(TYPE_STR, var);
   Environment *enclosingScope = findEnclosingScope(name, rt);

   if (!enclosingScope)
   {
      throw_error(
          ERROR_SYNTAX,
          "no binding for nonlocal '%s' found",
          var);
      return;
   }

   Data *marker = createData(TYPE_INT, &(int){1});
   dict_insert(name, marker, currentEnv->nonlocal_vars);
   free(var);
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

   saveInEnvironment(cloneData(iterator), cloneData(iter), rt->env);
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
      data_free(object);
      data_free(index);
      if (!result)
      {
         throw_error(
             ERROR_KEY, "'%s' key not found", key);
         return createData(TYPE_NONE, NULL);
      }
      return cloneData(result);
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
      dict_insert(cloneData(index), value, DICT_PTR(obj));

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
      saveInEnvironment(key, value, rt->env);
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
         ASTnode *expr = createASTexpr(
             asmt->op,
             createASTnode(currentValue),
             createASTnode(rhs));

         Data *newValue = executeAST(expr, rt);
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

Status executeStatement(Statement *stmt, void **returns, Runtime *rt)
{
   if (!stmt)
      return FLOW_NORMAL;

   g_thrown.lineno = stmt->lineno;

   switch (stmt->type)
   {
   case STMT_ASSIGNMENT:
      executeAssignment((Assignment *)stmt->data, rt);
      break;

   case STMT_EXPRESSION:
      executeAST((ASTnode *)stmt->data, rt);
      break;

   case STMT_FUNCTION:
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

   case STMT_GLOBAL:
      global_exec((char *)stmt->data, rt);
      break;

   case STMT_NONLOCAL:
      nonlocal_exec((char *)stmt->data, rt);
      break;

   default:
      throw_error(ERROR_RUNTIME, "Unknown statement type at line %d", stmt->lineno);
      return -1;
   }
   return FLOW_NORMAL;
}

static Data *executeFunction(Invoked *caller, Runtime *rt)
{
   return executeInvoked(caller, rt);
}

static Data *executeAttribute(Attribute *attr, Runtime *rt)
{
   if (!attr || !rt)
      return NULL;

   Data *object = executeAST(attr->object, rt);
   Data *attrName = attr->attrib;

   if (!object || !attrName)
   {
      data_free(object);
      data_free(attrName);
      return NULL;
   }

   // Handle attribute access based on object type
   Data *result = getAttribute(object, attrName, rt);

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

static Data *executeData(Data *leaf, Runtime *rt)
{
   void *value = NULL;

   switch (leaf->type)
   {
   case TYPE_INVOKED:
      value = executeFunction((Invoked *)leaf->any, rt);
      break;

   case TYPE_INSTANCE:
      value = leaf;
      break;

   case TYPE_INDEX:
      value = getFromIndex((Indexed *)leaf->any, rt);
      break;

   case TYPE_LIST:
      value = evalListValues(leaf, rt);
      break;

   case TYPE_ATTRIBUTE:
      value = executeAttribute((Attribute *)leaf->any, rt);
      break;

   case TYPE_SET:
      value = evalSetValues(leaf, rt);
      break;

   case TYPE_DICT:
      value = evalDictValues(leaf, rt);
      break;

   case TYPE_LOOKUP:
   {
      Data *key = createData(TYPE_STR, leaf->any);
      value = getData(key, rt);
      data_free(key);
      break;
   }

   case TYPE_SLICE:
      value = executeSlice((Indexed *)leaf->any, rt);
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
