%{
   #include <stdlib.h>

   #include "../main.h"
   
   // Include Core types
   #include "../core/lib/type.h"
   #include "../core/lib/list.h"
   #include "../core/lib/str.h"
   #include "../core/lib/maloc.h"
   #include "../core/lib/daloc.h"

   extern int  yylex(void);
   extern int  yylineno;
   extern char *yytext;
   extern int  yylineno;
   extern int  yychar;   // current lookahead token (set by yacc)
   extern void yyerror(const char *s);


   // Global declarations
   List *global_statements;

   // Function Depth
   static int f_depth = 0; 
   // Loop Depth
   static int l_depth = 0; 
   // Line Number
   static int line = -1;

%}

%union {
   void *generic;
}

%start program
%expect 0

%token IMPORT AS LAMBDA QMARK
%token FUNC NL RETURN THROW
%token TRY CATCH FINALLY
%token WHILE FOR CONTINUE BREAK 
%token IF ELIF ELSE 
%token GLOBAL NONLOCAL ENUM
%token LBRACE RBRACE FROM CLASS 
%token ASYNC AWAIT

%token <generic> IDENT STRING INTEGER 
%token <generic> DECIMAL LSHIFT RSHIFT
%token <generic> PLUS MINUS STAR DIVIDE
%token <generic> MODULO FLOOR EXPONENT
%token <generic> BITAND BITOR BITXOR 
%token <generic> MINUS_EQ DIV_EQ 
%token <generic> BITNOT MUL_EQ MOD_EQ 
%token <generic> LE IN EXP_EQ FLDIV_EQ
%token <generic> R_SHIFT_EQ XOR_EQ NONE
%token <generic> AND_EQ OR_EQ TRUE FALSE 
%token <generic> LAND PLUS_EQ  INVERT
%token <generic> EQ NE LT GT GE LOR
%token <generic> FSTRING L_SHIFT_EQ

/* Precedence from lowest to highest */
%left LOR 
%left LAND
%left BITOR
%left BITXOR
%left BITAND
%nonassoc EQ NE LT GT LE GE IN
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIVIDE MODULO FLOOR
%right EXPONENT
%right INVERT BITNOT
%left '.' '[' '('

%type <generic> slice_start slice_end sliced_items
%type <generic> boolean postfix term equals 
%type <generic> value values  ternary binop lambda
%type <generic> list class set 
%type <generic> func assignment 
%type <generic> dict dict_items kwarg kwargs 
%type <generic> statement simple_stmt compound_stmt
%type <generic> params param_item args
%type <generic> while_stmt if_stmt elif_chain else_block 
%type <generic> program stmt_list left_side for_stmt
%type <generic> import module import_items import_name
%type <generic> exception try_block 
%type <generic> catch_block final_block
%type <generic> block enum enumerations enumeration

%%

program
   : /* empty */                           { global_statements = list_create(0); }
   | stmt_list                             { global_statements = $1; }
   ;


block
   : LBRACE stmt_list RBRACE               { $$ = $2; }
   | LBRACE RBRACE                         { $$ = list_create(0); }
   | LBRACE error RBRACE                   { $$ = list_create(0); yyerrok; }
   | NL block                              { $$ = $2; }
   | ':' simple_stmt                       { $$ = list_create(1); list_append($2, $$); }
   ;

stmt_list
   : statement                             { $$ = list_create(__len__); list_append($1, $$); }
   | stmt_list statement                   { list_append($2, $1); $$ = $1; } 
   ;

statement
   : simple_stmt                           { $$ = $1; }
   | compound_stmt                         { $$ = $1; }
   | NL                                    { $$ = NULL; }
   | error NL                              { $$ = NULL; yyerrok; }
   | error ';'                             { $$ = NULL; yyerrok; }
   ; 

simple_stmt
   : assignment ';'                        { $$ = createStatement(STMT_ASMT, $1, yylineno); }
   | value ';'                             { $$ = createStatement(STMT_EXPR, $1, yylineno); }
   | GLOBAL IDENT ';'                      { $$ = createStatement(STMT_GLOBAL, $2, yylineno); }
   | NONLOCAL IDENT ';'                    { $$ = createStatement(STMT_NONLOCAL, $2, yylineno); }
   | RETURN value ';'                      { $$ = createStatement(STMT_RETURN, createReturn($2, f_depth, yylineno), yylineno); }
   | RETURN ';'                            { $$ = createStatement(STMT_RETURN, createReturn(NULL, f_depth, yylineno), yylineno); }
   | CONTINUE ';'                          { $$ = createStatement(STMT_CONTINUE, createJump(l_depth, 0, yylineno), yylineno); }
   | BREAK ';'                             { $$ = createStatement(STMT_BREAK, createJump(l_depth, 1, yylineno), yylineno); }
   | THROW value ';'                       { $$ = createStatement(STMT_THROW, $2, yylineno); }
   ;

compound_stmt
   : { line = yylineno; } func        { $$ = createStatement(STMT_FUNC, $2, line); }
   | { line = yylineno; } class       { $$ = createStatement(STMT_CLASS, $2, line); }
   | { line = yylineno; } if_stmt     { $$ = createStatement(STMT_FLOW, $2, line); }
   | { line = yylineno; } while_stmt  { $$ = createStatement(STMT_WHILE, $2, line); }
   | { line = yylineno; } for_stmt    { $$ = createStatement(STMT_FOR, $2, line); }
   | { line = yylineno; } import      { $$ = createStatement(STMT_IMPORT, $2, line); }
   | { line = yylineno; } exception   { $$ = createStatement(STMT_EXCEPTION, $2, line); }
   | { line = yylineno; } enum        { $$ = createStatement(STMT_ENUM, $2, line); }
   ;

import
   : IMPORT module ';'                                     { $$ = createImport($2, NULL, NULL, 0); }
   | IMPORT module AS IDENT ';'                            { $$ = createImport($2, NULL, $4, 0); }
   | FROM module IMPORT import_items ';'                   { $$ = createImport($2, $4, NULL, 0); }
   | FROM module IMPORT STAR ';'                           { $$ = createImport($2, NULL, NULL, 1); ast_free($4); }
   ;

module
   : IDENT                                                 { $$ = $1; }
   | module '.' IDENT                                      { $$ = str_concat(str_concat($1, "."), $3); }
   ;

import_items
   : import_name                                           { $$ = list_create(__len__); list_append($1, $$); }
   | import_items ',' import_name                          { list_append($3, $1); $$ = $1; }
   ;

import_name
   : IDENT                                                 { $$ = createASTnode(createData(TYPE_LOOKUP, $1)); }
   | import_name '.' IDENT                                 { $$ = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute($1, $3))); }
   ;

enum
   : ENUM IDENT LBRACE enumerations RBRACE                 { $$ = createEnum($2, $4); }
   ;

enumerations
   : enumeration                                           { $$ = list_create(__len__);  list_append($1, $$); }
   | enumerations ',' enumeration                          { list_append($3, $1); $$ = $1; }
   ;

enumeration
   : IDENT                                                 { $$ = createEnumItem($1, NULL); }
   | IDENT '=' INTEGER                                     { $$ = createEnumItem($1, $3); }
   | NL enumeration                                        { $$ = $2; }
   ;


class
   : CLASS IDENT block                                       { $$ = createClass($2, $3, NULL); }
   | CLASS IDENT '(' args ')' block                          { $$ = createClass($2, $6, $4); }
   ;

assignment
   : left_side '=' values                                    { $$ = createAssignment($1, $3, NULL); }
   | left_side equals values                                 { $$ = createAssignment($1, $3, $2); }
   ;

left_side 
   : postfix                                                  { $$ = list_create(__len__); list_append($1, $$); }
   | left_side ',' postfix                                    { list_append($3, $1); $$ = $1; }
   ;

equals
   : PLUS_EQ | MINUS_EQ | DIV_EQ | MUL_EQ 
   | FLDIV_EQ | EXP_EQ | MOD_EQ 
   | R_SHIFT_EQ | L_SHIFT_EQ | XOR_EQ 
   | AND_EQ | OR_EQ
   ;


value
   : ternary                                { $$ = $1; }
   ;

ternary
   : lambda                                 { $$ = $1; }
   | lambda QMARK lambda ':' ternary        { $$ = createASTnode(createData(TYPE_TERNARY, createTernary($1, $3, $5))); }
   | AWAIT lambda                           { $$ = createASTnode(createData(TYPE_AWAIT, createAwait($2))); }
   ;

lambda
   : binop                                  { $$ = $1; }
   | LAMBDA params ':' lambda               { $$ = createASTnode(createLambda($2, $4, yylineno)); }
   ;

binop
   : postfix
   | '(' value ')'                                       { $$ = $2; }
   | binop LOR    binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop LAND   binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop BITOR  binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop BITXOR binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop BITAND binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop EQ     binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop NE     binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop LT     binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop GT     binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop LE     binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop GE     binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop LSHIFT binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop RSHIFT binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop PLUS   binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop MINUS  binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop STAR   binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop DIVIDE binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop MODULO binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop FLOOR  binop                                  { $$ = createASTexpr($2, $1, $3); }
   | binop EXPONENT binop                                { $$ = createASTexpr($2, $1, $3); }
   | binop IN     binop                                  { $$ = createASTexpr($2, $1, $3); }
   | INVERT binop                                        { $$ = createASTexpr($1, NULL, $2); }
   | BITNOT binop                                        { $$ = createASTexpr($1, NULL, $2); }
   | MINUS binop %prec INVERT 
                                                         { $$ = createASTexpr($1, createASTnode(createData(TYPE_INT, &(int){0})), $2); }
   | PLUS  binop %prec INVERT 
                                                         { $$ = createASTexpr($1, createASTnode(createData(TYPE_INT, &(int){0})), $2); }
   ;
postfix
   : term                                                { $$ = $1; }
   | postfix '(' args ')'                                 { $$ = createASTnode(createData(TYPE_INVOKED, createInvoked($1, $3, NULL))); }
   | postfix '(' args ',' kwargs ')'                      { $$ = createASTnode(createData(TYPE_INVOKED, createInvoked($1, $3, $5))); }
   | postfix '.' IDENT                                    { $$ = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute($1, $3))); }
   | postfix '[' value ']'                                { $$ = createASTnode(createData(TYPE_INDEX, createIndexed($1, $3))); }
   | postfix sliced_items                                 { $$ = createASTnode(createData(TYPE_SLICE, createIndexed($1, $2))); }
   ;

term
   : INTEGER                                             { $$ = $1; }
   | DECIMAL                                             { $$ = $1; }
   | STRING                                              { $$ = $1; }
   | FSTRING                                             { $$ = $1; }
   | boolean                                             { $$ = $1; }
   | list                                                { $$ = createASTnode($1); }
   | dict                                                { $$ = createASTnode($1); }
   | set                                                 { $$ = createASTnode($1); }
   | IDENT                                               { $$ = createASTnode(createData(TYPE_LOOKUP, $1)); }
   | NONE                                                { $$ = createASTnode(createData(TYPE_NONE, NULL)); }
   ;

args 
   : /* No args */                                       { $$ = list_create(0); }
   | value                                               { $$ = list_create(__len__); list_append($1, $$); }
   | args ',' value                                      { list_append($3, $1); $$ = $1; }
   | NL value                                            { $$ = list_create(__len__); list_append($2, $$); }
   ; 

boolean
   : TRUE                                        { $$ = $1; }
   | FALSE                                       { $$ = $1; }
   ;

set 
   : LBRACE values RBRACE                        { $$ = createData(TYPE_SET_EXPR, $2); }
   ;

dict 
   : LBRACE dict_items RBRACE                   { $$ = createData(TYPE_DICT_EXPR, $2); }
   | LBRACE RBRACE                              { $$ = createData(TYPE_DICT_EXPR, list_create(0)); }
   ;


dict_items
   : value ':' value                             { $$ = list_create(__len__); list_append(createPair($1, $3), $$); }
   | NL value ':' value                          { $$ = list_create(__len__); list_append(createPair($2, $4), $$); }
   | dict_items ',' value ':' value              { list_append(createPair($3, $5), $1); $$ = $1; }
   ;


list
   : '[' ']'                                     { $$ = createData(TYPE_LIST_EXPR, list_create(0)); }
   | '[' values ']'                              { $$ = createData(TYPE_LIST_EXPR, $2); }
   ;

sliced_items
   : '[' slice_start ':' slice_end ':' slice_end ']'      { $$ = createSlice($2, $4, $6); }
   | '[' slice_start ':' slice_end ']'                    { $$ = createSlice($2, $4, NULL); }
   ;

slice_start
   : /* empty */                           { $$ = NULL; }
   | value                                 { $$ = $1; }
   ;

slice_end
   : /* empty */                           { $$ = NULL; }
   | value                                 { $$ = $1; }
   ;

values 
   : value                                 { $$ = list_create(__len__); list_append($1, $$); }
   | NL value                              { $$ = list_create(__len__); list_append($2, $$); }
   | values ',' value                      { list_append($3, $1); $$ = $1; }
   ;

kwargs
   : kwarg                                 { $$ = list_create(__len__); list_append($1, $$); }
   | kwargs ',' kwarg                      { list_append($3, $1); $$ = $1; }
   ;

kwarg  
   : IDENT '=' value                       { $$ = createPair(createData(TYPE_STR, $1), $3); }
   ;

func
   : FUNC IDENT '(' params ')' { f_depth++; } block             { $$ = createFunction($2, $4, $7, 0, &f_depth); }
   | ASYNC FUNC IDENT '(' params ')' { f_depth++; } block       { $$ = createFunction($3, $5, $8, 1, &f_depth); }
   ;

params                   
   : /* Empty */                                                  { $$ = list_create(0); }
   | param_item                                                   { $$ = list_create(__len__); list_append($1, $$); }
   | NL param_item                                                { $$ = list_create(__len__); list_append($2, $$); }
   | params ',' param_item                                        { list_append($3, $1); $$ = $1; }
   ;                       

param_item
   : IDENT                                                        { $$ = createParam($1, NULL, 0); }
   | IDENT '=' value                                              { $$ = createParam($1, $3, 0); }
   | STAR IDENT                                                   { ast_free($1); $$ = createParam($2, NULL, 1); } // ← variadic
   ;


while_stmt
   : WHILE value { l_depth++; } block                  { $$ = createWhile($2, $4, &l_depth); }
   ;

if_stmt
   : IF value block elif_chain else_block              { $$ = createFlow($2, $3, $4, $5); }
   ;

elif_chain   
   : /* empty */                                       { $$ = NULL; }
   | ELIF value block elif_chain                       { $$ = createFlow($2, $3, $4, NULL); }
   ;

else_block
   : /* empty */                                       { $$ = NULL; }
   | ELSE block                                        { $$ = createFlow(NULL, $2, NULL, NULL); }
   ;

for_stmt
   : FOR IDENT IN value { l_depth++; } block           { ast_free($3); $$ = createFor($2, $4, $6, &l_depth); }
   | FOR '(' IDENT IN value ')' { l_depth++; } block   { ast_free($4); $$ = createFor($3, $5, $8, &l_depth);}
   ;


exception
   : try_block catch_block                             { $$ = createException($1, $2, NULL); }
   | try_block final_block                              { $$ = createException($1, $2, NULL); }
   | try_block catch_block final_block                  { $$ = createException($1, $2, $3); }
   ;

try_block
   : TRY block                                         { $$ = $2; }
   ;

catch_block
   : CATCH '(' IDENT ')' block                         { $$ = createCatch($3, NULL, $5); }
   | CATCH block                                       { $$ = createCatch(NULL, NULL, $2); }
   | CATCH '(' IDENT AS IDENT ')' block                { $$ = createCatch($3, $5, $7); }
   ;


final_block
   : FINALLY block                                     { $$ = $2; }
   ;

%%

int main(int argc, char **argv)                        { return run(argc, argv); }