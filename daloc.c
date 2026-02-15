#include <stdlib.h>

#include "./lib/dict.h"
#include "./lib/type.h"
#include "./lib/daloc.h"
#include "./lib/list.h"
#include "./lib/set.h"
#include "./lib/error.h"

// Function Prototypes
void data_free(const void *data);

void pair_free(const void *pair)
{
   if (!pair)
      return;

   Pair *p = (Pair *)pair;

   data_free((Data *)p->key);
   data_free((Data *)p->value);

   free(p);
}

void ast_free(const void *ast)
{
   if (!ast)
      return;

   ASTnode *node = (ASTnode *)ast;

   ast_free(node->left);
   ast_free(node->right);
   data_free(node->data);

   free(node);
}

static void params_free(const void *params)
{
   if (!params)
      return;

   ParamInfo *param = (ParamInfo *)params;

   if (param->name)
      free(param->name);

   if (param->defaultValue)
      ast_free(param->defaultValue);

   free(param);
}

static void function_free(Function *func)
{
   if (!func)
      return;

   if (func->name)
      free(func->name);

   list_free(func->params, params_free);
   list_free(func->body, statement_free);

   if (func->env)
      env_free(func->env);

   free(func);
}

static void instance_free(Instance *inst)
{
   if (!inst)
      return;

   // Don't free the class
   inst->class = NULL; // Set the reference to NULL
   dict_free(inst->attributes);
   free(inst);
}

void env_free(Environment *env)
{
   if (!env)
      return;

   env->ref--;

   if (env->ref > 0)
      return;

   dict_free(env->vars);
   dict_free(env->global_vars);
   dict_free(env->nonlocal_vars);

   if (env->parent)
      env_free(env->parent);

   free(env);
}

static void methods_free(Method *methods)
{
   if (!methods)
      return;

   dict_free(methods->builtin);

   dict_free(methods->set);

   dict_free(methods->list);

   dict_free(methods->atom);

   dict_free(methods->real);

   dict_free(methods->dict);

   dict_free(methods->str);

   dict_free(methods->range);

   dict_free(methods->none);

   free(methods);
}

void runtime_free(Runtime *rt)
{
   if (!rt)
      return;

   if (rt->env)
      env_free(rt->env);

   if (rt->methods)
      methods_free(rt->methods);

   if (rt->modules)
      dict_free(rt->modules);

   free(rt);
}

static void class_free(Class *class)
{
   if (!class)
      return;

   if (class->name)
      free(class->name);

   list_free(class->parents, data_free);
   list_free(class->statements, statement_free);
   list_free(class->parents, data_free);

   if (class->rt)
      runtime_free(class->rt);

   free(class);
}

static void ref_free(Object *rc, DataType type)
{
   if (!rc)
      return;

   rc->references--;
   if (rc->references > 0)
      return;

   void *object = rc->object;

   switch (type)
   {
   case TYPE_CLASS:
      class_free((Class *)object);
      break;

   case TYPE_LIST:
      list_free((List *)object, data_free);
      break;

   case TYPE_DICT:
      dict_free((Dict *)object);
      break;

   case TYPE_SET:
      set_free((Set *)object);
      break;

   case TYPE_FUNCTION:
      function_free((Function *)object);
      break;
   case TYPE_INSTANCE:
      instance_free((Instance *)rc->object);
      break;
   default:
      if (rc->object)
         free(rc->object);
      break;
   }
   free(rc);
}

static void for_free(ForLoop *loop)
{
   if (!loop)
      return;

   if (loop->iterator)
      free(loop->iterator);

   ast_free(loop->iterable);
   list_free(loop->body, statement_free);
   free(loop);
}

static void flow_free(Flow *flow)
{
   if (!flow)
      return;

   if (flow->condition)
      ast_free(flow->condition);

   if (flow->body)
      list_free(flow->body, statement_free);

   flow_free(flow->elif_chain);
   flow_free(flow->else_block);

   free(flow);
}

static void while_free(WhileLoop *loop)
{
   if (!loop)
      return;

   if (loop->condition)
      ast_free(loop->condition);
   if (loop->body)
      list_free(loop->body, statement_free);

   free(loop);
}

void assignment_free(Assignment *asmt)
{
   if (!asmt)
      return;

   list_free(asmt->vars, ast_free);
   list_free(asmt->values, ast_free);
   ast_free(asmt->op);

   free(asmt);
}

static void catch_free(Catch *catch)
{
   if (!catch)
      return;

   data_free(catch->alias);
   data_free(catch->errorName);
   list_free(catch->statements, statement_free);

   if (catch->errorMsg)
      free(catch->errorMsg);

   return;
}

static void exception_free(Exception *except)
{
   if (!except)
      return;

   list_free(except->tried, statement_free);
   catch_free(except->catched);
   list_free(except->finally, statement_free);
   return;
}

void statement_free(const void *statement)
{
   if (!statement)
      return;

   Statement *stmt = (Statement *)statement;

   switch (stmt->type)
   {
   case STMT_FLOW:
      flow_free((Flow *)stmt->data);
      break;

   case STMT_WHILE:
      while_free((WhileLoop *)stmt->data);
      break;

   case STMT_FOR:
      for_free((ForLoop *)stmt->data);
      break;

   case STMT_RETURN:
   case STMT_EXPRESSION:
      ast_free((ASTnode *)stmt->data);
      break;

   case STMT_FUNCTION:
      stmt->data = NULL; // Just remove ownership
      break;

   case STMT_ASSIGNMENT:
      assignment_free((Assignment *)stmt->data);
      break;

   case STMT_EXCEPTION:
      exception_free((Exception *)stmt->data);
      break;

   case STMT_BREAK:
   case STMT_CONTINUE:
   default:
      break;
   }
   free(stmt);
}

static void invoked_free(Invoked *inv)
{
   if (!inv)
      return;

   ast_free(inv->postfix);
   list_free(inv->args, data_free);
   list_free(inv->kwargs, pair_free);

   free(inv);
}

static void attribute_free(Attribute *attr)
{
   if (!attr)
      return;

   ast_free(attr->object);
   data_free(attr->attrib);
   free(attr);
}

static void indexed_free(Indexed *index)
{
   if (!index)
      return;

   ast_free(index->object);
   ast_free(index->value);
   free(index);
}

void data_free(const void *data)
{
   if (!data)
      return;

   Data *d = (Data *)data;
   switch (d->type)
   {
   case TYPE_STR:
      free(d->str);
      break;

   // Reference-counted types - decrement ref count
   case TYPE_LIST:
   case TYPE_DICT:
   case TYPE_SET:
   case TYPE_FUNCTION:
   case TYPE_CLASS:
   case TYPE_INSTANCE:
      ref_free((Object *)d->ref, d->type);
      break;

   case TYPE_INVOKED:
      invoked_free((Invoked *)d->any);
      break;

   case TYPE_INDEX:
      indexed_free((Indexed *)d->any);
      break;

   case TYPE_LOOKUP:
      data_free((Data *)d->any);
      break;

   case TYPE_INT:
   case TYPE_BOOL:
      free(d->atom);
      break;

   case TYPE_FLOAT:
      free(d->real);
      break;

   case TYPE_RANGE:
      free(d->range);
      break;

   case TYPE_OPERATOR:
      free((Operator *)d->any);
      break;

   case TYPE_ATTRIBUTE:
      attribute_free((Attribute *)d->any);
      break;

   case TYPE_NONE:
      break;
   default:
      free((char *)d);
      return;
   }
   free(d);
}
