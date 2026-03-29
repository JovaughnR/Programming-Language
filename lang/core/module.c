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
#include "./lib/module.h"

#include "../parse/lex.yy.h"

// ---------------------------------------------------------------------------
// Module cache
// ---------------------------------------------------------------------------

static Dict *global_module_cache = NULL;
static int builtin_module = 0;

// ---------------------------------------------------------------------------
// Path resolution
// ---------------------------------------------------------------------------

static int file_exists(const char *path)
{
   struct stat st;
   return stat(path, &st) == 0;
}

/*
 * Returns a heap-allocated path for `module_name`, or NULL if not found.
 * Also sets `builtin_module` to 1 when the file lives under ./modules/.
 */
static char *get_module_path(const char *module_name)
{
   builtin_module = 0;

   // 1. <module>.cx in current directory
   size_t base_len = strlen(module_name);

   char *path = malloc(base_len + 4); // ".cx\0"
   snprintf(path, base_len + 4, "%s.cx", module_name);
   if (file_exists(path))
      return path;
   free(path);

   // 2. ./modules/<module>.cx
   path = malloc(base_len + 14); // "./modules/.cx\0"

   snprintf(path, base_len + 14, "./modules/%s.cx", module_name);
   if (file_exists(path))
   {
      builtin_module = 1;
      return path;
   }
   free(path);

   return NULL;
}

static Runtime *load_module_into(const char *filepath, Runtime *module_rt)
{
   extern FILE *yyin;
   extern int yyparse(void);
   extern List *global_statements;
   extern Runtime *rt;

   FILE *file = fopen(filepath, "r");
   if (!file)
   {
      throw_error(ERROR_IMPORT, "Cannot open module file '%s'", filepath);
      return NULL;
   }

   // Save all parser/lexer state
   FILE *saved_yyin = yyin;
   List *saved_statements = global_statements;
   Runtime *saved_rt = rt;

   // Push current lex buffer — preserves REPL buffer across the module parse
   yypush_buffer_state(yy_create_buffer(file, YY_BUF_SIZE));

   rt = module_rt;
   yyin = file;
   global_statements = NULL;

   int ok = yyparse();
   fclose(file);

   if (ok == 0 && global_statements)
   {
      void *returns = NULL;
      executeBody(global_statements, &returns, module_rt);
      if (returns)
         data_free((Data *)returns);
   }

   // Pop back to the previous lex buffer (restores REPL buffer)
   yypop_buffer_state();

   yyin = saved_yyin;
   global_statements = saved_statements;
   rt = saved_rt;

   if (ok != 0)
   {
      throw_error(ERROR_IMPORT, "Failed to parse module '%s'", filepath);
      return NULL;
   }

   return module_rt;
}

void register_native(ModuleRegistry *reg, char *name, void (*init)(Runtime *))
{
   if (reg->count == reg->capacity)
   {
      reg->capacity *= 2;
      reg->entries = realloc(reg->entries, sizeof(ModuleEntry) * reg->capacity);
   }
   reg->entries[reg->count++] = (ModuleEntry){
       .name = name,
       .rt = NULL, // lazy — not initialized yet
       .isNative = 1,
       .initializer = init,
   };
}

// ---------------------------------------------------------------------------
// Registry lookup
// ---------------------------------------------------------------------------

Runtime *registry_get(ModuleRegistry *reg, const char *name, const char *path)
{
   for (int i = 0; i < reg->count; i++)
   {
      ModuleEntry *entry = &reg->entries[i];
      if (strcmp(entry->name, name) != 0)
         continue;

      if (entry->rt)
         return entry->rt;

      Runtime *mod_rt = createRuntime();

      // 1. Register C builtins first so .cx wrapper can reference them
      if (entry->initializer)
         entry->initializer(mod_rt);

      // 2. Parse and execute the .cx wrapper into the same runtime
      load_module_into(path, mod_rt);
      entry->rt = mod_rt;

      return entry->rt;
   }
   return NULL;
}

// ---------------------------------------------------------------------------
// Module loading
// ---------------------------------------------------------------------------

/*
 * Parse and execute `filepath` inside a fresh Runtime, then return it.
 * Returns NULL on failure (error already thrown).
 */
static Runtime *load_module(const char *filepath)
{
   Runtime *module_rt = createRuntime();
   Runtime *result = load_module_into(filepath, module_rt);
   if (!result)
   {
      runtime_free(module_rt);
      return NULL;
   }
   return module_rt;
}

/*
 * Load `module_name` with caching.  Returns the cached or newly created
 * Runtime, or NULL on failure.
 */
static Runtime *load_module_cached(const char *module_name, Runtime *rt)
{
   if (!global_module_cache)
      global_module_cache = dict_create(__len__);

   Data *key = createData(TYPE_STR, (char *)module_name);

   if (dict_has(key, global_module_cache))
   {
      Data *cached = dict_get(key, global_module_cache);
      data_free(key);
      return (Runtime *)cached->any;
   }

   char *filepath = get_module_path(module_name);
   if (!filepath)
   {
      data_free(key);
      throw_error(ERROR_IMPORT, "No module named '%s'", module_name);
      return NULL;
   }

   if (builtin_module)
   {
      Runtime *mod_rt = registry_get(rt->registry, module_name, filepath);
      if (mod_rt)
      {
         Data *cache_key = createData(TYPE_STR, (char *)module_name);
         dict_insert(cache_key, createData(TYPE_RUNTIME, mod_rt), global_module_cache);
      }
      free(filepath);
      data_free(key);
      return mod_rt;
   }

   Runtime *module_rt = load_module(filepath);
   free(filepath);

   if (!module_rt)
   {
      data_free(key);
      return NULL;
   }

   dict_insert(key, createData(TYPE_RUNTIME, module_rt), global_module_cache);
   return module_rt;
}

// ---------------------------------------------------------------------------
// Import helpers
// ---------------------------------------------------------------------------

/*
 * Import specific named items from `module_rt` into `rt`.
 */
static void import_items(Import *import, Runtime *module_rt, Runtime *rt)
{
   if (!import || !rt || !import->items)
      return;

   for (int i = 0; i < import->items->length; i++)
   {
      ASTnode *node = (ASTnode *)import->items->items[i];
      Data *item = node->data;

      if (!item || item->type != TYPE_LOOKUP)
      {
         throw_error(ERROR_SYNTAX, "Invalid import item");
         continue;
      }

      Data *key = createData(TYPE_STR, item->str);
      Data *value = getData(key, module_rt);

      if (!value)
      {
         throw_error(ERROR_IMPORT,
                     "cannot import name '%s' from module '%s'",
                     item->str, import->module);
         data_free(key);
         continue;
      }

      env_save(key, value, rt->env);
      data_free(key);
   }
}

/*
 * Import every name from `module_rt` into `parent_rt`.
 */
static void import_all(Runtime *module_rt, Runtime *parent_rt)
{
   if (!module_rt || !parent_rt)
      return;

   Dict *vars = module_rt->env->vars;
   Env *env = parent_rt->env;

   for (int i = 0; i < vars->size; i++)
      for (Pair *p = vars->buckets[i]; p; p = p->next)
      {
         Data *key = (Data *)p->key;
         Data *value = (Data *)p->value;

         if (key->type == TYPE_STR)
            env_save(key, cloneData(value), env);
      }

   return;
}

// ---------------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------------

void executeImport(Import *import, Runtime *rt)
{
   if (!import || !rt)
      return;

   const char *module_name = import->module->str;
   Runtime *module_rt = load_module_cached(module_name, rt);
   if (!module_rt)
      return;

   if (import->importAll)
   {
      import_all(module_rt, rt);
      return;
   }
   else if (import->items)
   {
      import_items(import, module_rt, rt);
      return;
   }

   // `import module` or `import module as alias`
   Data *key = import->alias ? import->alias : import->module;
   Data *value = createData(TYPE_RUNTIME, module_rt);
   env_save(key, value, rt->env);
   g_thrown.lineno = 0;
}