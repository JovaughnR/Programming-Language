#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "./lib/env.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo ENV_MODULE[] = {
    {MODULE_ENV, ENV_LOAD, "__env_load__", 0, 1},
    {MODULE_ENV, ENV_GET, "__env_get__", 1, 2},
    {MODULE_ENV, ENV_SET, "__env_set__", 2, 2},
    {MODULE_ENV, ENV_HAS, "__env_has__", 1, 1},
    {MODULE_ENV, ENV_ALL, "__env_all__", 0, 0},
    {MODULE_ENV, ENV_UNSET, "__env_unset__", 1, 1},
};

const int ENV_MODULE_COUNT = sizeof(ENV_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Helpers
//=========================================================

static const char *get_str(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE || d->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a string", funcname);
      return NULL;
   }
   return d->str;
}

// Strip leading/trailing whitespace in place
static char *strip(char *s)
{
   while (*s && isspace((unsigned char)*s))
      s++;
   char *end = s + strlen(s) - 1;
   while (end > s && isspace((unsigned char)*end))
      *end-- = '\0';
   return s;
}

// Parse a single .env line into key/value
// Returns 1 on success, 0 if line should be skipped
static int parse_env_line(const char *line, char **out_key, char **out_val)
{
   char *copy = strdup(line);
   char *s = strip(copy);

   // Skip empty lines and comments
   if (*s == '\0' || *s == '#')
   {
      free(copy);
      return 0;
   }

   char *eq = strchr(s, '=');
   if (!eq)
   {
      free(copy);
      return 0;
   }

   *eq = '\0';
   char *key = strip(s);
   char *val = strip(eq + 1);

   // Strip surrounding quotes from value
   int vlen = strlen(val);
   if (vlen >= 2 &&
       ((val[0] == '"' && val[vlen - 1] == '"') ||
        (val[0] == '\'' && val[vlen - 1] == '\'')))
   {
      val[vlen - 1] = '\0';
      val++;
   }

   // Strip inline comments (unquoted # after value)
   char *comment = strchr(val, '#');
   if (comment && comment > val && isspace((unsigned char)*(comment - 1)))
      *comment = '\0';
   val = strip(val);

   *out_key = strdup(key);
   *out_val = strdup(val);
   free(copy);
   return 1;
}

//=========================================================
//  Implementations
//=========================================================

// load(path=".env") -> dict
// Loads a .env file and sets all variables in the environment.
// Returns a dict of all loaded key/value pairs.
// Lines starting with # are comments.
// Supports quoted values: KEY="value with spaces"
// Example: env.load()          # loads .env
// Example: env.load(".env.prod")
Data *__env_load__(Data *path)
{
   const char *filepath = ".env";
   if (path && path->type == TYPE_STR)
      filepath = path->str;

   FILE *f = fopen(filepath, "r");
   if (!f)
   {
      throw_error(ERROR_RUNTIME, "env.load(): cannot open file '%s'", filepath);
      return createData(TYPE_NONE, NULL);
   }

   Dict *result = dict_create(__size__);
   char buf[4096];

   while (fgets(buf, sizeof(buf), f))
   {
      // Strip trailing newline
      int len = strlen(buf);
      if (len > 0 && buf[len - 1] == '\n')
         buf[--len] = '\0';
      if (len > 0 && buf[len - 1] == '\r')
         buf[--len] = '\0';

      char *key, *val;
      if (!parse_env_line(buf, &key, &val))
         continue;

      // Set in actual process environment
      setenv(key, val, 1);

      // Add to result dict
      Data *k = createData(TYPE_STR, key);
      Data *v = createData(TYPE_STR, val);
      dict_insert(k, v, result);
      free(val);
   }

   fclose(f);
   return createData(TYPE_DICT, result);
}

// get(key, default=none) -> str | none
// Gets an environment variable by name.
// Returns default if the variable is not set.
// Example: env.get("HOME")           -> "/Users/jovaughn"
// Example: env.get("MISSING", "N/A") -> "N/A"
Data *__env_get__(Data *key, Data *default_val)
{
   const char *k = get_str(key, "env.get");
   if (!k)
      return createData(TYPE_NONE, NULL);

   const char *val = getenv(k);
   if (!val)
   {
      if (default_val && default_val->type != TYPE_NONE)
         return cloneData(default_val);
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_STR, strdup(val));
}

// set(key, value) -> none
// Sets an environment variable for the current process.
// Example: env.set("DEBUG", "true")
Data *__env_set__(Data *key, Data *value)
{
   const char *k = get_str(key, "env.set");
   const char *v = get_str(value, "env.set");
   if (!k || !v)
      return createData(TYPE_NONE, NULL);

   setenv(k, v, 1);
   return createData(TYPE_NONE, NULL);
}

// has(key) -> bool
// Returns true if the environment variable is set.
// Example: env.has("HOME") -> true
// Example: env.has("UNDEFINED_VAR") -> false
Data *__env_has__(Data *key)
{
   const char *k = get_str(key, "env.has");
   if (!k)
      return createData(TYPE_BOOL, &(int){0});
   int result = getenv(k) != NULL;
   return createData(TYPE_BOOL, &result);
}

// all() -> dict
// Returns a dict of all current environment variables.
// Example: vars = env.all()
// Example: print(vars["HOME"])
Data *__env_all__(void)
{
   extern char **environ;
   Dict *result = dict_create(__size__);

   for (char **e = environ; *e; e++)
   {
      char *copy = strdup(*e);
      char *eq = strchr(copy, '=');
      if (!eq)
      {
         free(copy);
         continue;
      }

      *eq = '\0';
      Data *k = createData(TYPE_STR, strdup(copy));
      Data *v = createData(TYPE_STR, strdup(eq + 1));
      dict_insert(k, v, result);
      free(copy);
   }

   return createData(TYPE_DICT, result);
}

// unset(key) -> none
// Unsets an environment variable.
// Example: env.unset("DEBUG")
Data *__env_unset__(Data *key)
{
   const char *k = get_str(key, "env.unset");
   if (!k)
      return createData(TYPE_NONE, NULL);
   unsetenv(k);
   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeEnvModule(Runtime *mod_rt)
{
   for (int i = 0; i < ENV_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)ENV_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&ENV_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchEnvBuiltin(EnvType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case ENV_LOAD:
      return __env_load__(ARG(0));
   case ENV_GET:
      return __env_get__(ARG(0), ARG(1));
   case ENV_SET:
      return __env_set__(ARG(0), ARG(1));
   case ENV_HAS:
      return __env_has__(ARG(0));
   case ENV_ALL:
      return __env_all__();
   case ENV_UNSET:
      return __env_unset__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown env builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}