#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>

#include "./lib/path.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo PATH_MODULE[] = {
    {MODULE_PATH, PATH_JOIN, "__path_join__", 1, -1},
    {MODULE_PATH, PATH_BASENAME, "__path_basename__", 1, 1},
    {MODULE_PATH, PATH_DIRNAME, "__path_dirname__", 1, 1},
    {MODULE_PATH, PATH_EXTENSION, "__path_extension__", 1, 1},
    {MODULE_PATH, PATH_ABSOLUTE, "__path_absolute__", 1, 1},
    {MODULE_PATH, PATH_NORMALIZE, "__path_normalize__", 1, 1},
    {MODULE_PATH, PATH_SPLIT, "__path_split__", 1, 1},
    {MODULE_PATH, PATH_SPLITEXT, "__path_splitext__", 1, 1},
    {MODULE_PATH, PATH_ISABS, "__path_isabs__", 1, 1},
    {MODULE_PATH, PATH_EXPANDUSER, "__path_expanduser__", 1, 1},
};

const int PATH_MODULE_COUNT = sizeof(PATH_MODULE) / sizeof(ModuleFuncInfo);

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

//=========================================================
//  Implementations
//=========================================================

// join(a, b, ...) -> str
// Joins path components together with the OS separator.
// Example: path.join("/usr", "local", "bin") -> "/usr/local/bin"
Data *__path_join__(List *args)
{
   if (!args || args->length == 0)
      return createData(TYPE_STR, strdup(""));

   // Unwrap if called with a single list argument (variadic *args)
   if (args->length == 1 && ((Data *)args->items[0])->type == TYPE_LIST)
      args = LIST_PTR((Data *)args->items[0]);

   int total = 0;
   for (int i = 0; i < args->length; i++)
   {
      Data *d = (Data *)args->items[i];
      if (!d || d->type == TYPE_NONE || d->type != TYPE_STR || strlen(d->str) == 0)
         continue;
      total += strlen(d->str) + 1;
   }

   char *result = malloc(total + 2);
   result[0] = '\0';
   int first = 1;

   for (int i = 0; i < args->length; i++)
   {
      Data *d = (Data *)args->items[i];
      if (!d || d->type == TYPE_NONE || d->type != TYPE_STR || strlen(d->str) == 0)
         continue;

      if (!first && d->str[0] == '/')
      {
         result[0] = '\0';
         strcat(result, d->str);
         first = 0;
         continue;
      }

      if (!first && result[strlen(result) - 1] != '/')
         strcat(result, "/");

      strcat(result, d->str);
      first = 0;
   }

   return createData(TYPE_STR, result);
}
// basename(path) -> str
// Returns the final component of a path.
// Example: path.basename("/usr/local/bin") -> "bin"
// Example: path.basename("/usr/local/bin/") -> ""
Data *__path_basename__(Data *p)
{
   const char *str = get_str(p, "path.basename");
   if (!str)
      return createData(TYPE_NONE, NULL);

   char *copy = strdup(str);
   char *result = strdup(basename(copy));
   free(copy);
   return createData(TYPE_STR, result);
}

// dirname(path) -> str
// Returns the directory component of a path.
// Example: path.dirname("/usr/local/bin") -> "/usr/local"
Data *__path_dirname__(Data *p)
{
   const char *str = get_str(p, "path.dirname");
   if (!str)
      return createData(TYPE_NONE, NULL);

   char *copy = strdup(str);
   char *result = strdup(dirname(copy));
   free(copy);
   return createData(TYPE_STR, result);
}

// extension(path) -> str
// Returns the file extension including the dot, or "" if none.
// Example: path.extension("file.txt") -> ".txt"
// Example: path.extension("file")     -> ""
Data *__path_extension__(Data *p)
{
   const char *str = get_str(p, "path.extension");
   if (!str)
      return createData(TYPE_STR, strdup(""));

   const char *dot = strrchr(str, '.');
   if (!dot || dot == str)
      return createData(TYPE_STR, strdup(""));

   // Make sure the dot is in the filename not a hidden file
   const char *slash = strrchr(str, '/');
   if (slash && dot < slash)
      return createData(TYPE_STR, strdup(""));

   return createData(TYPE_STR, strdup(dot));
}

// absolute(path) -> str
// Returns the absolute path by resolving relative to cwd.
// Example: path.absolute("./modules") -> "/usr/local/lang/modules"
Data *__path_absolute__(Data *p)
{
   const char *str = get_str(p, "path.absolute");
   if (!str)
      return createData(TYPE_NONE, NULL);

   char resolved[PATH_MAX];
   if (!realpath(str, resolved))
   {
      // realpath fails if path doesn't exist — build manually
      char cwd[PATH_MAX];
      if (!getcwd(cwd, sizeof(cwd)))
         return createData(TYPE_STR, strdup(str));

      char *result = malloc(strlen(cwd) + strlen(str) + 2);
      snprintf(result, strlen(cwd) + strlen(str) + 2, "%s/%s", cwd, str);
      return createData(TYPE_STR, result);
   }

   return createData(TYPE_STR, strdup(resolved));
}

// normalize(path) -> str
// Normalizes a path by resolving . and .. components.
// Example: path.normalize("/usr/local/../bin") -> "/usr/bin"
Data *__path_normalize__(Data *p)
{
   const char *str = get_str(p, "path.normalize");
   if (!str)
      return createData(TYPE_NONE, NULL);

   char *result = malloc(strlen(str) + 1);
   char *parts[256];
   int count = 0;
   char *copy = strdup(str);
   int is_abs = str[0] == '/';
   char *token = strtok(copy, "/");

   while (token)
   {
      if (strcmp(token, ".") == 0)
      {
         // skip
      }
      else if (strcmp(token, "..") == 0)
      {
         if (count > 0)
            count--;
      }
      else
         parts[count++] = token;
      token = strtok(NULL, "/");
   }

   result[0] = '\0';
   if (is_abs)
      strcat(result, "/");
   for (int i = 0; i < count; i++)
   {
      if (i > 0)
         strcat(result, "/");
      strcat(result, parts[i]);
   }

   if (result[0] == '\0')
      strcat(result, ".");

   Data *ret = createData(TYPE_STR, strdup(result));
   free(result);
   free(copy);
   return ret;
}

// split(path) -> list
// Splits a path into [dirname, basename].
// Example: path.split("/usr/local/bin") -> ["/usr/local", "bin"]
Data *__path_split__(Data *p)
{
   const char *str = get_str(p, "path.split");
   if (!str)
      return createData(TYPE_NONE, NULL);

   char *copy1 = strdup(str);
   char *copy2 = strdup(str);
   char *dir = strdup(dirname(copy1));
   char *base = strdup(basename(copy2));

   List *result = list_create(__size__);
   list_append(createData(TYPE_STR, dir), result);
   list_append(createData(TYPE_STR, base), result);

   free(copy1);
   free(copy2);
   return createData(TYPE_LIST, result);
}

// splitext(path) -> list
// Splits a path into [root, extension].
// Example: path.splitext("file.txt")     -> ["file", ".txt"]
// Example: path.splitext("/path/f.tar.gz") -> ["/path/f.tar", ".gz"]
Data *__path_splitext__(Data *p)
{
   const char *str = get_str(p, "path.splitext");
   if (!str)
      return createData(TYPE_NONE, NULL);

   const char *dot = strrchr(str, '.');
   const char *slash = strrchr(str, '/');

   List *result = list_create(__size__);

   if (!dot || dot == str || (slash && dot < slash))
   {
      list_append(createData(TYPE_STR, strdup(str)), result);
      list_append(createData(TYPE_STR, strdup("")), result);
   }
   else
   {
      int root_len = dot - str;
      char *root = malloc(root_len + 1);
      strncpy(root, str, root_len);
      root[root_len] = '\0';

      list_append(createData(TYPE_STR, root), result);
      list_append(createData(TYPE_STR, strdup(dot)), result);
   }

   return createData(TYPE_LIST, result);
}

// isabs(path) -> bool
// Returns true if the path is absolute.
// Example: path.isabs("/usr/local") -> true
// Example: path.isabs("./local")    -> false
Data *__path_isabs__(Data *p)
{
   const char *str = get_str(p, "path.isabs");
   if (!str)
      return createData(TYPE_BOOL, &(int){0});
   int result = str[0] == '/';
   return createData(TYPE_BOOL, &result);
}

// expanduser(path) -> str
// Expands ~ to the home directory.
// Example: path.expanduser("~/documents") -> "/Users/jovaughn/documents"
Data *__path_expanduser__(Data *p)
{
   const char *str = get_str(p, "path.expanduser");
   if (!str)
      return createData(TYPE_NONE, NULL);

   if (str[0] != '~')
      return createData(TYPE_STR, strdup(str));

   const char *home = getenv("HOME");
   if (!home)
      return createData(TYPE_STR, strdup(str));

   char *result = malloc(strlen(home) + strlen(str) + 1);
   strcpy(result, home);
   strcat(result, str + 1); // skip the ~
   return createData(TYPE_STR, result);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializePathModule(Runtime *mod_rt)
{
   for (int i = 0; i < PATH_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)PATH_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&PATH_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchPathBuiltin(PathType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case PATH_JOIN:
      return __path_join__(args);
   case PATH_BASENAME:
      return __path_basename__(ARG(0));
   case PATH_DIRNAME:
      return __path_dirname__(ARG(0));
   case PATH_EXTENSION:
      return __path_extension__(ARG(0));
   case PATH_ABSOLUTE:
      return __path_absolute__(ARG(0));
   case PATH_NORMALIZE:
      return __path_normalize__(ARG(0));
   case PATH_SPLIT:
      return __path_split__(ARG(0));
   case PATH_SPLITEXT:
      return __path_splitext__(ARG(0));
   case PATH_ISABS:
      return __path_isabs__(ARG(0));
   case PATH_EXPANDUSER:
      return __path_expanduser__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown path builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}