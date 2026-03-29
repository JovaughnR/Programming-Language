#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
#include "./lib/module.h"

#include "../builtin/lib/build.h"
#include "../builtin/lib/maths.h"
#include "../builtin/lib/time.h"
#include "../builtin/lib/os.h"
#include "../builtin/lib/io.h"
#include "../builtin/lib/random.h"
#include "../builtin/lib/json.h"
#include "../builtin/lib/sys.h"
#include "../builtin/lib/path.h"
#include "../builtin/lib/regex.h"
#include "../builtin/lib/hash.h"
#include "../builtin/lib/csv.h"
#include "../builtin/lib/stats.h"
#include "../builtin/lib/decimal.h"
#include "../builtin/lib/env.h"
#include "../builtin/lib/log.h"
#include "../builtin/lib/args.h"
#include "../builtin/lib/net.h"
#include "../builtin/lib/socket.h"
#include "../builtin/lib/db.h"
#include "../builtin/lib/thread.h"
#include "../builtin/lib/http.h"

List *computeMRO(Class *cls, Runtime *parentRt);

void defineFunction(Function *func, Runtime *rt)
{
   if (!func || !rt)
      return;

   // Capture the current Env (closure)
   func->env = rt->env;

   if (func->env)
      func->env->ref++;

   Data *funcData = createData(TYPE_FUNCTION, func);
   if (!funcData)
      return;

   env_save(func->name, funcData, func->env);
}

void defineClass(Class *class, Runtime *rt)
{
   if (!class || !rt)
      return;

   if (!class->mro)
      class->mro = computeMRO(class, rt);

   if (!class->isInitialize)
   {
      // Only set parent link once — use the stable global env
      Env *globalEnv = rt->env;
      while (globalEnv->parent)
         globalEnv = globalEnv->parent;

      class->rt->env->parent = globalEnv;
      globalEnv->ref++;

      void *returns = NULL;
      executeBody(class->statements, &returns, class->rt);
      class->isInitialize = 1;
   }

   if (dict_has(class->name, rt->env->vars))
   {
      Data *existing = dict_get(class->name, rt->env->vars);
      if (existing && existing->type == TYPE_CLASS && CLASS_PTR(existing) == class)
         return;
   }

   Data *classData = createData(TYPE_CLASS, class);
   env_save(class->name, classData, rt->env);
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
         // We need to get a fresh reference from the Env
         data_free(parent);
         parent = getData(parentClassName, parentRt);
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

   // Check if any param is variadic
   int has_variadic = 0;
   for (int i = startIdx; i < func->params->length; i++)
   {
      ParamInfo *p = (ParamInfo *)func->params->items[i];
      if (p->isVariadic)
      {
         has_variadic = 1;
         break;
      }
   }

   // Only check too-many-args if no variadic param
   if (!has_variadic && argsProvided > paramsNeeded)
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

      // Variadic — collect all remaining args into a list
      if (param->isVariadic)
      {
         List *rest = list_create(__size__);
         int argIdx = i - startIdx;
         while (argIdx < argsProvided)
            list_append(cloneData((Data *)args->items[argIdx++]), rest);
         dict_insert(cloneData(param->name), createData(TYPE_LIST, rest), vars);
         return;
      }

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

static Data *exec_func(Function *func, List *args, Dict *kwargs, Data *object, Runtime *rt)
{
   if (!func || !rt)
      return createData(TYPE_NONE, NULL);

   Env *funcEnv = env_create(func->env);
   Env *prevEnv = rt->env;

   rt->env = funcEnv;
   int paramStart = 0;

   if (object && func->params && func->params->length > 0)
   {
      ParamInfo *firstParam = (ParamInfo *)func->params->items[0];
      Data *thisParam = cloneData(object);
      env_save(cloneData(firstParam->name), thisParam, funcEnv);
      paramStart = 1;
   }

   bind_params(args, kwargs, func, paramStart, rt);

   Data *returnValue = NULL;
   executeBody(func->body, (void **)&returnValue, rt);

   if (returnValue && (returnValue->type == TYPE_LAMBDA ||
                       returnValue->type == TYPE_FUNCTION))
   {
      Function *inner = FUNC_PTR(returnValue);
      if (inner->env)
         inner->env->ref--;
      inner->env = rt->env;
      inner->env->ref++;
   }

   rt->env = prevEnv;
   env_free(funcEnv);

   return returnValue ? returnValue : createData(TYPE_NONE, NULL);
}
//=========================================================
//  Execution Handlers (Modular)
//=========================================================

static Data *handle_class_call(Data *callee, List *args, Dict *kwargs, Runtime *rt, Runtime *src_rt)
{
   Class *cls = CLASS_PTR(callee);

   if (!cls->isInitialize)
      defineClass(cls, src_rt ? src_rt : rt);

   // Search main rt first, then source_rt (module rt)
   Data *classInEnv = NULL;
   if (dict_has(cls->name, rt->env->vars))
      classInEnv = getData(cls->name, rt);

   else if (src_rt && dict_has(cls->name, src_rt->env->vars))
      classInEnv = getData(cls->name, src_rt);

   if (!classInEnv)
   {
      // Last resort — register into main rt from callee directly
      Data *classData = createData(TYPE_CLASS, cls);
      env_save(cls->name, classData, rt->env);
      classInEnv = getData(cls->name, rt);
   }

   Instance *newInst = createInstance(classInEnv);
   Data *result = createData(TYPE_INSTANCE, newInst);

   Data *ctorName = createData(TYPE_STR, CONSTRUCTOR_NAME);
   Data *ctor = getAttribute(callee, ctorName, src_rt ? src_rt : rt);

   if (ctor && ctor->type == TYPE_FUNCTION)
   {
      Function *ctorFunc = FUNC_PTR(ctor);
      Data *ctorResult = exec_func(ctorFunc, args, kwargs, result, rt);
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

   if (!validArgsProvided(info->min_args, info->max_args, argCount, info->name))
      return createData(TYPE_NONE, NULL);

   return dispatchBuiltin(info->type, args, kwargs, rt);
}

static Data *handle_module_call(Data *callee, List *args, Dict *kwargs, Runtime *rt)
{
   const ModuleFuncInfo *info = (const ModuleFuncInfo *)callee->any;
   int argCount = args ? args->length : 0;

   if (!validArgsProvided(info->min_args, info->max_args, argCount, info->name))
      return createData(TYPE_NONE, NULL);

   switch (info->module)
   {
   // add more modules here as they are built
   case MODULE_MATH:
      return dispatchMathBuiltin(info->type, args, kwargs, rt);
   case MODULE_TIME:
      return dispatchTimeBuiltin(info->type, args, kwargs, rt);
   case MODULE_OS:
      return dispatchOsBuiltin(info->type, args, kwargs, rt);
   case MODULE_IO:
      return dispatchIoBuiltin(info->type, args, kwargs, rt);
   case MODULE_RANDOM:
      return dispatchRandomBuiltin(info->type, args, kwargs, rt);
   case MODULE_JSON:
      return dispatchJsonBuiltin(info->type, args, kwargs, rt);
   case MODULE_SYS:
      return dispatchSysBuiltin(info->type, args, kwargs, rt);
   case MODULE_PATH:
      return dispatchPathBuiltin(info->type, args, kwargs, rt);
   case MODULE_REGEX:
      return dispatchReBuiltin(info->type, args, kwargs, rt);
   case MODULE_HASH:
      return dispatchHashBuiltin(info->type, args, kwargs, rt);
   case MODULE_CSV:
      return dispatchCsvBuiltin(info->type, args, kwargs, rt);
   case MODULE_STATS:
      return dispatchStatsBuiltin(info->type, args, kwargs, rt);
   case MODULE_DECIMAL:
      return dispatchDecimalBuiltin(info->type, args, kwargs, rt);
   case MODULE_ENV:
      return dispatchEnvBuiltin(info->type, args, kwargs, rt);
   case MODULE_LOG:
      return dispatchLogBuiltin(info->type, args, kwargs, rt);
   case MODULE_ARGS:
      return dispatchArgsBuiltin(info->type, args, kwargs, rt);
   case MODULE_NET:
      return dispatchNetBuiltin(info->type, args, kwargs, rt);
   case MODULE_SOCKET:
      return dispatchSocketBuiltin(info->type, args, kwargs, rt);
   case MODULE_DB:
      return dispatchDbBuiltin(info->type, args, kwargs, rt);
   case MODULE_THREAD:
      return dispatchThreadBuiltin(info->type, args, kwargs, rt);
   case MODULE_HTTP:
      return dispatchHttpBuiltin(info->type, args, kwargs, rt);
   default:
      throw_error(ERROR_RUNTIME, "unknown module type %d", info->module);
      return createData(TYPE_NONE, NULL);
   }
}

static Data *handle_function_call(Data *callee, List *args, Dict *kwargs,
                                  Runtime *rt, Runtime *src_rt)
{
   Function *func = FUNC_PTR(callee);

   // If the function has no env yet, capture from source_rt
   if (!func->env && src_rt)
      func->env = src_rt->env;

   return exec_func(func, args, kwargs, NULL, rt);
}

//=========================================================
//  Main Entry Point
//=========================================================

Data *executeFunction(Data *func_data, List *args, Runtime *rt)
{
   if (!func_data || FUNC_PTR(func_data) == NULL)
      return createData(TYPE_NONE, NULL);
   return exec_func(FUNC_PTR(func_data), args, NULL, NULL, rt);
}

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
      Data *result = NULL;

      // Handle module attribute access: t.Dog
      if (object->type == TYPE_RUNTIME)
      {
         Runtime *mod_rt = (Runtime *)object->any;

         Data *lookupKey = createData(TYPE_STR, name->str);
         Data *target = getData(lookupKey, mod_rt);
         data_free(lookupKey);

         if (!target)
         {
            throw_error(ERROR_RUNTIME, "module has no attribute '%s'", name->str);
            data_free(object);
            list_free(args, data_free);
            dict_free(kwargs);
            return createData(TYPE_NONE, NULL);
         }
         data_free(object);

         switch (target->type)
         {
         case TYPE_CLASS:
            result = handle_class_call(target, args, kwargs, rt, mod_rt);
            break;
         case TYPE_BUILTIN:
            result = handle_builtin_call(target, args, kwargs, rt);
            break;
         case TYPE_MODULE:
            result = handle_module_call(target, args, kwargs, rt);
            break;
         case TYPE_LAMBDA:
         case TYPE_FUNCTION:
            result = handle_function_call(target, args, kwargs, rt, mod_rt);
            break;
         default:
            result = cloneData(target);
            break;
         }

         data_free(target);
         list_free(args, data_free);
         dict_free(kwargs);
         return result;
      }

      Data *method = getAttribute(object, name, rt);

      // Call the method
      switch (method->type)
      {
      case TYPE_BUILTIN:
         result = dispatchMethod(object, method, args, kwargs, rt);
         break;

      case TYPE_LAMBDA:
      case TYPE_FUNCTION:
      {
         Function *func = FUNC_PTR(method);
         if (object->type == TYPE_CLASS)
            result = exec_func(func, args, kwargs, NULL, rt);
         else
            result = exec_func(func, args, kwargs, object, rt);
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
      result = handle_class_call(target, args, kwargs, rt, NULL);
      break;
   case TYPE_BUILTIN:
      result = handle_builtin_call(target, args, kwargs, rt);
      break;
   case TYPE_MODULE:
      result = handle_module_call(target, args, kwargs, rt);
      break;
   case TYPE_LAMBDA:
   case TYPE_FUNCTION:
      result = handle_function_call(target, args, kwargs, rt, NULL);
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

   if (result && (result->type == TYPE_LAMBDA || result->type == TYPE_FUNCTION))
      result = cloneData(result);

   data_free(target);

   return result ? result : createData(TYPE_NONE, NULL);
}
