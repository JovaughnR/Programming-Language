#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./parse/y.tab.h"
#include "./parse/parse.h"

#include "./core/lib/type.h"
#include "./core/lib/list.h"
#include "./core/lib/error.h"
#include "./core/lib/module.h"
#include "./core/lib/maloc.h"
#include "./core/lib/daloc.h"
#include "./core/lib/exec.h"
#include "./core/lib/format.h"

#include "./builtin/lib/build.h"
#include "./builtin/lib/methods.h"
#include "./builtin/lib/maths.h"
#include "./builtin/lib/time.h"
#include "./builtin/lib/os.h"
#include "./builtin/lib/io.h"
#include "./builtin/lib/random.h"
#include "./builtin/lib/json.h"
#include "./builtin/lib/sys.h"
#include "./builtin/lib/path.h"
#include "./builtin/lib/regex.h"
#include "./builtin/lib/hash.h"
#include "./builtin/lib/csv.h"
#include "./builtin/lib/stats.h"
#include "./builtin/lib/decimal.h"
#include "./builtin/lib/env.h"
#include "./builtin/lib/log.h"
#include "./builtin/lib/args.h"
#include "./builtin/lib/net.h"
#include "./builtin/lib/socket.h"
#include "./builtin/lib/db.h"
#include "./builtin/lib/thread.h"
#include "./builtin/lib/async.h"
#include "./builtin/lib/http.h"

extern YY_BUFFER_STATE yy_scan_string(const char *);
extern void yy_delete_buffer(YY_BUFFER_STATE);

extern int yylex(void);
extern int yylineno;
extern char *yytext;
extern int yychar;
void yyerror(const char *s);

// Declared in parser.y
extern List *global_statements;

// Globals
ThrownError g_thrown;
FILE *g_input_stream = NULL;
int repl_mode = 0;
Method *builtin;
Runtime *rt;

// ---------------------------------------------------------------------------
// Module registry table
// ---------------------------------------------------------------------------

static const struct
{
   const char *module;
   void (*initializer)(Runtime *);
} modules[] = {
    {"math", initializeMathModule},
    {"time", initializeTimeModule},
    {"os", initializeOsModule},
    {"io", initializeIoModule},
    {"random", initializeRandomModule},
    {"json", initializeJsonModule},
    {"sys", initializeSysModule},
    {"path", initializePathModule},
    {"regex", initializeReModule},
    {"hash", initializeHashModule},
    {"csv", initializeCsvModule},
    {"stats", initializeStatsModule},
    {"decimal", initializeDecimalModule},
    {"env", initializeEnvModule},
    {"log", initializeLogModule},
    {"args", initializeArgsModule},
    {"net", initializeNetModule},
    {"socket", initializeSocketModule},
    {"db", initializeDbModule},
    {"thread", initializeThreadModule},
    {"http", initializeHttpModule},
};

#define MODULE_COUNT (int)(sizeof(modules) / sizeof(modules[0]))

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static const char *get_token(char *text)
{
   if (text && text[0] != '\0')
      return text;

   return token_name(yychar);
}

void yyerror(const char *s)
{
   (void)s; // we build our own message
   g_thrown.lineno = yylineno;

   const char *token = get_token(yytext);

   // Try to give a more specific message based on what we saw
   if (yychar == 0)
      throw_error(ERROR_SYNTAX, " unexpected end of file");

   else if (yychar == NL || yychar == ';')
      throw_error(
          ERROR_SYNTAX,
          " incomplete statement before %s",
          token_name(yychar));

   else if (yychar == RBRACE)
      throw_error(
          ERROR_SYNTAX,
          "unexpected '}' — missing opening '{'?");

   else if (yychar == LBRACE)
      throw_error(
          ERROR_SYNTAX,
          "unexpected '{' — missing condition or expression?");

   else if (yychar == ELSE || yychar == ELIF)
      throw_error(
          ERROR_SYNTAX,
          "'%s' without matching 'if'", token);

   else if (yychar == CATCH || yychar == FINALLY)
      throw_error(
          ERROR_SYNTAX,
          "'%s' without matching 'try'", token);
   else
      throw_error(
          ERROR_SYNTAX,
          "unexpected %s '%s'", token_name(yychar), token);
}

static void executeProgram(List *stmts)
{
   if (!stmts || !rt)
      return;

   fflush(stdout);

   void *returns = NULL;
   Status status = executeBody(stmts, &returns, rt);

   fflush(stdout);

   if (status == FLOW_RETURN && returns)
      data_free((Data *)returns);
}

static void register_modules(Runtime *rt)
{
   if (!rt->registry)
      rt->registry = registry_create();

   for (int i = 0; i < MODULE_COUNT; i++)
   {
      const char *module = modules[i].module;
      register_native(rt->registry, (char *)module, modules[i].initializer);
   }
}

// ---------------------------------------------------------------------------
// Execution modes
// ---------------------------------------------------------------------------

static int executeFile(int argc, char **argv)
{
   extern FILE *yyin;

   FILE *file = fopen(argv[1], "r");
   if (!file)
   {
      throw_error(ERROR_RUNTIME, "cannot open file '%s'", argv[1]);
      return 1;
   }

   yyin = file;
   int result = yyparse();
   fclose(file);

   if (result == 0 && global_statements)
      executeProgram(global_statements);

   return 0;
}

static int executeRepl(void)
{
   repl_mode = 1;
   printf("cxlang repl version 1.0.0\n");
   printf("Type 'exit' or Press Ctrl+D to quit.\n\n");

   char line[4096];

   while (1)
   {
      printf(">>> ");
      fflush(stdout);

      if (!fgets(line, sizeof(line), stdin))
         break;

      int len = strlen(line);
      if (len > 0 && line[len - 1] == '\n')
         line[--len] = '\0';

      if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0)
         break;
      if (len == 0)
         continue;

      // Always build buf — don't leave it uninitialized
      char buf[4096 + 4];
      snprintf(buf, sizeof(buf), "%s\n", line);

      if (global_statements)
      {
         list_free(global_statements, statement_free);
         global_statements = NULL;
      }

      YY_BUFFER_STATE lex_buf = yy_scan_string(buf);
      yy_switch_to_buffer(lex_buf);
      int result = yyparse();
      yy_delete_buffer(lex_buf);

      if (result != 0 || !global_statements)
      {
         clear_error();
         continue;
      }

      Statement *stmt = global_statements->items[global_statements->end];
      Data *returns = NULL;

      executeStatement(stmt, (void **)&returns, rt);
      if (stmt->type == STMT_EXPR && returns && returns->type != TYPE_NONE)
      {
         List *args = list_create(1);
         list_append(cloneData(returns), args);
         builtin_print(args, NULL);
         data_free(returns);
      }
   }

   repl_mode = 0;
   return 0;
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

int run(int argc, char **argv)
{
   sys_init(argc, argv);
   builtin = createBuiltins();
   rt = createRuntime();

   builtin_init(rt);
   register_modules(rt);
   async_init(4);

   int exit_code = (argc >= 2) ? executeFile(argc, argv) : executeRepl();

   methods_free(builtin);
   runtime_free(rt);
   async_shutdown();
   return exit_code;
}