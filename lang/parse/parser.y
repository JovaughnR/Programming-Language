%{
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>

   #include "../lib/type.h"
   #include "../lib/list.h"
   #include "../lib/set.h"
   #include "../lib/dict.h"
   #include "../lib/utils.h"
   #include "../lib/str.h"
   #include "../lib/eval.h"
   #include "../lib/exec.h"
   #include "../lib/error.h"
   #include "../lib/module.h"
   #include "../lib/maloc.h"
   #include "../lib/daloc.h"
   #include "../lib/build.h"
   #include "../lib/error.h"


   extern int yylex(void);
   void yyerror(const char *s);
   void executeProgram(List *statements);
   extern int yylineno;
   extern char *yytext;


   // Global declarations
   Runtime *rt;
   Method *builtins;

   static int funcDepth = 0;
   static int loopDepth = 0;
   List *global_statements = NULL;

   ThrownError g_thrown;
   FILE *g_input_stream = NULL;
   int repl_mode = 0;
%}

%union {
   void *generic;
}

%start program
%expect 0

%token INDENT DEDENT FROM PASS CLASS IMPORT AS
%token FUNCTION NL RETURN CONTINUE BREAK TRY CATCH FINALLY
%token WHILE FOR IF ELIF ELSE GLOBAL NONLOCAL

%token <generic> VARIABLE STRING INTEGER DECIMAL
%token <generic> ADD SUB MUL DIV MOD FLDIV EXP
%token <generic> BITAND BITOR BITXOR L_SHIFT R_SHIFT BITNOT
%token <generic> PLUS_EQ MINUS_EQ DIV_EQ MUL_EQ MOD_EQ L_SHIFT_EQ R_SHIFT_EQ
%token <generic> XOR_EQ FLDIV_EQ EXP_EQ AND_EQ OR_EQ
%token <generic> TRUE FALSE NONE
%token <generic> LOGIC_AND LOGIC_OR INVERT
%token <generic> EQ NE LT GT GE LE IN

/* Precedence from lowest to highest */
%left LOGIC_OR 
%left LOGIC_AND
%left BITOR
%left BITXOR
%left BITAND
%nonassoc EQ NE LT GT LE GE
%left L_SHIFT R_SHIFT
%left ADD SUB
%left MUL DIV MOD FLDIV
%right EXP
%right INVERT BITNOT
%left '(' '[' 

%type <generic> values slice_start slice_end sliced_items
%type <generic> boolean postfix term equals value 
%type <generic> list dict set class assignment 
%type <generic> function dict_items kwargs kwarg
%type <generic> statement simple_stmt compound_stmt
%type <generic> param_list param_item args pair key
%type <generic> while_stmt if_stmt elif_chain else_block 
%type <generic> program stmt_list left_side for_stmt
%type <generic> import module import_items import_name
%type <generic> exception try_block catch_block final_block

%%

program
   : /* empty */                            { global_statements = list_create(0); }
   | stmt_list                              { global_statements = $1; }
   ;

stmt_list
   : statement                               { $$ = list_create(__len__); list_append($1, $$); }
   | stmt_list statement                     { list_append($2, $1); $$ = $1;  }
   ;

statement
   : simple_stmt                                { $$ = $1; }
   | compound_stmt                              { $$ = $1; }
   | NL                                         { $$ = NULL; }
   | PASS NL                                    { $$ = NULL; }
   ; 

simple_stmt
   : assignment NL                     { $$ = createStatement(STMT_ASSIGNMENT, $1, yylineno - 1); }
   | value NL                          { $$ = createStatement(STMT_EXPRESSION, $1, yylineno - 1); }
   | GLOBAL VARIABLE NL                { $$ = createStatement(STMT_GLOBAL, $2, yylineno - 1); }
   | NONLOCAL VARIABLE NL              { $$ = createStatement(STMT_NONLOCAL, $2, yylineno); }
   | RETURN value NL                   { $$ = createStatement(STMT_RETURN, createReturn($2, funcDepth), yylineno - 1); }
   | RETURN NL                         { $$ = createStatement(STMT_RETURN, createReturn(NULL, funcDepth), yylineno - 1); }
   | CONTINUE NL                       { $$ = createStatement(STMT_CONTINUE, createJump(loopDepth, 0), yylineno - 1); }
   | BREAK NL                          { $$ = createStatement(STMT_CONTINUE, createJump(loopDepth, 1), yylineno - 1); }
   ;

compound_stmt
   : { $<generic>$ = (void*)(long)yylineno; } function    { $$ = createStatement(STMT_FUNCTION, $2, (int)(long)$<generic>1); }
   | { $<generic>$ = (void*)(long)yylineno; } class       { $$ = createStatement(STMT_CLASS, $2, (int)(long)$<generic>1); }
   | { $<generic>$ = (void*)(long)yylineno; } if_stmt     { $$ = createStatement(STMT_FLOW, $2, (int)(long)$<generic>1); }
   | { $<generic>$ = (void*)(long)yylineno; } while_stmt  { $$ = createStatement(STMT_WHILE, $2, (int)(long)$<generic>1); }
   | { $<generic>$ = (void*)(long)yylineno; } for_stmt    { $$ = createStatement(STMT_FOR, $2, (int)(long)$<generic>1); }
   | { $<generic>$ = (void*)(long)yylineno; } import      { $$ = createStatement(STMT_IMPORT, $2, (int)(long)$<generic>1); }
   | { $<generic>$ = (void*)(long)yylineno; } exception   { $$ = createStatement(STMT_EXCEPTION, $2, (int)(long)$<generic>1); }
   ;

import
   : IMPORT module                                    { $$ = createImport($2, NULL, NULL, 0); }
   | IMPORT module AS VARIABLE                        { $$ = createImport($2, NULL, $4, 0); }
   | FROM module IMPORT import_items                  { $$ = createImport($2, $4, NULL, 0); }
   | FROM module IMPORT MUL                           { $$ = createImport($2, NULL, NULL, 1); ast_free($4); }
   ;

module
   : VARIABLE                                         { $$ = $1; }
   | module '.' VARIABLE                              { $$ = str_concat(str_concat($1, "."), $3); }
   ;

import_items
   : import_name                                     { $$ = list_create(__len__); list_append($1, $$); }
   | import_items ',' import_name                    { list_append($3, $1); $$ = $1; }
   ;

import_name
   : VARIABLE                                        { $$ = createASTnode(createData(TYPE_LOOKUP, $1)); }
   | import_name '.' VARIABLE                        { $$ = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute($1, $3))); }
   ;

class
   : CLASS VARIABLE ':' NL INDENT stmt_list DEDENT                      {  $$ = createClass($2, $6, NULL); }
   | CLASS VARIABLE '(' args ')' ':' NL INDENT stmt_list DEDENT         { $$ = createClass($2, $9, $4);}
   ;
   
assignment
   : left_side '=' values                  { $$ = createAssignment($1, $3, NULL); }
   | left_side equals values               { $$ = createAssignment($1, $3, $2); }
   ;

left_side 
   : postfix                              { $$ = list_create(__len__); list_append($1, $$); }
   | left_side ',' postfix                { list_append($3, $1); $$ = $1; }
   ;


equals
   :  PLUS_EQ | MINUS_EQ | DIV_EQ | MUL_EQ | FLDIV_EQ | EXP_EQ | MOD_EQ 
   |  R_SHIFT_EQ | L_SHIFT_EQ | XOR_EQ | AND_EQ | OR_EQ
   ;

value
   : postfix                                 { $$ = $1; }
   | '(' value ')'                          { $$ = $2; }
   | value ADD value                        { $$ = createASTexpr($2, $1, $3); }
   | value SUB value                        { $$ = createASTexpr($2, $1, $3); }
   | value MUL value                        { $$ = createASTexpr($2, $1, $3); }
   | value DIV value                        { $$ = createASTexpr($2, $1, $3); }
   | value MOD value                        { $$ = createASTexpr($2, $1, $3); }
   | value FLDIV value                      { $$ = createASTexpr($2, $1, $3); }
   | value EXP value                        { $$ = createASTexpr($2, $1, $3); }
   | value BITXOR value                     { $$ = createASTexpr($2, $1, $3); }
   | value BITOR value                      { $$ = createASTexpr($2, $1, $3); }
   | value BITAND value                     { $$ = createASTexpr($2, $1, $3); }
   | value L_SHIFT value                    { $$ = createASTexpr($2, $1, $3); }
   | value R_SHIFT value                    { $$ = createASTexpr($2, $1, $3); }
   | value LOGIC_AND value                  { $$ = createASTexpr($2, $1, $3); }
   | value LOGIC_OR value                   { $$ = createASTexpr($2, $1, $3); }
   | value LE value                         { $$ = createASTexpr($2, $1, $3); }
   | value GE value                         { $$ = createASTexpr($2, $1, $3); }
   | value NE value                         { $$ = createASTexpr($2, $1, $3); }
   | value GT value                         { $$ = createASTexpr($2, $1, $3); }
   | value LT value                         { $$ = createASTexpr($2, $1, $3); }
   | value EQ value                         { $$ = createASTexpr($2, $1, $3); }
   | INVERT value                           { $$ = createASTexpr($1, NULL, $2); }
   | BITNOT value                           { $$ = createASTexpr($1, NULL, $2); }
   | SUB value %prec INVERT                 
   {
      ASTnode *left = createASTnode(createData(TYPE_INT, &(int){0}));
      $$ = createASTexpr($1, left, $2);
   }
   | ADD value %prec INVERT                 
   {
      ASTnode *left = createASTnode(createData(TYPE_INT, &(int){0}));
      $$ = createASTexpr($1, left, $2);
   }
   ;

postfix
   : term                                         { $$ = $1; }    
   | postfix '(' args ')'                          { $$ = createASTnode(createData(TYPE_INVOKED, createInvoked($1, $3, NULL))); }
   | postfix '(' args ',' kwargs ')'               { $$ = createASTnode(createData(TYPE_INVOKED, createInvoked($1, $3, $5))); }
   | postfix '.' VARIABLE                          { $$ = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute($1, $3))); }
   | postfix '[' value ']'                         { $$ = createASTnode(createData(TYPE_INDEX, createIndexed($1, $3))); }
   | postfix sliced_items                          { $$ = createASTnode(createData(TYPE_SLICE, createIndexed($1, $2))); }
   ;

term
   : INTEGER                                { $$ = $1; }
   | DECIMAL                                { $$ = $1; }
   | STRING                                 { $$ = $1; }
   | boolean                                { $$ = $1; }
   | list                                   { $$ = createASTnode($1); }
   | VARIABLE                               { $$ = createASTnode(createData(TYPE_LOOKUP, $1)); } 
   | set                                    { $$ = createASTnode(createData(TYPE_SET, $1)); }
   | dict                                   { $$ = createASTnode(createData(TYPE_DICT, $1)); }
   | NONE                                   { $$ = NULL; }
   ;

args 
   : /* No args */                          { $$ = list_create(0); }
   | value                                  { $$ = list_create(__len__); list_append($1, $$); }
   | args ',' value                         { list_append($3, $1); $$ = $1; }   
   ; 


boolean
   : TRUE                                               { $$ = $1; }
   | FALSE                                              { $$ = $1; }
   ;

list
   : '[' ']'                                            { $$ = createData(TYPE_LIST ,list_create(__len__)); }
   | '[' values ']'                                     { $$ = createData(TYPE_LIST, $2); }
   ;

sliced_items
   : '[' slice_start ':' slice_end ':' slice_end ']'    { $$ = createSlice($2, $4, $6); }
   | '[' slice_start ':' slice_end ']'                  { $$ = createSlice($2, $4, NULL); }
   ;

slice_start
   : /* empty */                                        { $$ = NULL; }
   | value                                              { $$ = $1; }
   ;

slice_end
   : /* empty */                                        { $$ = NULL; }
   | value                                              { $$ = $1; }
   ;


values 
   : value                                              { $$ = list_create(__len__); list_append($1, $$); }
   | values ',' value                                   { list_append($3, $1); $$ = $1; }  
   ;

set 
   : '{' values '}'                                    { $$ = $2; }
   ;

dict
   : '{' '}'                                           { $$ = list_create(__len__); }
   | '{' dict_items '}'                                { $$ = $2; }
   ;

dict_items
   : pair                                   { $$ = list_create(__len__); list_append($1, $$); }
   | dict_items ',' pair                    { list_append($3, $1); $$ = $1; }
   ;

kwargs
   : kwarg                                  { $$ = list_create(__len__); list_append($1, $$); }
   | kwargs ',' kwarg                       { list_append($3, $1); $$ = $1; }
   ;

kwarg  
   : VARIABLE '=' value                     { $$ = createPair(createData(TYPE_STR, $1), $3); }
   ;

pair
   : key ':' value                          { $$ = createPair($1, $3); }
   ;

key
   : STRING                                 { $$ = $1; }
   | INTEGER                                { $$ = $1; }
   | DECIMAL                                { $$ = $1; }
   | boolean                                { $$ = $1; }
   ;


function
   : FUNCTION VARIABLE '(' param_list ')' ':' NL 
     { funcDepth++; }
     INDENT stmt_list DEDENT
     { 
        funcDepth = funcDepth > 0 ? funcDepth - 1 : 0;
        $$ = createFunction($2, $4, $10);
     }
   ;


param_list                   
   : /* Empty */                            { $$ = list_create(0); }
   | param_item                             { $$ = list_create(__len__); list_append($1, $$); }
   | param_list ',' param_item              { list_append($3, $1); $$ = $1; }
   ;

param_item
   : VARIABLE                               { $$ = createParams($1, NULL); }
   | VARIABLE '=' value                     { $$ = createParams($1, $3); }
   ;

while_stmt
   : WHILE '(' value ')' ':' NL        { loopDepth++; }
     INDENT stmt_list DEDENT
     {
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        $$ = createWhile($3, $9);
     }
   ;

if_stmt
   : IF value ':' NL INDENT stmt_list DEDENT elif_chain else_block   
    { 
      $$ = createFlow($2, $6, $8, $9); 
    }
   ;

elif_chain   
   : /* empty */                                                      { $$ = NULL; }
   | ELIF value ':' NL INDENT stmt_list DEDENT elif_chain        
    { 
      $$ = createFlow($2, $6, $8, NULL); 
    }
   ;

else_block
   : /* empty */                                 { $$ = NULL; }
   | ELSE ':' NL INDENT stmt_list DEDENT         { $$ = createFlow(NULL, $5, NULL, NULL); }
   ;

for_stmt
   : FOR VARIABLE IN postfix ':' NL 
     { 
      loopDepth++; 
     }
     INDENT stmt_list DEDENT
     { 
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        ast_free($3);
        $$ = createFor($2, $4, $9); 
     }
   ;

exception
   : try_block catch_block                                 { $$ = createException($1, $2, NULL); }
   | try_block final_block                                  { $$ = createException($1, $2, NULL);  }
   | try_block catch_block final_block                      { $$ = createException($1, $2, $3);  }
   ;

try_block
   : TRY ':' NL INDENT stmt_list DEDENT                              { $$ = $5; }
   ;

catch_block
   : CATCH VARIABLE ':' NL INDENT stmt_list DEDENT                   { $$ = createCatch($2, NULL, $6); }
   | CATCH ':' NL INDENT stmt_list DEDENT                            { $$ = createCatch(NULL, NULL, $5); }    
   | CATCH VARIABLE AS VARIABLE ':' NL INDENT stmt_list DEDENT       { $$ = createCatch($2, $4, $8); }
   ;

final_block
   : FINALLY ':' NL INDENT stmt_list DEDENT                          { $$ = $5; }
   ;

%%

int main(int argc, char **argv) {
    builtins = createBuiltins();
    rt = createRuntime();
    initializeBuiltins(rt);

    extern FILE *yyin;

    if (argc >= 2) {
        // ── File mode ─────────────────────────────────────────
        // stdin is free, so input() works normally from the terminal
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
            return 1;
        }

        yyin = file;
        int result = yyparse();
        fclose(file);

        if (result == 0 && global_statements)
            executeProgram(global_statements);

    } else {
        // ── REPL mode ─────────────────────────────────────────
        // stdin is consumed by the parser, so input() must use /dev/tty
        repl_mode = 1;
        yyin = stdin;

        printf("Custom Language Interpreter\n");
        printf("===========================\n");
        printf("Type code and press Enter. Press Ctrl+D to exit.\n\n");

        g_input_stream = fopen("/dev/tty", "r");

        int result = yyparse();

        if (result == 0 && global_statements)
            executeProgram(global_statements);

        if (g_input_stream) {
            fclose(g_input_stream);
            g_input_stream = NULL;
        }
    }

    if (rt)
        runtime_free(rt);
    if (builtins)
        methods_free(builtins);

    return 0;
}


void yyerror(const char *s) 
{
   g_thrown.lineno = yylineno;
   throw_error(ERROR_SYNTAX, "near '%s'", yytext);
}


void executeProgram(List *statements) {
   if (!statements || !rt) 
      return;

   fflush(stdout);  // ADD THIS
   global_statements = statements;
   
   void *returns = NULL;
   Status status = executeBody(statements, &returns, rt);
   
   fflush(stdout);  // ADD THIS
   if (status == FLOW_RETURN && returns) {
      data_free((Data *)returns);
   }
}