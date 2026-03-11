#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "./lib/os.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo OS_MODULE[] = {
    {MODULE_OS, OS_EXISTS, "__os_exists__", 1, 1},
    {MODULE_OS, OS_LISTDIR, "__os_listdir__", 1, 1},
    {MODULE_OS, OS_MKDIR, "__os_mkdir__", 1, 1},
    {MODULE_OS, OS_REMOVE, "__os_remove__", 1, 1},
    {MODULE_OS, OS_RENAME, "__os_rename__", 2, 2},
    {MODULE_OS, OS_GETCWD, "__os_getcwd__", 0, 0},
    {MODULE_OS, OS_GETENV, "__os_getenv__", 1, 1},
    {MODULE_OS, OS_ISFILE, "__os_isfile__", 1, 1},
    {MODULE_OS, OS_ISDIR, "__os_isdir__", 1, 1},
    {MODULE_OS, OS_GETSIZE, "__os_getsize__", 1, 1},
};

const int OS_MODULE_COUNT = sizeof(OS_MODULE) / sizeof(ModuleFuncInfo);

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

// exists(path) -> bool
// Returns true if the path exists (file or directory)
Data *__os_exists__(Data *path)
{
   const char *p = get_str(path, "os.exists");
   if (!p)
      return createData(TYPE_BOOL, &(int){0});
   struct stat st;
   int exists = stat(p, &st) == 0;
   return createData(TYPE_BOOL, &exists);
}

// isfile(path) -> bool
// Returns true if path exists and is a regular file
Data *__os_isfile__(Data *path)
{
   const char *p = get_str(path, "os.isfile");
   if (!p)
      return createData(TYPE_BOOL, &(int){0});
   struct stat st;
   if (stat(p, &st) != 0)
      return createData(TYPE_BOOL, &(int){0});
   int result = S_ISREG(st.st_mode);
   return createData(TYPE_BOOL, &result);
}

// isdir(path) -> bool
// Returns true if path exists and is a directory
Data *__os_isdir__(Data *path)
{
   const char *p = get_str(path, "os.isdir");
   if (!p)
      return createData(TYPE_BOOL, &(int){0});
   struct stat st;
   if (stat(p, &st) != 0)
      return createData(TYPE_BOOL, &(int){0});
   int result = S_ISDIR(st.st_mode);
   return createData(TYPE_BOOL, &result);
}

// listdir(path) -> list
// Returns a list of filenames in the given directory
Data *__os_listdir__(Data *path)
{
   const char *p = get_str(path, "os.listdir");
   if (!p)
      return createData(TYPE_NONE, NULL);

   DIR *dir = opendir(p);
   if (!dir)
   {
      throw_error(ERROR_RUNTIME, "os.listdir(): cannot open directory '%s'", p);
      return createData(TYPE_NONE, NULL);
   }

   List *result = list_create(__size__);
   struct dirent *entry;
   while ((entry = readdir(dir)) != NULL)
   {
      // Skip . and ..
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
         continue;
      list_append(createData(TYPE_STR, strdup(entry->d_name)), result);
   }
   closedir(dir);
   return createData(TYPE_LIST, result);
}

// mkdir(path) -> bool
// Creates a directory at path, returns true on success
Data *__os_mkdir__(Data *path)
{
   const char *p = get_str(path, "os.mkdir");
   if (!p)
      return createData(TYPE_BOOL, &(int){0});
   int result = mkdir(p, 0755) == 0;
   if (!result)
      throw_error(ERROR_RUNTIME, "os.mkdir(): failed to create directory '%s'", p);
   return createData(TYPE_BOOL, &result);
}

// remove(path) -> bool
// Deletes a file at path, returns true on success
Data *__os_remove__(Data *path)
{
   const char *p = get_str(path, "os.remove");
   if (!p)
      return createData(TYPE_BOOL, &(int){0});
   int result = remove(p) == 0;
   if (!result)
      throw_error(ERROR_RUNTIME, "os.remove(): failed to remove '%s'", p);
   return createData(TYPE_BOOL, &result);
}

// rename(src, dst) -> bool
// Renames/moves a file from src to dst, returns true on success
Data *__os_rename__(Data *src, Data *dst)
{
   const char *s = get_str(src, "os.rename");
   const char *d = get_str(dst, "os.rename");
   if (!s || !d)
      return createData(TYPE_BOOL, &(int){0});
   int result = rename(s, d) == 0;
   if (!result)
      throw_error(ERROR_RUNTIME, "os.rename(): failed to rename '%s' to '%s'", s, d);
   return createData(TYPE_BOOL, &result);
}

// getcwd() -> str
// Returns the current working directory
Data *__os_getcwd__(void)
{
   char buffer[4096];
   if (!getcwd(buffer, sizeof(buffer)))
   {
      throw_error(ERROR_RUNTIME, "os.getcwd(): failed to get current directory");
      return createData(TYPE_NONE, NULL);
   }
   return createData(TYPE_STR, strdup(buffer));
}

// getenv(name) -> str | none
// Returns the value of an environment variable, or none if not set
Data *__os_getenv__(Data *name)
{
   const char *n = get_str(name, "os.getenv");
   if (!n)
      return createData(TYPE_NONE, NULL);
   const char *val = getenv(n);
   if (!val)
      return createData(TYPE_NONE, NULL);
   return createData(TYPE_STR, strdup(val));
}

// getsize(path) -> int
// Returns the size of a file in bytes
Data *__os_getsize__(Data *path)
{
   const char *p = get_str(path, "os.getsize");
   if (!p)
      return createData(TYPE_NONE, NULL);
   struct stat st;
   if (stat(p, &st) != 0)
   {
      throw_error(ERROR_RUNTIME, "os.getsize(): cannot stat '%s'", p);
      return createData(TYPE_NONE, NULL);
   }
   int size = (int)st.st_size;
   return createData(TYPE_INT, &size);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeOsModule(Runtime *mod_rt)
{
   for (int i = 0; i < OS_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)OS_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&OS_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchOsBuiltin(OsType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case OS_EXISTS:
      return __os_exists__(ARG(0));
   case OS_ISFILE:
      return __os_isfile__(ARG(0));
   case OS_ISDIR:
      return __os_isdir__(ARG(0));
   case OS_LISTDIR:
      return __os_listdir__(ARG(0));
   case OS_MKDIR:
      return __os_mkdir__(ARG(0));
   case OS_REMOVE:
      return __os_remove__(ARG(0));
   case OS_RENAME:
      return __os_rename__(ARG(0), ARG(1));
   case OS_GETCWD:
      return __os_getcwd__();
   case OS_GETENV:
      return __os_getenv__(ARG(0));
   case OS_GETSIZE:
      return __os_getsize__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown os builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}