#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "./lib/error.h"
#include "./lib/list.h"
#include "./lib/str.h"
#include "./lib/maloc.h"
#include "./lib/exec.h"
#include "./lib/utils.h"
#include "./lib/dict.h"
#include "./lib/format.h"
#include "./lib/daloc.h"

const ErrorEntry table[] = {
    {"SyntaxError", ERROR_SYNTAX},
    {"NameError", ERROR_NAME},
    {"TypeError", ERROR_TYPE},
    {"ValueError", ERROR_VALUE},
    {"AttributeError", ERROR_ATTRIBUTE},
    {"KeyError", ERROR_KEY},
    {"IndexError", ERROR_INDEX},
    {"MemoryError", ERROR_MEMORY},
    {"ZeroDivisionError", ERROR_ZERO_DIVISION},
    {"RuntimeError", ERROR_RUNTIME},
    {"ImportError", ERROR_IMPORT},
    {"IOError", ERROR_IO},
    {"AssertionError", ERROR_ASSERTION},
    {NULL, ERROR_NONE},
};

static const char *get_error_name(ErrorType type)
{
   switch (type)
   {
   case ERROR_SYNTAX:
      return "SyntaxError";
   case ERROR_NAME:
      return "NameError";
   case ERROR_TYPE:
      return "TypeError";
   case ERROR_VALUE:
      return "ValueError";
   case ERROR_ATTRIBUTE:
      return "AttributeError";
   case ERROR_KEY:
      return "KeyError";
   case ERROR_INDEX:
      return "IndexError";
   case ERROR_MEMORY:
      return "MemoryError";
   case ERROR_ZERO_DIVISION:
      return "ZeroDivisionError";
   case ERROR_RUNTIME:
      return "RuntimeError";
   case ERROR_IMPORT:
      return "ImportError";
   case ERROR_IO:
      return "IOError";
   case ERROR_ASSERTION:
      return "AssertionError";
   default:
      return "Error";
   }
}

static char *vformat(const char *format, va_list args)
{
   va_list copy;
   va_copy(copy, args);
   int len = vsnprintf(NULL, 0, format, copy);
   va_end(copy);
   if (len < 0)
      return NULL;

   char *buf = malloc(len + 1);
   if (!buf)
      return NULL;
   vsnprintf(buf, len + 1, format, args);

   char linePrefix[32];
   snprintf(linePrefix, sizeof(linePrefix), "Line %d: ", g_thrown.lineno);

   char *result = str_concat(linePrefix, buf);
   free(buf);
   return result;
}

// Reverse lookup — maps a string name back to an ErrorType
// Used to match "except ValueError" against a thrown error
ErrorType get_error_type(const char *name)
{
   if (!name)
      return ERROR_NONE;

   if (strcmp(name, "Error") == 0)
      return ERROR_ALL;

   for (int i = 0; table[i].name; i++)
      if (strcmp(table[i].name, name) == 0)
         return table[i].type;

   return ERROR_NONE;
}

void throw_error(ErrorType type, const char *format, ...)
{
   va_list args;
   va_start(args, format);
   char *message = vformat(format, args);
   va_end(args);

   if (!message)
      message = strdup("(allocation failed)");

   if (g_thrown.active)
   {
      g_thrown.type = type;
      g_thrown.error = 1;
      snprintf(g_thrown.message, sizeof(g_thrown.message), "%s: %s",
               get_error_name(type), message);
   }
   else
   {
      fprintf(stderr, "%s%s%s%s @ %s\n",
              COLOR_BOLD, COLOR_RED,
              get_error_name(type), COLOR_RESET,
              message);

      if (!repl_mode)
         exit(EXIT_FAILURE);
   }

   free(message);
}
void throw_warning(const char *format, ...)
{
   va_list args;
   va_start(args, format);

   fprintf(stderr, "%s%sWarning%s: ",
           COLOR_BOLD, COLOR_YELLOW,
           COLOR_RESET);

   vfprintf(stderr, format, args);
   fprintf(stderr, "\n");

   va_end(args);
}

void clear_error(void)
{
   g_thrown.type = ERROR_NONE;
   g_thrown.message[0] = '\0';
   g_thrown.active = 0;
   g_thrown.error = 0;
}

static Status handleCatchBlock(Catch *catch, void **returns, Runtime *rt)
{
   // Execute the body all errors passed
   if (!catch->alias || !catch->errorName)
   {
      clear_error();
      return executeBody(catch->statements, returns, rt);
   }

   // Validate the error thrown is same as what needs to be caught
   const char *error_name = get_error_name(g_thrown.type);
   if (strcmp(catch->errorName->str, error_name) == 1)
   {
      printf("%s", g_thrown.message);
      clear_error();
      return FLOW_NORMAL;
   }

   if (catch->alias)
   {
      Data *errorMsg = createData(TYPE_STR, g_thrown.message);
      saveInEnvironment(catch->alias, errorMsg, rt->env);
   }

   Status status = FLOW_NORMAL;
   clear_error();
   status = executeBody(catch->statements, returns, rt);

   if (catch->alias)
      data_free(dict_pop(catch->alias, rt->env->vars));

   return status;
}

Status handleException(Exception *except, void **returns, Runtime *rt)
{
   if (!except)
      return FLOW_NORMAL;

   g_thrown.active = 1;
   Status status = FLOW_NORMAL;

   status = executeBody(except->tried, returns, rt);

   if (g_thrown.error && except->catched)
      status = handleCatchBlock(except->catched, returns, rt);

   // finally always runs regardless of error or return
   if (except->finally)
   {
      Status finalStatus = executeBody(except->finally, returns, rt);
      if (finalStatus == FLOW_RETURN)
         status = finalStatus; // finally's return overrides
   }

   g_thrown.active = 0;
   g_thrown.error = 0;
   return status;
}
