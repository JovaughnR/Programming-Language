#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "./lib/log.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo LOG_MODULE[] = {
    {MODULE_LOG, LOG_DEBUG, "__log_debug__", 1, 1},
    {MODULE_LOG, LOG_INFO, "__log_info__", 1, 1},
    {MODULE_LOG, LOG_WARN, "__log_warn__", 1, 1},
    {MODULE_LOG, LOG_ERROR, "__log_error__", 1, 1},
    {MODULE_LOG, LOG_FATAL, "__log_fatal__", 1, 1},
    {MODULE_LOG, LOG_SETLEVEL, "__log_setlevel__", 1, 1},
    {MODULE_LOG, LOG_SETFILE, "__log_setfile__", 1, 1},
    {MODULE_LOG, LOG_SETFORMAT, "__log_setformat__", 1, 1},
    {MODULE_LOG, LOG_CLOSE, "__log_close__", 0, 0},
};

const int LOG_MODULE_COUNT = sizeof(LOG_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Global Logger State
//=========================================================

static LogLevel g_level = LOG_LEVEL_DEBUG;
static FILE *g_file = NULL; // NULL means stderr
static int g_use_color = 1;
static int g_show_time = 1;
static int g_show_level = 1;

// Format flags
typedef enum
{
   LOG_FMT_PLAIN,    // "message"
   LOG_FMT_STANDARD, // "[LEVEL] message"
   LOG_FMT_FULL,     // "2024-01-01 12:00:00 [LEVEL] message"
} LogFormat;

static LogFormat g_format = LOG_FMT_FULL;

// ANSI color codes
#define COLOR_RESET "\033[0m"
#define COLOR_DEBUG "\033[36m" // cyan
#define COLOR_INFO "\033[32m"  // green
#define COLOR_WARN "\033[33m"  // yellow
#define COLOR_ERROR "\033[31m" // red
#define COLOR_FATAL "\033[35m" // magenta

static const char *level_names[] = {
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

static const char *level_colors[] = {
    COLOR_DEBUG, COLOR_INFO, COLOR_WARN, COLOR_ERROR, COLOR_FATAL};

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

static FILE *get_output(void)
{
   return g_file ? g_file : stderr;
}

static void write_log(LogLevel level, const char *message)
{
   if (level < g_level)
      return;

   FILE *out = get_output();
   int use_color = g_use_color && !g_file; // only color to terminal

   // Timestamp
   char timebuf[32] = "";
   if (g_format == LOG_FMT_FULL)
   {
      time_t now = time(NULL);
      struct tm *tm = localtime(&now);
      strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm);
   }

   // Build the log line
   if (use_color)
      fprintf(out, "%s", level_colors[level]);

   switch (g_format)
   {
   case LOG_FMT_PLAIN:
      fprintf(out, "%s", message);
      break;
   case LOG_FMT_STANDARD:
      fprintf(out, "[%-5s] %s", level_names[level], message);
      break;
   case LOG_FMT_FULL:
      fprintf(out, "%s [%-5s] %s", timebuf, level_names[level], message);
      break;
   }

   if (use_color)
      fprintf(out, "%s", COLOR_RESET);
   fprintf(out, "\n");
   fflush(out);
}

//=========================================================
//  Implementations
//=========================================================

// debug(message) -> none
// Logs a message at DEBUG level (cyan).
// Only shown when log level is DEBUG.
// Example: log.debug("entering function foo")
Data *__log_debug__(Data *msg)
{
   const char *s = get_str(msg, "log.debug");
   if (!s)
      return createData(TYPE_NONE, NULL);
   write_log(LOG_LEVEL_DEBUG, s);
   return createData(TYPE_NONE, NULL);
}

// info(message) -> none
// Logs a message at INFO level (green).
// Example: log.info("server started on port 8080")
Data *__log_info__(Data *msg)
{
   const char *s = get_str(msg, "log.info");
   if (!s)
      return createData(TYPE_NONE, NULL);
   write_log(LOG_LEVEL_INFO, s);
   return createData(TYPE_NONE, NULL);
}

// warn(message) -> none
// Logs a message at WARN level (yellow).
// Example: log.warn("config file not found, using defaults")
Data *__log_warn__(Data *msg)
{
   const char *s = get_str(msg, "log.warn");
   if (!s)
      return createData(TYPE_NONE, NULL);
   write_log(LOG_LEVEL_WARN, s);
   return createData(TYPE_NONE, NULL);
}

// error(message) -> none
// Logs a message at ERROR level (red).
// Example: log.error("failed to connect to database")
Data *__log_error__(Data *msg)
{
   const char *s = get_str(msg, "log.error");
   if (!s)
      return createData(TYPE_NONE, NULL);
   write_log(LOG_LEVEL_ERROR, s);
   return createData(TYPE_NONE, NULL);
}

// fatal(message) -> none
// Logs a message at FATAL level (magenta) then exits with code 1.
// Example: log.fatal("unrecoverable error — shutting down")
Data *__log_fatal__(Data *msg)
{
   const char *s = get_str(msg, "log.fatal");
   if (!s)
      return createData(TYPE_NONE, NULL);
   write_log(LOG_LEVEL_FATAL, s);
   if (g_file)
      fclose(g_file);
   exit(1);
   return createData(TYPE_NONE, NULL);
}

// setlevel(level) -> none
// Sets the minimum log level. Messages below this level are suppressed.
// Levels: "debug", "info", "warn", "error", "fatal", "off"
// Example: log.setlevel("warn")   # suppresses debug and info
// Example: log.setlevel("off")    # suppresses all output
Data *__log_setlevel__(Data *level)
{
   const char *s = get_str(level, "log.setlevel");
   if (!s)
      return createData(TYPE_NONE, NULL);

   if (strcmp(s, "debug") == 0)
      g_level = LOG_LEVEL_DEBUG;
   else if (strcmp(s, "info") == 0)
      g_level = LOG_LEVEL_INFO;
   else if (strcmp(s, "warn") == 0)
      g_level = LOG_LEVEL_WARN;
   else if (strcmp(s, "error") == 0)
      g_level = LOG_LEVEL_ERROR;
   else if (strcmp(s, "fatal") == 0)
      g_level = LOG_LEVEL_FATAL;
   else if (strcmp(s, "off") == 0)
      g_level = LOG_LEVEL_OFF;
   else
   {
      throw_error(ERROR_VALUE, "log.setlevel(): unknown level '%s'", s);
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_NONE, NULL);
}

// setfile(path) -> none
// Redirects all log output to a file instead of stderr.
// Pass none to revert back to stderr.
// Example: log.setfile("app.log")
// Example: log.setfile(none)  # back to stderr
Data *__log_setfile__(Data *path)
{
   // Close existing file if any
   if (g_file)
   {
      fclose(g_file);
      g_file = NULL;
   }

   if (!path || path->type == TYPE_NONE)
      return createData(TYPE_NONE, NULL);

   const char *p = get_str(path, "log.setfile");
   if (!p)
      return createData(TYPE_NONE, NULL);

   g_file = fopen(p, "a");
   if (!g_file)
   {
      throw_error(ERROR_RUNTIME, "log.setfile(): cannot open file '%s'", p);
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_NONE, NULL);
}

// setformat(format) -> none
// Sets the log output format.
// "plain"    -> "message"
// "standard" -> "[LEVEL] message"
// "full"     -> "2024-01-01 12:00:00 [LEVEL] message"
// Example: log.setformat("plain")
// Example: log.setformat("full")
Data *__log_setformat__(Data *fmt)
{
   const char *s = get_str(fmt, "log.setformat");
   if (!s)
      return createData(TYPE_NONE, NULL);

   if (strcmp(s, "plain") == 0)
      g_format = LOG_FMT_PLAIN;
   else if (strcmp(s, "standard") == 0)
      g_format = LOG_FMT_STANDARD;
   else if (strcmp(s, "full") == 0)
      g_format = LOG_FMT_FULL;
   else
   {
      throw_error(ERROR_VALUE, "log.setformat(): unknown format '%s'", s);
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_NONE, NULL);
}

// close() -> none
// Closes the log file if one is open.
// Example: log.close()
Data *__log_close__(void)
{
   if (g_file)
   {
      fclose(g_file);
      g_file = NULL;
   }
   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeLogModule(Runtime *mod_rt)
{
   UNUSED(g_show_time);
   UNUSED(g_show_level);

   for (int i = 0; i < LOG_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)LOG_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&LOG_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchLogBuiltin(LogType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case LOG_DEBUG:
      return __log_debug__(ARG(0));
   case LOG_INFO:
      return __log_info__(ARG(0));
   case LOG_WARN:
      return __log_warn__(ARG(0));
   case LOG_ERROR:
      return __log_error__(ARG(0));
   case LOG_FATAL:
      return __log_fatal__(ARG(0));
   case LOG_SETLEVEL:
      return __log_setlevel__(ARG(0));
   case LOG_SETFILE:
      return __log_setfile__(ARG(0));
   case LOG_SETFORMAT:
      return __log_setformat__(ARG(0));
   case LOG_CLOSE:
      return __log_close__();
   default:
      throw_error(ERROR_SYNTAX, "unknown log builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}