#include <stdlib.h>

#include "./lib/type.h"
#include "./lib/utils.h"
#include "./lib/dict.h"
#include "./lib/maloc.h"
#include "./lib/list.h"
#include "./lib/daloc.h"
#include "./lib/error.h"
#include "./lib/class.h"
#include "./lib/exec.h"
#include "./lib/format.h"
#include "./lib/build.h"

List *computeMRO(Class *cls, Runtime *parentRt);

void defineFunction(Function *func, Runtime *rt)
{
   if (!func || !rt)
      return;

   // Capture the current environment (closure)
   func->env = rt->env;

   if (func->env)
      func->env->ref++;

   Data *funcData = createData(TYPE_FUNCTION, func);
   if (!funcData)
      return;

   saveInEnvironment(func->name, funcData, func->env);
}

void defineClass(Class *class, Runtime *rt)
{
   if (!class || !rt)
      return;

   // Compute MRO if not already done
   if (!class->mro)
      class->mro = computeMRO(class, rt);

   // Execute class body to populate methods
   if (!class->isInitialize)
   {
      void *returns = NULL;
      executeBody(class->statements, &returns, class->rt);
      class->isInitialize = 1;
   }

   // Check if class is already in environment to avoid creating duplicate Data* wrappers
   if (dict_has(class->name, rt->env->vars))
   {
      Data *existing = dict_get(class->name, rt->env->vars);
      if (existing && existing->type == TYPE_CLASS && CLASS_PTR(existing) == class)
      {
         // Already properly registered
         return;
      }
   }
   // Store class in environment
   Data *classData = createData(TYPE_CLASS, class);
   saveInEnvironment(class->name, classData, rt->env);
}

// Helper function to compute MRO
List *computeMRO(Class *cls, Runtime *parentRt)
{
   List *mro = list_create(__size__);
   if (!cls->parents || cls->parents->length == 0)
      return mro;

   List *parents = cls->parents;
   for (int i = 0; i < cls->parents->length; i++)
   {
      Data *parent = executeAST(parents->items[i], parentRt);
      if (!parent || parent->type != TYPE_CLASS)
      {
         throw_error(ERROR_TYPE, "Base must be a class");
         data_free(parent);
         return NULL;
      }

      Class *parentcls = CLASS_PTR(parent);

      // Store the class name before we potentially free parent
      Data *parentClassName = parentcls->name;

      // If not initialized, define it first
      if (!parentcls->isInitialize)
      {
         defineClass(parentcls, parentRt);
         // After defineClass, the original 'parent' Data* may be freed
         // We need to get a fresh reference from the environment
         data_free(parent);
         parent = getData(parentClassName, parentRt->env);
         parentcls = CLASS_PTR(parent);
      }

      // Add the parent class itself (as Data*)
      int found = 0;
      for (int k = 0; k < mro->length; k++)
      {
         Data *existing = (Data *)mro->items[k];
         if (CLASS_PTR(existing) == parentcls)
         {
            found = 1;
            break;
         }
      }
      if (!found)
         list_append(parent, mro);

      // Add parent's MRO
      if (parentcls->mro)
      {
         for (int j = 0; j < parentcls->mro->length; j++)
         {
            Data *ancestor = (Data *)parentcls->mro->items[j];
            // Check if already in our MRO
            int found = 0;
            for (int k = 0; k < mro->length; k++)
            {
               Data *existing = (Data *)mro->items[k];
               if (CLASS_PTR(existing) == CLASS_PTR(ancestor))
               {
                  found = 1;
                  break;
               }
            }
            // Add if not duplicate
            if (!found)
               list_append(cloneData(ancestor), mro); // CLONE here too!
         }
      }
   }
   return mro;
}

//=========================================================
//  Evaluation Helpers
//=========================================================

static Dict *eval_kwargs(List *kwargsList, Runtime *rt)
{
   if (!kwargsList || kwargsList->length == 0)
      return NULL;

   Dict *kwargs = dict_create(kwargsList->length);
   if (!kwargs)
      return NULL;

   for (int i = 0; i < kwargsList->length; i++)
   {
      Pair *pair = (Pair *)kwargsList->items[i];
      if (!pair)
         continue;

      Data *val = executeAST((ASTnode *)pair->value, rt);
      dict_insert(cloneData((Data *)pair->key), val, kwargs);
   }
   return kwargs;
}

static List *eval_args(List *args, Runtime *rt)
{
   if (!args || args->length == 0)
      return list_create(__size__);

   List *evaluated = list_create(__size__);
   for (int i = 0; i < args->length; i++)
   {
      Data *val = executeAST((ASTnode *)args->items[i], rt);
      list_append(val, evaluated);
   }
   return evaluated;
}

//=========================================================
//  Parameter Binding
//=========================================================

static void bind_params(List *args, Dict *kwargs, Function *func, int startIdx, Runtime *rt)
{
   if (!func || !func->params || !rt || !rt->env)
      return;

   Dict *vars = rt->env->vars;
   int argsProvided = args ? args->length : 0;
   int paramsNeeded = func->params->length - startIdx;

   if (argsProvided > paramsNeeded)
   {
      throw_error(
          ERROR_TYPE,
          "%s() takes %d positional argument(s) but %d were given",
          dataTostring(func->name), paramsNeeded, argsProvided);
      return;
   }

   for (int i = startIdx; i < func->params->length; i++)
   {
      ParamInfo *param = (ParamInfo *)func->params->items[i];
      if (!param || !param->name)
         continue;

      int argIdx = i - startIdx;
      Data *value = NULL;

      if (kwargs && dict_has(param->name, kwargs))
         value = cloneData(dict_get(param->name, kwargs));

      else if (argIdx < argsProvided)
         value = cloneData((Data *)args->items[argIdx]);

      else if (param->hasDefault && param->defaultValue)
         value = executeAST(param->defaultValue, rt);

      else
      {
         throw_error(
             ERROR_TYPE, "%s() missing required argument: '%s'",
             dataTostring(func->name), param->name->str);
         return;
      }

      dict_insert(cloneData(param->name), value, vars);
   }
}

//=========================================================
//  Function Execution (Generalized)
//=========================================================

static Data *execute_function(Function *func, List *args, Dict *kwargs, Data *object, Runtime *rt)
{
   if (!func || !rt)
      return createData(TYPE_NONE, NULL);

   // Create function environment
   Environment *funcEnv = env_create(func->env);
   Environment *prevEnv = rt->env;

   rt->env = funcEnv;
   int paramStart = 0;

   // If this is a method call, bind first param to instance
   if (object && func->params && func->params->length > 0)
   {
      ParamInfo *firstParam = (ParamInfo *)func->params->items[0];
      Data *thisParam = cloneData(object);
      saveInEnvironment(cloneData(firstParam->name), thisParam, funcEnv);
      paramStart = 1;
   }

   // Bind remaining parameters
   bind_params(args, kwargs, func, paramStart, rt);

   // Execute function body
   Data *returnValue = NULL;
   executeBody(func->body, (void **)&returnValue, rt);

   // Restore environment
   rt->env = prevEnv;
   env_free(funcEnv);

   return returnValue ? returnValue : createData(TYPE_NONE, NULL);
}

//=========================================================
//  Execution Handlers (Modular)
//=========================================================

static Data *handle_class_call(Data *callee, List *args, Dict *kwargs, Runtime *rt)
{
   Class *cls = CLASS_PTR(callee);

   if (!cls->isInitialize)
      defineClass(cls, rt);

   Data *classInEnv = getData(cls->name, rt->env);

   if (!classInEnv)
   {
      throw_error(
          ERROR_RUNTIME,
          "class '%s' not found in runtime environment",
          cls->name->str);
      return createData(TYPE_NONE, NULL);
   }

   Instance *newInst = createInstance(CLASS_PTR(classInEnv));
   Data *result = createData(TYPE_INSTANCE, newInst);

   // Look for constructor
   Data *ctorName = createData(TYPE_STR, CONSTRUCTOR_NAME);
   Data *ctor = getAttribute(callee, ctorName, rt);

   if (ctor && ctor->type == TYPE_FUNCTION)
   {
      Function *ctorFunc = FUNCTION_PTR(ctor);
      Data *ctorResult = execute_function(ctorFunc, args, kwargs, result, rt);
      if (ctorResult)
         data_free(ctorResult);
   }

   data_free(ctorName);
   data_free(classInEnv);
   return result;
}

//=========================================================
//  OOP: Data & Class Helpers
//=========================================================

static Data *handle_builtin_call(Data *callee, List *args, Dict *kwargs, Runtime *rt)
{
   const BuiltinInfo *info = (const BuiltinInfo *)callee->any;
   int argCount = args ? args->length : 0;

   if (argCount < info->min_args)
   {
      throw_error(
          ERROR_TYPE,
          "%s() takes at least %d argument(s), %d given",
          info->name, info->min_args, argCount);
      return createData(TYPE_NONE, NULL);
   }

   if (info->max_args != -1 && argCount > info->max_args)
   {
      throw_error(
          ERROR_TYPE,
          "%s() takes at most %d argument(s), %d given",
          info->name, info->max_args, argCount);
      return createData(TYPE_NONE, NULL);
   }

   return dispatchBuiltin(info->type, args, kwargs, rt);
}

static Data *handle_function_call(Data *callee, List *args, Dict *kwargs, Runtime *rt)
{
   Function *func = FUNCTION_PTR(callee);
   return execute_function(func, args, kwargs, NULL, rt);
}

//=========================================================
//  Main Entry Point (Clean & Simple)
//=========================================================

Data *executeInvoked(Invoked *caller, Runtime *rt)
{
   if (!caller || !caller->postfix || !rt || !caller->postfix->data)
      return createData(TYPE_NONE, NULL);

   Data *result = NULL;
   Data *callee = caller->postfix->data;

   List *args = eval_args(caller->args, rt);
   Dict *kwargs = eval_kwargs(caller->kwargs, rt);

   // Check if postfix is an ATTRIBUTE (method call case: obj.method())
   if (callee->type == TYPE_ATTRIBUTE)
   {
      Attribute *attr = (Attribute *)callee->any;

      Data *object = executeAST(attr->object, rt);
      Data *name = attr->attrib;
      Data *method = getAttribute(object, name, rt);
      Data *result = NULL;

      // Call the method
      switch (method->type)
      {
      case TYPE_BUILTIN:
         result = dispatchMethod(object, method, args, kwargs, rt);
         break;

      case TYPE_FUNCTION:
      {
         Function *func = FUNCTION_PTR(method);
         result = execute_function(func, args, kwargs, object, rt);
         break;
      }
      default:
         throw_error(ERROR_RUNTIME, "Unknown method");
         break;
      }

      data_free(object);
      list_free(args, data_free);
      dict_free(kwargs);
      return result;
   }

   // Evaluate the postfix to get the callable
   Data *target = executeAST(caller->postfix, rt);

   switch (target->type)
   {
   case TYPE_CLASS:
      result = handle_class_call(target, args, kwargs, rt);
      break;
   case TYPE_BUILTIN:
      result = handle_builtin_call(target, args, kwargs, rt);
      break;
   case TYPE_FUNCTION:
      result = handle_function_call(target, args, kwargs, rt);
      break;
   default:
      throw_error(ERROR_TYPE, "'%s' object is not callable",
                  getDataType(target->type));
      result = createData(TYPE_NONE, NULL);
   }

   if (args)
      list_free(args, data_free);
   if (kwargs)
      dict_free(kwargs);
   data_free(target);

   return result ? result : createData(TYPE_NONE, NULL);
}