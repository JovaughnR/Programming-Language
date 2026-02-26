#include <stdlib.h>
#include <string.h>

#include "./lib/dict.h"
#include "./lib/list.h"
#include "./lib/type.h"
#include "./lib/error.h"
#include "./lib/maloc.h"

/**
 * ref_create - Create a reference object wrapper
 * @object: The object to wrap
 *
 * Creates a reference-counted wrapper for complex objects (lists, dicts, functions, etc.)
 * Initializes reference count to 1 and marks object as not yet evaluated.
 *
 * Returns: Pointer to new Object, or NULL on allocation failure
 */
static Object *ref_create(void *object)
{
   Object *ref = (Object *)malloc(sizeof(Object));
   if (!ref)
      return NULL;

   ref->object = object;
   ref->references = 1;
   ref->isEvaluated = 0;

   return ref;
}

Data *createData(DataType type, void *value)
{
   Data *d = malloc(sizeof(Data));
   if (!d)
      return NULL;

   d->type = type;

   switch (type)
   {
   case TYPE_INT:
   case TYPE_BOOL:
   {
      int *val = malloc(sizeof(int));
      *val = *(int *)value;
      d->atom = val;
      break;
   }
   case TYPE_FLOAT:
   {
      double *val = malloc(sizeof(double));
      *val = *(double *)value;
      d->real = val;
      break;
   }

   case TYPE_STR:
   case TYPE_LOOKUP:
      d->str = strdup((char *)value);
      break;

   case TYPE_OPERATOR:
   {
      Operator *op = malloc(sizeof(Operator));
      *op = *(Operator *)value;
      d->any = op;
      break;
   }
   case TYPE_INDEX:
      d->any = (Indexed *)value;
      break;

   case TYPE_ATTRIBUTE:
      d->any = (Attribute *)value;
      break;

   case TYPE_DICT:
   case TYPE_LIST:
   case TYPE_SET:
   case TYPE_FUNCTION:
   case TYPE_CLASS:
   case TYPE_INSTANCE:
      d->ref = ref_create(value);
      break;

   case TYPE_RANGE:
      d->range = (Range *)value;
      break;

   default:
      d->any = value;
      break;
   }

   return d;
}

Indexed *createIndexed(ASTnode *object, void *value)
{
   Indexed *index = malloc(sizeof(Indexed));
   if (!index)
      return NULL;

   index->object = object;
   index->value = value;
   return index;
}

/**
 * methods_create - Initialize method dictionaries for all types
 * Creates method dictionaries for each built-in type to store their associated methods.
 * Types supported: builtin, set, list, atom (int/bool), real (float), dict, str, range.
 *
 * Note: TODO - consider whether to create method dictionary for TYPE_NONE
 *
 * Returns: Pointer to new Method structure, or NULL on allocation failure
 */
Method *createBuiltins()
{
   Method *method = (Method *)malloc(sizeof(Method));
   if (!method)
      return NULL;

   method->builtin = dict_create(__LEN__);
   method->set = dict_create(__LEN__);
   method->list = dict_create(__LEN__);
   method->atom = dict_create(__LEN__);
   method->real = dict_create(__LEN__);
   method->dict = dict_create(__LEN__);
   method->str = dict_create(__LEN__);
   method->range = dict_create(__LEN__);
   // TODO: whether or not to create method directory for TYPE 'none'
   method->none = NULL;

   return method;
}

Environment *env_create(Environment *parent)
{
   Environment *env = malloc(sizeof(Environment));
   if (!env)
      return NULL;

   env->vars = dict_create(__LEN__);
   env->parent = parent;
   env->ref = 1;
   env->global_vars = dict_create(__LEN__);
   env->nonlocal_vars = dict_create(__LEN__);

   if (parent)
      parent->ref++;

   return env;
}

Runtime *createRuntime()
{
   Runtime *rt = (Runtime *)malloc(sizeof(Runtime));
   if (!rt)
      return NULL;

   // global environment
   rt->env = env_create(NULL);
   rt->methods = builtins;
   rt->modules = NULL;

   return rt;
}

Attribute *createAttribute(ASTnode *object, char *attribute)
{
   Attribute *attr = (Attribute *)malloc(sizeof(Attribute));
   if (!attr)
      return NULL;

   attr->object = object;
   attr->attrib = createData(TYPE_LOOKUP, attribute);
   return attr;
}

ASTnode *createASTnode(Data *value)
{
   ASTnode *node = (ASTnode *)malloc(sizeof(ASTnode));
   if (!node)
      return NULL;

   node->left = NULL;
   node->right = NULL;
   node->data = value;
   return node;
}

ASTnode *createASTop(Operator op)
{
   Data *opData = createData(TYPE_OPERATOR, &op);
   ASTnode *root = createASTnode(opData);
   if (!root)
   {
      free(opData);
      return NULL;
   }
   return root;
}

ASTnode *createASTexpr(ASTnode *root, ASTnode *left, ASTnode *right)
{
   root->left = left;
   root->right = right;
   return root;
}

Pair *createPair(void *key, void *value)
{
   Pair *pair = (Pair *)malloc(sizeof(Pair));
   if (!pair)
      return NULL;

   pair->key = key;
   pair->value = value;
   pair->next = NULL;
   return pair;
}

ParamInfo *createParams(char *name, ASTnode *defaultValue)
{
   ParamInfo *pinfo = malloc(sizeof(ParamInfo));
   if (!pinfo)
      return NULL;

   pinfo->name = createData(TYPE_STR, name);
   pinfo->defaultValue = defaultValue;
   pinfo->hasDefault = defaultValue ? 1 : 0;
   return pinfo;
}

Function *createFunction(char *name, List *params, List *body)
{
   if (!name)
      return NULL;

   Function *func = (Function *)malloc(sizeof(Function));
   if (!func)
      return NULL;

   func->name = createData(TYPE_STR, name);
   func->params = params;
   func->body = body;
   func->env = NULL;
   func->locals = NULL; // Will be set during defineFunction

   return func;
}

Invoked *createInvoked(ASTnode *postfix, List *args, List *kwargs)
{
   Invoked *inv = (Invoked *)malloc(sizeof(Invoked));
   if (!inv)
      return NULL;

   // Store the entire var Data - it will be evaluated later

   inv->postfix = postfix;
   inv->args = args;
   inv->kwargs = kwargs;
   return inv;
}

Range *createRange(int start, int stop, int step)
{
   if (step == 0)
   {
      throw_error(ERROR_VALUE, "range() step cannot be zero\n");
      return NULL;
   }

   Range *range = malloc(sizeof(Range));

   range->start = start;
   range->stop = stop;
   range->step = step;

   return range;
}

ForLoop *createFor(char *iterator, ASTnode *iterable, List *body)
{
   ForLoop *loop = (ForLoop *)malloc(sizeof(ForLoop));
   if (!loop)
   {
      throw_error(ERROR_MEMORY, "Fail to allocate memory");
      return NULL;
   }
   loop->iterable = iterable;
   loop->body = body;
   loop->iterator = createData(TYPE_STR, iterator);
   return loop;
}

Statement *createStatement(StatementType type, void *data, int lineno)
{
   Statement *stmt = (Statement *)malloc(sizeof(Statement));
   if (!stmt)
   {
      throw_error(ERROR_MEMORY, "Could not allocate memory");
      return NULL;
   }

   stmt->type = type;
   stmt->data = data;
   stmt->lineno = lineno;

   // Always return the statement without executing
   return stmt;
}

Flow *createFlow(ASTnode *cond, List *body, Flow *elif, Flow *else_blck)
{
   Flow *flow = malloc(sizeof(Flow));
   if (!flow)
      throw_error(ERROR_MEMORY, "Could not allocate memory");

   if (cond)
      flow->condition = cond;

   flow->body = body;
   flow->elif_chain = elif;
   flow->else_block = else_blck;

   return flow;
}

WhileLoop *createWhile(ASTnode *condition, List *body)
{
   WhileLoop *loop = (WhileLoop *)malloc(sizeof(WhileLoop));
   if (!loop)
   {
      throw_error(ERROR_MEMORY, "Fail to allocate memory");
      return NULL;
   }

   loop->condition = condition;
   loop->body = body;
   return loop;
}

Import *createImport(char *module, List *items, char *alias, int all)
{
   Import *import = (Import *)malloc(sizeof(Import));
   if (!import)
   {
      throw_error(ERROR_MEMORY, "Failed to allocate Import");
      return NULL;
   }

   import->module = createData(TYPE_STR, module);
   import->items = items;
   import->alias = alias ? createData(TYPE_STR, alias) : NULL;
   import->importAll = all;

   return import;
}

Assignment *createAssignment(List *vars, List *values, ASTnode *op)
{
   Assignment *asmt = (Assignment *)malloc(sizeof(Assignment));
   if (!asmt)
      return NULL;

   asmt->vars = vars;     // List of vars => [x, y...]
   asmt->values = values; // List of Ast nodes [Ast, AST...]
   asmt->op = op;
   return asmt;
}

ASTnode *createReturn(ASTnode *astNode, int funcDepth)
{
   if (!funcDepth)
   {
      throw_error(ERROR_SYNTAX, "return outside function");
      exit(0);
   }
   return astNode;
}

void *createJump(int loopDepth, int jump)
{
   if (!loopDepth)
   {
      if (jump == 0)
         throw_error(ERROR_SYNTAX, "continue outside loop");
      else
         throw_error(ERROR_SYNTAX, "break outside loop");
      exit(0);
   }
   return NULL;
}

Class *createClass(char *var, List *statements, List *parents)
{
   Class *class = (Class *)malloc(sizeof(Class));
   if (!class)
      return NULL;

   class->name = createData(TYPE_STR, var);
   class->statements = statements;
   class->parents = parents;
   class->rt = createRuntime();
   class->mro = NULL;
   class->isInitialize = 0;

   return class;
}

Instance *createInstance(Class *class)
{
   if (!class)
      return NULL;

   Instance *instance = (Instance *)malloc(sizeof(Instance));
   if (!instance)
      return NULL;

   // custom classes or builtin data types
   instance->class = class;
   instance->attributes = dict_create(__size__);

   return instance;
}

Slice *createSlice(ASTnode *start, ASTnode *stop, ASTnode *step)
{
   Slice *slice = (Slice *)malloc(sizeof(Slice));
   if (!slice)
      return NULL;

   slice->start = start;
   slice->stop = stop;
   slice->step = step;

   return slice;
}

Catch *createCatch(char *errorName, char *alias, List *statements)
{
   Catch *catch = (Catch *)malloc(sizeof(Catch));
   if (!catch)
      return NULL;

   catch->errorName = createData(TYPE_STR, errorName);
   catch->alias = createData(TYPE_STR, alias);
   catch->statements = statements;

   return catch;
}

Exception *createException(List *try, Catch *error, List *finally)
{
   Exception *except = (Exception *)malloc(sizeof(Exception));
   if (!except)
      return NULL;

   except->tried = try;
   except->catched = error;
   except->finally = finally;

   return except;
}