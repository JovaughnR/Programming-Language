#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "./lib/sys.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

// Stored at startup from main()
int g_argc = 0;
char **g_argv = NULL;

void sys_init(int argc, char **argv)
{
   g_argc = argc;
   g_argv = argv;
}

const ModuleFuncInfo SYS_MODULE[] = {
    {MODULE_SYS, SYS_ARGS, "__sys_args__", 0, 0},
    {MODULE_SYS, SYS_EXIT, "__sys_exit__", 0, 1},
    {MODULE_SYS, SYS_PLATFORM, "__sys_platform__", 0, 0},
    {MODULE_SYS, SYS_VERSION, "__sys_version__", 0, 0},
    {MODULE_SYS, SYS_GETPID, "__sys_getpid__", 0, 0},
    {MODULE_SYS, SYS_STDIN, "__sys_stdin__", 0, 0},
    {MODULE_SYS, SYS_STDOUT, "__sys_stdout__", 0, 0},
    {MODULE_SYS, SYS_STDERR, "__sys_stderr__", 0, 0},
};

const int SYS_MODULE_COUNT = sizeof(SYS_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Implementations
//=========================================================

// args() -> list
// Returns command line arguments as a list of strings.
// args[0] is the script name, args[1..] are the user arguments.
// Example: ./interpreter script.cx hello world
//          sys.args() -> ["script.cx", "hello", "world"]
Data *__sys_args__(void)
{
   List *list = list_create(__size__);
   for (int i = 1; i < g_argc; i++)
      list_append(createData(TYPE_STR, strdup(g_argv[i])), list);
   return createData(TYPE_LIST, list);
}

// exit(code=0) -> none
// Exits the program with the given exit code.
// Example: sys.exit()    # exits with code 0
// Example: sys.exit(1)   # exits with code 1
Data *__sys_exit__(Data *code)
{
   int c = 0;
   if (code && code->type == TYPE_INT)
      c = *(int *)code->atom;
   exit(c);
   return createData(TYPE_NONE, NULL);
}

// platform() -> str
// Returns the current operating system platform string.
// Example: sys.platform() -> "macos" | "linux" | "windows"
Data *__sys_platform__(void)
{
#if defined(_WIN32) || defined(_WIN64)
   return createData(TYPE_STR, strdup("windows"));
#elif defined(__APPLE__)
   return createData(TYPE_STR, strdup("macos"));
#elif defined(__linux__)
   return createData(TYPE_STR, strdup("linux"));
#else
   return createData(TYPE_STR, strdup("unknown"));
#endif
}

// version() -> str
// Returns the CX language version string.
// Example: sys.version() -> "cx 1.0.0"
Data *__sys_version__(void)
{
   return createData(TYPE_STR, strdup("cx 1.0.0"));
}

// getpid() -> int
// Returns the current process ID.
// Example: sys.getpid() -> 12345
Data *__sys_getpid__(void)
{
   int pid = (int)getpid();
   return createData(TYPE_INT, &pid);
}

// stdin() -> file
// Returns a file handle to standard input.
// Example: f = sys.stdin(); line = io.readline(f)
Data *__sys_stdin__(void)
{
   return createData(TYPE_FILE, stdin);
}

// stdout() -> file
// Returns a file handle to standard output.
// Example: f = sys.stdout(); io.write(f, "hello")
Data *__sys_stdout__(void)
{
   return createData(TYPE_FILE, stdout);
}

// stderr() -> file
// Returns a file handle to standard error.
// Example: f = sys.stderr(); io.write(f, "error message")
Data *__sys_stderr__(void)
{
   return createData(TYPE_FILE, stderr);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeSysModule(Runtime *mod_rt)
{
   for (int i = 0; i < SYS_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)SYS_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&SYS_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchSysBuiltin(SysType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case SYS_ARGS:
      return __sys_args__();
   case SYS_EXIT:
      return __sys_exit__(ARG(0));
   case SYS_PLATFORM:
      return __sys_platform__();
   case SYS_VERSION:
      return __sys_version__();
   case SYS_GETPID:
      return __sys_getpid__();
   case SYS_STDIN:
      return __sys_stdin__();
   case SYS_STDOUT:
      return __sys_stdout__();
   case SYS_STDERR:
      return __sys_stderr__();
   default:
      throw_error(ERROR_SYNTAX, "unknown sys builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}