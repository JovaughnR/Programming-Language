#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./lib/args.h"

#include "../core/lib/type.h"
#include "../core/lib/error.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"

// Forward declare sys_argv access
extern int g_argc;
extern char **g_argv;

const ModuleFuncInfo ARGS_MODULE[] = {
    {MODULE_ARGS, ARGS_PARSE, "__args_parse__", 1, 1},
    {MODULE_ARGS, ARGS_GET, "__args_get__", 1, 2},
    {MODULE_ARGS, ARGS_HAS, "__args_has__", 1, 1},
    {MODULE_ARGS, ARGS_ALL, "__args_all__", 0, 0},
    {MODULE_ARGS, ARGS_USAGE, "__args_usage__", 1, 1},
};

const int ARGS_MODULE_COUNT = sizeof(ARGS_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Global Parsed State
//=========================================================

// Stored after parse() is called
static Dict *g_parsed = NULL;     // flag -> value
static List *g_positional = NULL; // positional args
static Dict *g_schema = NULL;     // flag -> {type, default, help}

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

// Normalize flag name: "--foo" -> "foo", "-f" -> "f"
static char *normalize_flag(const char *s)
{
   if (strncmp(s, "--", 2) == 0)
      return strdup(s + 2);
   if (s[0] == '-')
      return strdup(s + 1);
   return strdup(s);
}

static int is_flag(const char *s)
{
   return s && s[0] == '-';
}

//=========================================================
//  Implementations
//=========================================================

// parse(schema) -> dict
// Parses command line arguments according to a schema dict.
// Schema format: {"flag": {"type": "str|int|float|bool", "default": val, "help": "desc"}}
// Returns a dict of parsed flag -> value pairs.
//
// Supported flag formats:
//   --flag value     string/int/float flag with value
//   --flag           boolean flag (sets to true)
//   -f value         short flag
//   positional       stored in result["_"]  as a list
//
// Example schema:
//   schema = dict();
//   schema["port"]    = {"type": "int",  "default": 8080, "help": "Port number"};
//   schema["host"]    = {"type": "str",  "default": "localhost", "help": "Host"};
//   schema["verbose"] = {"type": "bool", "default": false, "help": "Verbose output"};
//   parsed = args.parse(schema);
Data *__args_parse__(Data *schema)
{
   if (!schema || schema->type != TYPE_DICT)
   {
      throw_error(ERROR_TYPE, "args.parse() argument must be a dict");
      return createData(TYPE_NONE, NULL);
   }

   // Store schema globally for usage()
   g_schema = DICT_PTR(schema);

   // Initialize result with defaults from schema
   if (g_parsed)
      dict_free(g_parsed);
   if (g_positional)
      list_free(g_positional, data_free);

   g_parsed = dict_create(__size__);
   g_positional = list_create(__size__);

   // Apply defaults first
   Dict *sch = DICT_PTR(schema);
   for (int i = 0; i < sch->size; i++)
   {
      for (Pair *p = sch->buckets[i]; p; p = p->next)
      {
         Data *flag_name = (Data *)p->key;
         Data *flag_def = (Data *)p->value;

         if (flag_def->type != TYPE_DICT)
            continue;

         Dict *def = DICT_PTR(flag_def);
         Data *def_key = createData(TYPE_STR, "default");
         Data *def_val = dict_get(def_key, def);
         data_free(def_key);

         if (def_val)
            dict_insert(cloneData(flag_name), cloneData(def_val), g_parsed);
      }
   }

   // Parse actual argv (skip argv[0]=interpreter, argv[1]=script)
   int i = 2;
   while (i < g_argc)
   {
      const char *arg = g_argv[i];

      if (is_flag(arg))
      {
         char *name = normalize_flag(arg);
         Data *key = createData(TYPE_STR, name);

         // Look up type in schema
         char *type_str = "bool";
         if (g_schema)
         {
            Data *flag_def = dict_get(key, g_schema);
            if (flag_def && flag_def->type == TYPE_DICT)
            {
               Data *type_key = createData(TYPE_STR, "type");
               Data *type_val = dict_get(type_key, DICT_PTR(flag_def));
               data_free(type_key);
               if (type_val && type_val->type == TYPE_STR)
                  type_str = type_val->str;
            }
         }

         // Boolean flag — no value needed
         if (strcmp(type_str, "bool") == 0)
         {
            // Check if next arg is "true" or "false"
            if (i + 1 < g_argc &&
                (strcmp(g_argv[i + 1], "true") == 0 ||
                 strcmp(g_argv[i + 1], "false") == 0))
            {
               int val = strcmp(g_argv[++i], "true") == 0;
               dict_insert(cloneData(key), createData(TYPE_BOOL, &val), g_parsed);
            }
            else
            {
               int val = 1;
               dict_insert(cloneData(key), createData(TYPE_BOOL, &val), g_parsed);
            }
         }
         else if (i + 1 < g_argc && !is_flag(g_argv[i + 1]))
         {
            i++;
            const char *val_str = g_argv[i];

            Data *value = NULL;
            if (strcmp(type_str, "int") == 0)
            {
               int v = atoi(val_str);
               value = createData(TYPE_INT, &v);
            }
            else if (strcmp(type_str, "float") == 0)
            {
               double v = atof(val_str);
               value = createData(TYPE_FLOAT, &v);
            }
            else
               value = createData(TYPE_STR, strdup(val_str));

            dict_insert(cloneData(key), value, g_parsed);
         }
         else
         {
            // Flag with no value — store as true
            int val = 1;
            dict_insert(cloneData(key), createData(TYPE_BOOL, &val), g_parsed);
         }

         data_free(key);
      }
      else
      {
         // Positional argument
         list_append(createData(TYPE_STR, strdup(arg)), g_positional);
      }

      i++;
   }

   // Store positionals under "_" key
   Data *pos_key = createData(TYPE_STR, "_");
   dict_insert(pos_key, createData(TYPE_LIST, g_positional), g_parsed);

   return createData(TYPE_DICT, g_parsed);
}

// get(flag, default=none) -> any
// Gets a parsed flag value by name.
// Must call args.parse() first.
// Example: port = args.get("port")
// Example: host = args.get("host", "localhost")
Data *__args_get__(Data *flag, Data *default_val)
{
   const char *name = get_str(flag, "args.get");
   if (!name)
      return createData(TYPE_NONE, NULL);

   if (!g_parsed)
   {
      throw_error(ERROR_RUNTIME, "args.get(): must call args.parse() first");
      return createData(TYPE_NONE, NULL);
   }

   Data *key = createData(TYPE_STR, (char *)name);
   Data *val = dict_get(key, g_parsed);
   data_free(key);

   if (!val)
   {
      if (default_val && default_val->type != TYPE_NONE)
         return cloneData(default_val);
      return createData(TYPE_NONE, NULL);
   }

   return cloneData(val);
}

// has(flag) -> bool
// Returns true if the flag was provided on the command line.
// Must call args.parse() first.
// Example: if (args.has("verbose")) : print("verbose mode");
Data *__args_has__(Data *flag)
{
   const char *name = get_str(flag, "args.has");
   if (!name)
      return createData(TYPE_BOOL, &(int){0});

   if (!g_parsed)
   {
      throw_error(ERROR_RUNTIME, "args.has(): must call args.parse() first");
      return createData(TYPE_BOOL, &(int){0});
   }

   Data *key = createData(TYPE_STR, (char *)name);
   Data *val = dict_get(key, g_parsed);
   data_free(key);

   int result = val != NULL;
   return createData(TYPE_BOOL, &result);
}

// all() -> dict
// Returns all parsed arguments as a dict.
// Must call args.parse() first.
// Example: print(args.all())
Data *__args_all__(void)
{
   if (!g_parsed)
   {
      throw_error(ERROR_RUNTIME, "args.all(): must call args.parse() first");
      return createData(TYPE_NONE, NULL);
   }
   return createData(TYPE_DICT, g_parsed);
}

// usage(description) -> none
// Prints a formatted help/usage message and exits.
// Uses the schema passed to parse() to build the message.
// Example: args.usage("A simple HTTP server")
Data *__args_usage__(Data *desc)
{
   const char *description = "";
   if (desc && desc->type == TYPE_STR)
      description = desc->str;

   // Program name from argv
   const char *prog = g_argc > 1 ? g_argv[1] : "script.cx";

   fprintf(stderr, "\n%s\n\n", description);
   fprintf(stderr, "Usage: %s [options]\n\n", prog);
   fprintf(stderr, "Options:\n");

   if (g_schema)
   {
      for (int i = 0; i < g_schema->size; i++)
      {
         for (Pair *p = g_schema->buckets[i]; p; p = p->next)
         {
            Data *flag_name = (Data *)p->key;
            Data *flag_def = (Data *)p->value;

            if (flag_def->type != TYPE_DICT)
               continue;

            Dict *def = DICT_PTR(flag_def);

            // Get type
            Data *type_key = createData(TYPE_STR, "type");
            Data *type_val = dict_get(type_key, def);
            data_free(type_key);
            const char *type_str = type_val && type_val->type == TYPE_STR
                                       ? type_val->str
                                       : "bool";

            // Get help
            Data *help_key = createData(TYPE_STR, "help");
            Data *help_val = dict_get(help_key, def);
            data_free(help_key);
            const char *help_str = help_val && help_val->type == TYPE_STR
                                       ? help_val->str
                                       : "";

            // Get default
            Data *def_key = createData(TYPE_STR, "default");
            Data *def_val = dict_get(def_key, def);
            data_free(def_key);

            // Format default value as string
            char def_str[64] = "none";
            if (def_val)
            {
               if (def_val->type == TYPE_INT)
                  snprintf(def_str, sizeof(def_str), "%d", *(int *)def_val->atom);
               else if (def_val->type == TYPE_FLOAT)
                  snprintf(def_str, sizeof(def_str), "%g", *(double *)def_val->real);
               else if (def_val->type == TYPE_BOOL)
                  snprintf(def_str, sizeof(def_str), "%s", *(int *)def_val->atom ? "true" : "false");
               else if (def_val->type == TYPE_STR)
                  snprintf(def_str, sizeof(def_str), "%s", def_val->str);
            }

            if (strcmp(type_str, "bool") == 0)
               fprintf(stderr, "  --%-20s %s (default: %s)\n",
                       flag_name->str, help_str, def_str);
            else
               fprintf(stderr, "  --%-15s %-6s %s (default: %s)\n",
                       flag_name->str, type_str, help_str, def_str);
         }
      }
   }

   fprintf(stderr, "  --%-20s %s\n", "help", "Show this help message");
   fprintf(stderr, "\n");
   exit(0);

   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeArgsModule(Runtime *mod_rt)
{
   for (int i = 0; i < ARGS_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)ARGS_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&ARGS_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchArgsBuiltin(ArgsType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case ARGS_PARSE:
      return __args_parse__(ARG(0));
   case ARGS_GET:
      return __args_get__(ARG(0), ARG(1));
   case ARGS_HAS:
      return __args_has__(ARG(0));
   case ARGS_ALL:
      return __args_all__();
   case ARGS_USAGE:
      return __args_usage__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown args builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}