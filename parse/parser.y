%{
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include "type.h"
   #include "list.h"
   #include "set.h"
   #include "dict.h"
   #include "utils.h"
   #include "run.h"
   #include "eval.h"
   #include "exe.h"
   #include "error.h"
   #include "run.h"
   #include "maloc.h"
   #include "daloc.h"
   #include "debug.h"
   #include "file.h"

   extern int yylex(void);
   void yyerror(const char *s);
   void executeProgram(List *statements);
   extern int yylineno;
   extern char *yytext;

   // Global declarations
   Runtime *rt;
   static int funcDepth = 0;
   static int loopDepth = 0;
   List *global_statements = NULL;
%}

%union {
   void *generic;
}

%start program
%expect 0

%token INDENT DEDENT FROM PASS CLASS IMPORT AS
%token FUNCTION NEWLINE RETURN CONTINUE BREAK
%token WHILE FOR IN IF ELIF ELSE GLOBAL NONLOCAL

%token <generic> VARIABLE STRING INTEGER DECIMAL
%token <generic> ADD SUB MUL DIV MOD FLDIV EXP
%token <generic> BITAND BITOR BITXOR L_SHIFT R_SHIFT
%token <generic> PLUS_EQ MINUS_EQ DIV_EQ MUL_EQ MOD_EQ L_SHIFT_EQ R_SHIFT_EQ
%token <generic> XOR_EQ FLDIV_EQ EXP_EQ AND_EQ OR_EQ
%token <generic> TRUE FALSE NONE
%token <generic> LOGIC_AND LOGIC_OR INVERT
%token <generic> EQ NE LT GT GE LE 

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
%right INVERT
%left '(' '[' 

%type <generic> assignment value values
%type <generic> boolean postfix term equals
%type <generic> list dict set class 
%type <generic> function dict_items kwargs kwarg
%type <generic> statement simple_stmt compound_stmt
%type <generic> param_list param_item args pair key
%type <generic> while_stmt if_stmt elif_chain else_block for_stmt
%type <generic> program stmt_list left_side
%type <generic> import module import_items import_name

%%

program
   : /* empty */                            { executeProgram(createList(0)); }
   | stmt_list                              { executeProgram($1); }
   ;


stmt_list
   : statement                               { $$ = createList(RUNTIME_SIZE); if ($1) append((Stmt *)$1, $$); }
   | stmt_list statement                     { if ($2) append((Stmt *)$2, $1); $$ = $1;  }
   ;

statement
   : simple_stmt                                { $$ = $1; }
   | compound_stmt                              { $$ = $1; }
   | NEWLINE                                    { $$ = NULL; }
   | PASS NEWLINE                               { $$ = NULL; }
   ;

simple_stmt
   : assignment NEWLINE                     { $$ = createStatement(STMT_ASSIGNMENT, $1, yylineno); }
   | value NEWLINE                          { $$ = createStatement(STMT_EXPRESSION, $1, yylineno); }
   | GLOBAL VARIABLE NEWLINE                { $$ = createStatement(STMT_GLOBAL, createVarDecl(DECL_GLOBAL, $2), yylineno); }
   | NONLOCAL VARIABLE NEWLINE              { $$ = createStatement(STMT_NONLOCAL, createVarDecl(DECL_NONLOCAL, $2), yylineno); }
   | RETURN value NEWLINE                   { $$ = createStatement(STMT_RETURN, createReturn($2, funcDepth), yylineno); }
   | RETURN NEWLINE                         { $$ = createStatement(STMT_RETURN, createReturn(NULL, funcDepth), yylineno); }
   | CONTINUE NEWLINE                       { $$ = createStatement(STMT_CONTINUE, createJump(loopDepth, 0), yylineno); }
   | BREAK NEWLINE                          { $$ = createStatement(STMT_CONTINUE, createJump(loopDepth, 1), yylineno); }
   ;

compound_stmt
   : function                               { $$ = createStatement(STMT_FUNCTION, $1, yylineno); }
   | class                                  { $$ = createStatement(STMT_CLASS, $1, yylineno); }
   | if_stmt                                { $$ = createStatement(STMT_FLOW, $1, yylineno); }
   | while_stmt                             { $$ = createStatement(STMT_WHILE, $1, yylineno); }
   | for_stmt                               { $$ = createStatement(STMT_FOR, $1, yylineno); }
   | import                                 { $$ = createStatement(STMT_IMPORT, $1, yylineno); }
   ;


import
   : IMPORT module                                    { $$ = createImport($2, NULL, NULL, 0); }
   | IMPORT module AS VARIABLE                        { $$ = createImport($2, NULL, $4, 0); }
   | FROM module IMPORT import_items                  { $$ = createImport($2, $4, NULL, 0); }
   | FROM module IMPORT MUL                           { $$ = createImport($2, NULL, NULL, 1); freeAST($4); }
   ;

module
   : VARIABLE                                         { $$ = $1; }
   | module '.' VARIABLE                              { $$ = createData(TYPE_STR, getFileName($1, $3)); }
   ;

import_items
   : import_name                                     { $$ = createList(PARSE_SIZE); append($1, $$); }
   | import_items ',' import_name                    { append($3, $1); $$ = $1; }
   ;

import_name
   : VARIABLE                                        { $$ = createASTnode(createData(TYPE_LOOKUP, $1)); }
   | import_name '.' VARIABLE                        { $$ = createASTnode(createData(TYPE_ATTRIBUTE, attribute_create($1, $3))); }
   ;

class
   : CLASS VARIABLE ':' NEWLINE INDENT stmt_list DEDENT                      {  $$ = createClass($2, $6, NULL); }
   | CLASS VARIABLE '(' args ')' ':' NEWLINE INDENT stmt_list DEDENT    { $$ = createClass($2, $9, $4);}
   ;
   
assignment
   : left_side '=' values                  { $$ = createAssignment($1, $3, NULL); }
   | left_side equals values               { $$ = createAssignment($1, $3, $2); }
   ;

left_side 
   : postfix                              { $$ = createList(PARSE_SIZE); append($1, $$); }
   | left_side ',' postfix                { append($3, $1); $$ = $1; }
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
   | SUB value %prec INVERT                 
   {
      int zero = 0; 
      Instance *inst = createInstance(TYPE_INT, &zero);
      Ast *left = createASTnode(createData(TYPE_INSTANCE, inst));
      $$ = createASTexpr($1, left, $2);
   }
   ;

postfix
   : term                                         { $$ = $1; }    
   | postfix '.' VARIABLE                          { $$ = createASTnode(createData(TYPE_ATTRIBUTE, attribute_create($1, $3))); }
   | postfix '(' args ')'                          { $$ = createASTnode(createData(TYPE_INVOKED, createInvoked($1, $3, NULL))); }
   | postfix '(' args ',' kwargs ')'               { $$ = createASTnode(createData(TYPE_INVOKED, createInvoked($1, $3, $5))); }
   | postfix '[' value ']'                         { $$ = createASTnode(createData(TYPE_INDEX, createIndexed($1, $3))); }
   ;

term
   : INTEGER                                { $$ = $1; }
   | DECIMAL                                { $$ = $1; }
   | STRING                                 { $$ = $1; }
   | boolean                                { $$ = $1; }
   | VARIABLE                               { $$ = createASTnode(createData(TYPE_LOOKUP, $1)); } 
   | list                                   { $$ = createASTnode(createData(TYPE_INSTANCE, $1)); }
   | set                                    { $$ = createASTnode(createData(TYPE_INSTANCE, $1)); }
   | dict                                   { $$ = createASTnode(createData(TYPE_INSTANCE, $1)); }
   | NONE                                   { $$ = createASTnode(createData(TYPE_INSTANCE, $1)); }
   ;

args 
   : /* No args */                          { $$ = createList(0); }
   | value                                  { $$ = createList(PARSE_SIZE); append($1, $$); }
   | args ',' value                         { append($3, $1); $$ = $1; }   
   ; 

boolean
   : TRUE                                   { $$ = $1; }
   | FALSE                                  { $$ = $1; }
   ;

list
   : '[' ']'                                { $$ = createInstance(list_create(PARSE_SIZE)); }
   | '[' values ']'                         { $$ = createInstance(TYPE_LIST, $2); }
   ;

values 
   : value                                  { $$ = createList(PARSE_SIZE); append($1, $$); }
   | values ',' value                       { append($3, $1); $$ = $1; }  
   ;

set 
   : '{' values '}'                         { $$ = createInstance(TYPE_SET, $2); }
   ;

dict
   : '{' '}'                                { $$ = createInstance(TYPE_DICT, list_create(PARSE_SIZE)); }
   | '{' dict_items '}'                     { $$ = createInstane(TYPE_DICT, $2); }
   ;

dict_items
   : pair                                   { $$ = list_create(PARSE_SIZE); list_append($1, $$); }
   | dict_items ',' pair                    { append($3, $1); $$ = $1; }
   ;

kwargs
   : kwarg                                  { $$ = list_create(PARSE_SIZE); list_append($1, $$); }
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
   : FUNCTION VARIABLE '(' param_list ')' ':' NEWLINE 
     { funcDepth++; }
     INDENT stmt_list DEDENT
     { 
        funcDepth = funcDepth > 0 ? funcDepth - 1 : 0;
        $$ = createFunction($2, $4, $10);
     }
   ;


param_list                   
   : /* Empty */                            { $$ = createList(0); }
   | param_item                             { $$ = createList(PARSE_SIZE); append($1, $$); }
   | param_list ',' param_item              { append($3, $1); $$ = $1; }
   ;

param_item
   : VARIABLE                               { $$ = createParamInfo($1, NULL); }
   | VARIABLE '=' value                     { $$ = createParamInfo($1, $3); }
   ;

while_stmt
   : WHILE '(' value ')' ':' NEWLINE        { loopDepth++; }
     INDENT stmt_list DEDENT
     {
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        $$ = createWhileLoop($3, $9);
     }
   ;

if_stmt
   : IF value ':' NEWLINE INDENT stmt_list DEDENT elif_chain else_block   
    { 
      $$ = createFlow($2, $6, $8, $9); 
    }
   ;

elif_chain   
   : /* empty */                                                      { $$ = NULL; }
   | ELIF value ':' NEWLINE INDENT stmt_list DEDENT elif_chain        
    { 
      $$ = createFlow($2, $6, $8, NULL); 
    }
   ;

else_block
   : /* empty */                                 { $$ = NULL; }
   | ELSE ':' NEWLINE INDENT stmt_list DEDENT    { $$ = createDefaultFlow($5); }
   ;

for_stmt
   : FOR VARIABLE IN postfix ':' NEWLINE 
     { 
      loopDepth++; 
     }
     INDENT stmt_list DEDENT
     { 
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        $$ = createForLoop($2, $4, $9); 
     }
   ;

%%

int main(int argc, char **argv) {
   fprintf(stderr, "=== STARTING MAIN ===\n");
   // Create runtime environment
   rt = createRuntime();
   
   // Initialize built-in functions
   initializeBuiltins(rt);

   // Check if reading from file or interactive
   int isInteractive = isatty(fileno(stdin));
   
   if (isInteractive) {
      printf("Custom Language Interpreter\n");
      printf("===========================\n");
      printf("Ready. Type code and press Enter.\n");
      printf("Press Ctrl+D to exit.\n\n");
   }
   
   // Parse the input
   int result = yyparse();
   
   // Cleanup
   if (rt) {
      freeRuntime(rt);
   }
   
   if (global_statements) {
      freeList(global_statements, TYPE_STATEMENT);
   }
   
   return result;
}

void yyerror(const char *s) 
{
   fprintf(stderr, "DEBUG: yyerror called at line %d\n", yylineno);
   fprintf(stderr, "DEBUG: yytext = '%s' (length: %zu)\n", yytext, strlen(yytext));
   fprintf(stderr, "DEBUG: Error message: %s\n", s);
   
   // Print hex values of yytext to see hidden characters
   fprintf(stderr, "DEBUG: yytext hex bytes: ");
   for (int i = 0; yytext[i] != '\0'; i++) {
      fprintf(stderr, "%02x ", (unsigned char)yytext[i]);
   }
   fprintf(stderr, "\n");
   
   throw_error(ERROR_SYNTAX, "@ Line %d: near '%s'", yylineno, yytext);
   fprintf(stderr, "Parse error: %s at line %d near '%s'\n", s, yylineno, yytext);
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
      freeData((Data *)returns);
   }

   printf("\n%s....Program execution completed.....\n", COLOR_GREEN);
}