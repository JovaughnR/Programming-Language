
#include <stdlib.h>

#include "./lib/type.h"
#include "./lib/dict.h"
#include "./lib/list.h"
#include "./lib/utils.h"
#include "./lib/str.h"
#include "./lib/set.h"
#include "./lib/error.h"
#include "./lib/eval.h"
#include "./lib/maloc.h"
#include "./lib/class.h"

// Function prototypes
Data *executeAST(ASTnode *node, Runtime *rt);
Status executeBody(List *body, void **returns, Environment *env);
void defineFunction(Function *func, Runtime *rt);
void defineClass(Class *class, Runtime *rt);
void executeGlobal(VarDecl *decl, Runtime *rt);
void executeNonlocal(VarDecl *decl, Runtime *rt);

// extern char *error_msg;
// extern ErrorType error_type;

/**
 * Function: executeFlow
 * Recursively evaluates an IF-ELIF-ELSE statement.
 *
 * Parameters:
 * - stmt: The IF statement to evaluate.
 * - scopes: The scope stack.
 * - returns: Pointer to store return values.
 *
 * Returns:
 * - Status code indicating flow control (FLOW_NORMAL, FLOW_RETURN, etc.)
 */
Status executeFlow(Flow *stmt, Runtime *rt, void **returns)
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

   int condValue = 0;

   if (cond && cond->integer.atom)
      condValue = *(int *)cond->integer.atom;

   freeData(cond);

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

Status executeWhile(WhileLoop *loop, Runtime *rt, void **returns)
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
         condValue = *(int *)cond->integer.atom;

      freeData(cond);
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
void bindIteratorValue(char *iterator, Data *iter, Runtime *rt)
{
   if (!iterator || !iter || !rt)
      return;
   saveInEnvironment(cloneInstance(iterator), cloneInstance(iter), rt->env);
}

/**
 * Handles flow control status after executing the loop body.
 * Returns 1 if the loop should break (either BREAK or RETURN),
 * 0 otherwise (CONTINUE or normal execution).
 */
// Helper function to get the iterable data

static Data *getIterableData(ASTnode *iterableAst, Runtime *rt)
{
   if (!iterableAst)
      return NULL;

   // Evaluate the AST to get the actual iterable data
   Data *iterable = executeAST(iterableAst, rt);
   return iterable;
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
      Data *item = createInstance(TYPE_STR, str_char_at(str, i));
      Status status = executeLoopIteration(loop, item, returns, rt, &shouldBreak);
      freeData(item);

      if (status == FLOW_RETURN)
         return status;
   }

   return FLOW_NORMAL;
}

// Main for loop execution function
Status executeFor(ForLoop *loop, Runtime *rt, void **returns)
{
   if (!loop)
      return FLOW_NORMAL;

   // Evaluate the iterable expression
   Data *iterable = getIterableData(loop->iterable, rt);
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
      return iterateString(loop, iterable->str.string, returns, rt);

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
   Data *object = executeAST(createASTnode(data->variable), rt);
   Data *index = executeAST(data->value, rt);

   void *result;

   int idx = validateIndex(index, object);
   if (idx == 1)
      return NULL;

   switch (object->type)
   {
   case TYPE_DICT:
      result = dict_get(index, DICT_PTR(object), dataCompare);
      if (!result)
         throw_error(ERROR_KEY, instanceToString(index));
      break;

   case TYPE_LIST:
      result = list_get(idx, LIST_PTR(object));
      break;

   case TYPE_STR:
      char *character = str_char_at(object->str.string, idx);
      result = createInstance(TYPE_STR, character);
      break;
   }

   freeData(index);
   freeData(object);
   return createData(TYPE_INSTANCE, cloneInstance((Data *)result));
}

void assignToIndex(Data *obj, Data *index, Data *value)
{
   if (obj->type == TYPE_DICT)
   {
      dict_insert(cloneInstance(index), value, DICT_PTR(obj));
   }

   else if (obj->type == TYPE_LIST)
   {
      int idx = validateIndex(index, obj);
      void **items = LIST_PTR(obj)->items;
      if (items[idx])
         freeData((Data *)items[idx]);
      items[idx] = value;
   }
   else
   {
      throw_error(
          ERROR_TYPE,
          "'%s' object does not support item assignment",
          getDataType(obj->type));
   }

   return;
}

// New helper function to handle assignment to different target types
static void assignToTarget(ASTnode *target, Data *value, Runtime *rt)
{
   if (!target || !target->data || !value || !rt)
      return;

   Data *targetData = target->data;

   switch (targetData->type)
   {
   case TYPE_LOOKUP:
      // Simple variable: x = value
      saveInEnvironment((Data *)targetData->any, value, rt->env);
      break;

   case TYPE_INDEX:
   {
      // Indexed assignment: x[i] = value or obj.attr[i] = value
      Indexed *idx = (Indexed *)targetData->any;
      Data *obj = executeAST(createASTnode(idx->variable), rt);
      Data *index = executeAST(idx->value, rt);

      assignToIndex(obj, index, value);

      freeData(obj);
      freeData(index);
      break;
   }

   case TYPE_ATTRIBUTE:
   {
      Attribute *attr = (Attribute *)targetData->any;
      Data *object = executeAST(attr->object, rt);

      if (!object)
      {
         throw_error(ERROR_SYNTAX, "invalid object in attribute assignment");
         freeData(value);
         return;
      }

      // Extract the attribute name
      Data *attrName = attr->attrib;

      if (!attrName)
      {
         throw_error(ERROR_SYNTAX, "invalid attribute name");
         freeData(object);
         freeData(value);
         return;
      }

      setAttribute(object, attrName, value);
      freeData(object);
      break;
   }

   default:
      throw_error(ERROR_SYNTAX, "cannot assign to this expression");
      freeData(value);
      break;
   }
}

void executeAssignment(Assignment *asmt, Runtime *rt)
{
   if (!asmt || !rt)
      return;

   List *vars = asmt->vars;
   List *values = asmt->values;

   // First, evaluate all the right-hand side values
   List *evaluatedValues = list_create(values->length);
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
      freeList(evaluatedValues, TYPE_DATA);
      return;
   }
   else if (vars->length > evaluatedValues->length)
   {
      throw_error(ERROR_VALUE,
                  "too many values to unpack (expected %d)", vars->length);
      freeList(evaluatedValues, TYPE_DATA);
      return;
   }

   // Now handle each assignment
   for (int i = 0; i < vars->length; i++)
   {
      ASTnode *lhs = (ASTnode *)vars->items[i];
      Data *rhs = createData(TYPE_INSTANCE, evaluatedValues->items[i]);

      // Handle augmented assignment (+=, -=, etc.)
      if (asmt->op)
      {
         Data *currentValue = executeAST(lhs, rt);
         lhs = createASTexpr(currentValue);
         ASTnode *expr = createASTexpr(asmt->op, lhs, createASTnode(rhs));

         Data *newValue = executeAST(expr, rt);
         freeData(currentValue);
         freeData(rhs);
         rhs = newValue;
      }

      // Perform the assignment based on the LHS type
      assignToTarget(lhs, rhs, rt);
   }

   // Don't free the data, it's been assigned
   freeList(evaluatedValues, TYPE_NODATA);
}

Status executeStatement(Statement *stmt, void **returns, Runtime *rt)
{
   if (!stmt)
      return FLOW_NORMAL;

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

   case STMT_GLOBAL:
      executeGlobal((VarDecl *)stmt->data, rt);
      break;

   case STMT_NONLOCAL:
      executeNonlocal((VarDecl *)stmt->data, rt);
      break;

   default:
      throw_error(ERROR_RUNTIME, "Unknown statement type at line %d", stmt->lineno);
      return -1;
   }
   return FLOW_NORMAL;
}

Status executeBody(List *body, void **returns, Environment *env)
{
   if (!body)
      return FLOW_NORMAL;

   for (int i = 0; i < body->length; i++)
   {
      Statement *stmt = (Statement *)body->items[i];
      if (!stmt)
         continue;

      Status status = executeStatement(stmt, returns, env);

      if (status != FLOW_NORMAL)
         return status;
   }

   return FLOW_NORMAL;
}

void defineFunction(Function *func, Runtime *rt)
{
   if (!func || !rt)
      return;

   // Capture the current environment (closure)
   func->env = rt->env;

   if (func->env)
      func->env->ref++;

   Data *funcData = createInstance(TYPE_FUNCTION, func);
   if (!funcData)
      return;

   saveInEnvironment(func->name, funcData, func->env);
}

Data *executeFunction(Invoked *caller, Runtime *rt)
{
   return executeInvoked(caller, rt);
}

/**
 * Marks a variable as global in the current scope
 */
void executeGlobal(VarDecl *decl, Runtime *rt)
{
   if (!decl || !decl->name || !rt)
      return;

   Environment *globalEnv = getGlobalEnv(rt);
   Environment *currentEnv = rt->env;

   // If we're already in global scope, nothing to do
   if (currentEnv == globalEnv)
      return;

   // Check if variable is already declared in local scope
   if (dict_has(decl->name, currentEnv->vars, dataCompare))
   {
      throw_error(
          ERROR_SYNTAX,
          "name '%s' is assigned to before global declaration",
          decl->name->str.string);
      return;
   }

   // Mark this variable as global in current scope
   Data *marker = createData(TYPE_INT, &(int){1});
   saveInEnvironment(decl->name, marker, currentEnv->global_vars);

   // Initialize in global scope if it doesn't exist
   if (!dict_has(decl->name, globalEnv->vars, dataCompare))
   {
      Data *none = createData(TYPE_NONE, NULL);
      saveInEnvironment(cloneData(decl->name), none, globalEnv);
   }
}

void executeNonlocal(VarDecl *decl, Runtime *rt)
{
   if (!decl || !decl->name || !rt)
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
   if (dict_has(decl->name, currentEnv->vars, dataCompare))
   {
      throw_error(
          ERROR_SYNTAX,
          "name '%s' is assigned to before nonlocal declaration",
          decl->name->str.string);
      return;
   }

   // Find the variable in an enclosing scope
   Environment *enclosingScope = findEnclosingScope(decl->name, rt);

   if (!enclosingScope)
   {
      throw_error(
          ERROR_SYNTAX,
          "no binding for nonlocal '%s' found",
          decl->name->str.string);
      return;
   }

   Data *marker = createData(TYPE_INT, &(int){1});
   insert(cloneData(decl->name), marker, currentEnv->nonlocal_vars);
}

Data *evalAttribute(Attribute *attr, Runtime *rt)
{
   if (!attr || !rt)
      return NULL;

   Data *object = executeAST(attr->object, rt);
   Data *attrName = attr->attrib;

   if (!object || !attrName)
   {
      freeData(object);
      freeData(attrName);
      return NULL;
   }

   // Handle attribute access based on object type
   Data *result = getAttribute(object, attrName);

   freeData(object);
   // freeData(attrName);
   return result;
}

static Data *executeLeaf(Data *leaf, Runtime *rt)
{
   void *value = NULL;

   switch (leaf->type)
   {
   case TYPE_LOOKUP:;
      value = lookUpData((Data *)leaf->any, rt->env);
      break;

   case TYPE_INVOKED:
      value = executeFunction((Invoked *)leaf->any, rt);
      break;

   case TYPE_INDEX:
      value = getFromIndex((Indexed *)leaf->any, rt);
      break;

   case TYPE_ATTRIBUTE:
      value = executeAttribute((Attribute *)leaf->any, rt);
      break;

   case TYPE_LIST:
      value = evalListInDepth(leaf, rt, executeAST);
      break;

   case TYPE_SET:
      value = evalSetInDepth(leaf, rt, executeAST);
      break;

   case TYPE_DICT:
      value = evalDictInDepth(leaf, rt, executeAST);
      break;

   case TYPE_INSTANCE:
      value = leaf;
      break;

   default:
      value = cloneData(leaf);
      break;
   }
   return (Data *)value;
}

Data *executeAST(ASTnode *tree, Runtime *rt)
{
   if (!tree)
      return createData(TYPE_NONE, NULL);

   if (!tree->left && !tree->right && tree->data)
      return executeLeaf(tree->data, rt);

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
         freeData(left);
      if (right)
         freeData(right);
      throw_error(ERROR_RUNTIME, "insufficient operands");
      return NULL;
   }

   // Handle binary operations
   Operator op = *(Operator *)tree->data->any;
   Data *result = handleBinaryOperation(left, right, op);

   freeData(left);
   freeData(right);

   return result;
}