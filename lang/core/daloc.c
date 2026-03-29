#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <pthread.h>

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
      data_free(param->name);

   if (param->defaultValue)
      ast_free(param->defaultValue);

   free(param);
}

static void function_free(Function *func)
{
   if (!func)
      return;

   if (func->name)
      data_free(func->name);

   list_free(func->params, params_free);
   list_free(func->body, statement_free);
   if (func->upvalues)
      free(func->upvalues); // free the array (not the upvalues themselves)

   if (func->env)
      env_free(func->env);

   free(func);
}

static void instance_free(Instance *inst)
{
   if (!inst)
      return;

   // Don't free the class
   data_free(inst->class);
   dict_free(inst->attributes);
   free(inst);
}

void env_free(Env *env)
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

void methods_free(Method *methods)
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

   if (methods->none)
      dict_free(methods->none);

   free(methods);
}

static void registry_free(ModuleRegistry *reg)
{
   if (!reg)
      return;
   free(reg->entries);
   free(reg);
}

void runtime_free(Runtime *rt)
{
   if (!rt)
      return;

   if (rt->env)
      env_free(rt->env);

   if (rt->methods)
      rt->methods = NULL;

   if (rt->registry)
      registry_free(rt->registry);

   free(rt);
}

static void class_free(Class *class)
{
   if (!class)
      return;

   if (class->name)
      data_free(class->name);

   list_free(class->parents, data_free);
   list_free(class->statements, statement_free);
   list_free(class->mro, data_free);

   if (class->rt)
      runtime_free(class->rt);

   free(class);
}

static void future_free(Future *f)
{
   if (!f)
      return;

   if (f->result)
      data_free(f->result);

   pthread_mutex_destroy(&f->lock);
   pthread_cond_destroy(&f->ready);
   free(f);
}

void file_close(void *file)
{
   // Never close stdin, stdout, stderr
   if (file && file != stdin && file != stdout && file != stderr)
      fclose((FILE *)file);
   return;
}

static void generator_free(Generator *generator)
{
   // if (!generator)
   //    return;

   // free(generator->ident);
   // for_free(generator->loop);
   // ast_free(generator->cond);
   // free(generator);
}

static void ref_free(Object *rc, DataType type)
{
   if (!rc)
      return;

   rc->references--;
   if (rc->references > 0)
      return;

   void *object = rc->object;
   if (!object)
      return;

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
   case TYPE_LAMBDA:
      function_free((Function *)object);
      break;

   case TYPE_INSTANCE:
      instance_free((Instance *)object);
      break;

   case TYPE_FILE:
      file_close((FILE *)object);
      break;

   case TYPE_DB:
      sqlite3_close((sqlite3 *)object);
      break;

   case TYPE_STMT:
      sqlite3_finalize((sqlite3_stmt *)object);
      break;

   case TYPE_THREAD:
      free(object);
      object = NULL;
      break;

   case TYPE_MUTEX:
      pthread_mutex_destroy((pthread_mutex_t *)object);
      object = NULL;
      break;

   case TYPE_COND:
      pthread_cond_destroy((pthread_cond_t *)object);
      object = NULL;
      break;

   case TYPE_FUTURE:
      future_free((Future *)object);
      object = NULL;
      break;

   case TYPE_GENERATOR:
      generator_free((Generator *)object);
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

static void import_free(Import *import)
{
   if (!import)
      return;

   data_free(import->module);
   list_free(import->items, ast_free);
   data_free(import->alias);

   free(import);
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
   case STMT_EXPR:
      ast_free((ASTnode *)stmt->data);
      break;

   case STMT_FUNC:
      stmt->data = NULL; // Just remove ownership
      break;

   case STMT_ASMT:
      assignment_free((Assignment *)stmt->data);
      break;

   case STMT_EXCEPTION:
      exception_free((Exception *)stmt->data);
      break;

   case STMT_IMPORT:
      import_free((Import *)stmt->data);
      break;

   case STMT_CLASS:
      stmt->data = NULL; // Just remove ownership
      break;

   case STMT_NONLOCAL:
   case STMT_GLOBAL:
      free((char *)stmt->data);
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

static void ternary_free(Ternary *ternary)
{
   if (!ternary)
      return;

   ast_free(ternary->condition);
   ast_free(ternary->trueExpr);
   ast_free(ternary->falseExpr);
   free(ternary);
}

void data_free(const void *data)
{
   if (!data)
      return;

   Data *d = (Data *)data;

   switch (d->type)
   {
   case TYPE_FSTRING:
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
   case TYPE_LAMBDA:
   case TYPE_STMT:
   case TYPE_DB:
   case TYPE_THREAD:
   case TYPE_MUTEX:
   case TYPE_COND:
   case TYPE_FUTURE:
   case TYPE_FILE:
   case TYPE_GENERATOR:
      ref_free((Object *)d->ref, d->type);
      break;

   case TYPE_INVOKED:
      invoked_free((Invoked *)d->any);
      break;

   case TYPE_INDEX:
      indexed_free((Indexed *)d->any);
      break;

   case TYPE_LOOKUP:
      free((char *)d->any);
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

   case TYPE_TERNARY:
      ternary_free((Ternary *)d->any);
      break;

   case TYPE_BUILTIN:
   case TYPE_MODULE:
      break;

   case TYPE_NONE:
   default:
      break;
   }

   free(d);
}

void enum_free(Enum *e)
{
   if (!e)
      return;
   data_free(e->name);
   for (int i = 0; i < e->items->length; i++)
   {
      EnumItem *item = (EnumItem *)e->items->items[i];
      free(item->name);
      if (item->value)
         ast_free(item->value);
      free(item);
   }
   list_free(e->items, NULL);
   free(e);
}