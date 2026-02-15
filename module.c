#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "./lib/type.h"
#include "./lib/error.h"
#include "./lib/str.h"
#include "./lib/maloc.h"
#include "./lib/exec.h"
#include "./lib/daloc.h"
#include "./lib/dict.h"
#include "./lib/list.h"
#include "./lib/utils.h"

// Global module cache - shared across all runtimes
static Dict *global_module_cache = NULL;

// Helper: Check if file exists
static int file_exists(const char *filename)
{
   struct stat buffer;
   return (stat(filename, &buffer) == 0);
}

// Helper: Construct module file path
static char *get_module_path(const char *module_name)
{
   // Try module_name.cx
   size_t len = strlen(module_name) + 4; // +4 for ".cx\0"
   char *path = malloc(len);
   snprintf(path, len, "%s.cx", module_name);

   if (file_exists(path))
      return path;

   // Try module_name/__init__.cx
   free(path);
   len = strlen(module_name) + 13; // +13 for "/__init__.cx\0"
   path = malloc(len);
   snprintf(path, len, "%s/__init__.cx", module_name);

   if (file_exists(path))
      return path;

   free(path);
   return NULL;
}

// Helper: Parse and execute a module file
static Runtime *load_module(const char *filepath)
{
   extern FILE *yyin;
   extern int yyparse(void);
   extern List *global_statements;
   extern Runtime *rt;

   FILE *module_file = fopen(filepath, "r");
   if (!module_file)
   {
      throw_error(ERROR_IMPORT, "Cannot open module file '%s'", filepath);
      return NULL;
   }

   // Save current parser state
   FILE *saved_yyin = yyin;
   List *saved_statements = global_statements;

   // Create module runtime
   Runtime *module_rt = createRuntime();
   Runtime *saved_rt = rt;
   rt = module_rt;

   // Parse module
   yyin = module_file;
   global_statements = NULL;

   int parse_result = yyparse();

   fclose(module_file);

   // Execute module
   if (parse_result == 0 && global_statements)
   {
      void *returns = NULL;
      executeBody(global_statements, &returns, module_rt);
      if (returns)
         data_free((Data *)returns);
   }

   // Restore parser state
   yyin = saved_yyin;
   global_statements = saved_statements;
   rt = saved_rt;

   if (parse_result != 0)
   {
      runtime_free(module_rt);
      throw_error(ERROR_IMPORT, "Failed to parse module '%s'", filepath);
      return NULL;
   }

   return module_rt;
}

// Helper: Load module with global caching
static Runtime *load_module_cached(const char *module_name)
{
   // Initialize global cache on first use
   if (!global_module_cache)
      global_module_cache = dict_create(__len__);

   Data *module_key = createData(TYPE_STR, (char *)module_name);

   // Check if module is already in global cache
   if (dict_has(module_key, global_module_cache))
   {
      Data *cached = dict_get(module_key, global_module_cache);
      data_free(module_key);
      return (Runtime *)cached->any; // Extract cached Runtime*
   }

   // Module not cached - load it
   char *filepath = get_module_path(module_name);
   if (!filepath)
   {
      data_free(module_key);
      throw_error(ERROR_IMPORT, "No module named '%s'", module_name);
      return NULL;
   }

   Runtime *module_rt = load_module(filepath);
   free(filepath);

   if (!module_rt)
   {
      data_free(module_key);
      return NULL;
   }

   // Cache the loaded module globally
   Data *rt_wrapper = createData(TYPE_RUNTIME, module_rt);
   dict_insert(module_key, rt_wrapper, global_module_cache);

   return module_rt;
}

static void importItems(Import *import, Runtime *module_rt, Runtime *rt)
{
   if (!import || !rt || !import->items)
      return;

   for (int i = 0; i < import->items->length; i++)
   {
      ASTnode *astItem = (ASTnode *)import->items->items[i];
      Data *item = astItem->data;

      Data *item_name = NULL;
      if (item->type == TYPE_LOOKUP)
         item_name = item->any;
      else
      {
         throw_error(ERROR_SYNTAX, "Invalid import item");
         continue;
      }

      // Look up item in module (returns cloned value)
      Data *value = getData(item_name, module_rt->env);
      if (!value)
      {
         throw_error(ERROR_IMPORT,
                     "cannot import name '%s' from module '%s'",
                     item_name->str, import->module);
         continue;
      }

      // insertIntoEnv clones the key, takes ownership of value
      saveInEnvironment(item_name, value, rt->env);
   }
}

static void importAll(Runtime *module_rt, Runtime *parent_rt)
{
   if (!module_rt || !parent_rt)
      return;

   // Get the module's variable dictionary
   Dict *module_vars = module_rt->env->vars;

   // Iterate through all variables in the module
   for (int i = 0; i < module_vars->size; i++)
   {
      Pair *pair = module_vars->buckets[i];
      while (pair)
      {
         Data *name = (Data *)pair->key;
         Data *value = (Data *)pair->value;

         // Skip private names (Python convention)
         if (name->type == TYPE_STR && name->str[0] != '_')
         {
            // Clone the value for the parent environment
            // insertIntoEnv clones the key, takes ownership of value
            saveInEnvironment(name, cloneData(value), parent_rt->env);
         }

         pair = pair->next;
      }
   }
}
void executeImport(Import *import, Runtime *rt)
{
   if (!import || !rt)
      return;

   const char *module_name = import->module->str;

   // Load module (uses global cache)
   Runtime *module_rt = load_module_cached(module_name);
   if (!module_rt)
      return;

   // Handle different import types
   if (import->importAll)
   {
      importAll(module_rt, rt);
   }

   else if (import->items)
   {
      importItems(import, module_rt, rt);
   }

   else
   {
      // import module [as alias]
      Data *import_name = import->alias ? import->alias : import->module;
      Data *modules = createData(TYPE_DICT, module_rt->env->vars);
      saveInEnvironment(import_name, modules, rt->env);
   }
}
