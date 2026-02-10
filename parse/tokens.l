%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
#include "../lib/type.h"
#include "../lib/maloc.h"
#include "../lib/error.h"

// Indentation stack
typedef struct {
   int levels[100];
   int top;
} IndentStack;

IndentStack indent_stack = {{0}, 1};
int pending_dedents = 0;
int at_line_start = 1;
int column = 0;

int get_indent_level(char *text, int len) 
{
  int spaces = 0;
  for (int i = 0; i < len; i++) {
    if (text[i] == '\t') 
        spaces += 4;
    else 
        spaces++;
  }
  return spaces;
}

#define YY_USER_ACTION column += yyleng;
%}

%option noyywrap
%option yylineno
%option nounput
%option noinput

DIGIT       [0-9]
LETTER      [_a-zA-Z]
IDENT       {LETTER}({LETTER}|{DIGIT})*
INT         -?{DIGIT}+
FLOAT       {DIGIT}+"."{DIGIT}+
STRING_DQ   \"([^"\\]|\\.)*\"
STRING_SQ   \'([^'\\]|\\.)*\'

%%

   /* Handle pending DEDENT tokens */
%{
   if (pending_dedents > 0) {
      pending_dedents--;
      return DEDENT;
   }
%}

   /* Empty lines - keep them simple */
^[ \t]*\n {
   at_line_start = 1;
   return NL;
}

   /* Indented line - match indent + first char of content */
^[ \t]+[^ \t\n] {

   if (at_line_start) {
      at_line_start = 0;
      
      /* Find where the actual content starts */
      int indent_len = 0;
      while (yytext[indent_len] == ' ' || yytext[indent_len] == '\t') {
         indent_len++;
      }
      
      int new_level = get_indent_level(yytext, indent_len);
      int current_level = indent_stack.levels[indent_stack.top - 1];
      
      if (new_level > current_level) {
         if (indent_stack.top >= 100) {
            exit(1);
         }
         indent_stack.levels[indent_stack.top++] = new_level;
         
         /* Put back everything after the indent */
         yyless(indent_len);
         return INDENT;
         
      } else if (new_level < current_level) {
         while (indent_stack.top > 1 && 
               indent_stack.levels[indent_stack.top - 1] > new_level) {
            indent_stack.top--;
            pending_dedents++;
         }
         
         
         if (indent_stack.levels[indent_stack.top - 1] != new_level) {
            fprintf(stderr, "ERROR: Indentation mismatch!\n");
            exit(1);
         }
         /* Put back everything */
         yyless(0);
         at_line_start = 1; // Keep it so the dedent logic works
         
         if (pending_dedents > 0) {
            pending_dedents--;
            return DEDENT;
         }
      } else {
         /* Same level - put back everything after indent */
         yyless(indent_len);
      }
   }
}

   /* Non-indented line when we're indented - need dedents */
^[a-zA-Z_#][a-zA-Z0-9_]* {
   if (at_line_start && indent_stack.top > 1) {
      /* We need to dedent back to base level */
      while (indent_stack.top > 1) {
         indent_stack.top--;
         pending_dedents++;
      }
      
      /* Put the entire word back */
      yyless(0);
      
      if (pending_dedents > 0) {
         pending_dedents--;
         return DEDENT;
      }
   }
   
   /* Not at line start or no dedents needed, put it back to be matched by other rules */
   yyless(0);
}

   /* Regular newline */
\n {
   at_line_start = 1;
   column = 0;
   return NL;
}

   /* Keywords - elif and else need special handling for dedents */
"function"   { at_line_start = 0; return FUNCTION; }
"class"      { at_line_start = 0; return CLASS; }
"return"     { at_line_start = 0; return RETURN; }
"continue"   { at_line_start = 0; return CONTINUE; }
"break"      { at_line_start = 0; return BREAK; }
"import"     { at_line_start = 0; return IMPORT; }
"from"       { at_line_start = 0; return FROM; }
"as"         { at_line_start = 0; return AS; }
"pass"       { at_line_start = 0; return PASS; }
"if"         { at_line_start = 0; return IF; }
"elif"       { 
   /* If we're at the start of a line and indented, need to dedent first */
   if (at_line_start && indent_stack.levels[indent_stack.top - 1] > 0) {
      at_line_start = 0;
      /* Generate dedents back to base level */
      while (indent_stack.top > 1) {
         indent_stack.top--;
         pending_dedents++;
      }
      /* Put the keyword back */
      yyless(0);
      if (pending_dedents > 0) {
         pending_dedents--;
         return DEDENT;
      }
   }
   at_line_start = 0;
   return ELIF;
}
"else"       { 
   /* If we're at the start of a line and indented, need to dedent first */
   if (at_line_start && indent_stack.levels[indent_stack.top - 1] > 0) {
      at_line_start = 0;
      /* Generate dedents back to base level */
      while (indent_stack.top > 1) {
         indent_stack.top--;
         pending_dedents++;
      }
      /* Put the keyword back */
      yyless(0);
      if (pending_dedents > 0) {
         pending_dedents--;
         return DEDENT;
      }
   }
   at_line_start = 0;
   return ELSE;
}
"while"      { at_line_start = 0; return WHILE; }
"for"        { at_line_start = 0; return FOR; }
"in"         { at_line_start = 0; return IN; }
"global"     { at_line_start = 0; return GLOBAL; }
"nonlocal"   { at_line_start = 0; return NONLOCAL; }


   /* Delimiters */
"("   { return '('; }
")"   { return ')'; }
"["   { return '['; }
"]"   { return ']'; }
"{"   { return '{'; }
"}"   { return '}'; }
":"   { return ':'; }
","   { return ','; }
"."   { return '.'; }
"="   { return '='; }

"None" { return NONE; }


   /* Boolean literals */
"true" {
   int value = 1;
   yylval.generic = createASTnode(createData(TYPE_BOOL, &value));
   return TRUE;
}

"false" {
   int value = 0;
   yylval.generic = createASTnode(createData(TYPE_BOOL, &value));
   return FALSE;
}

   /* Logical operators */
"and" {
   yylval.generic = createASToperator(LOGICAL_AND);
   return LOGIC_AND;
}

"or" {
   yylval.generic = createASToperator(LOGICAL_OR);
   return LOGIC_OR;
}

"not" {
   yylval.generic = createASToperator(NOT);
   return INVERT;
}

   /* Comparison operators */
"==" { yylval.generic = createASToperator(EQUAL); return EQ; }
"!=" { yylval.generic = createASToperator(NEQ);   return NE; }
"<=" { yylval.generic = createASToperator(LEQ);   return LE; }
">=" { yylval.generic = createASToperator(GEQ);   return GE; }
"<"  { yylval.generic = createASToperator(LST);   return LT; }
">"  { yylval.generic = createASToperator(GRT);   return GT; }

   /* Bitwise operators */
"<<"  { yylval.generic = createASToperator(LEFT_SHIFT);  return L_SHIFT; }
">>"  { yylval.generic = createASToperator(RIGHT_SHIFT); return R_SHIFT; }
"&"   { yylval.generic = createASToperator(BIT_AND);     return BITAND; }
"|"   { yylval.generic = createASToperator(BIT_OR);      return BITOR; }
"^"   { yylval.generic = createASToperator(XOR);         return BITXOR; }
"~"   { yylval.generic = createASToperator(BIT_NOT);     return BITNOT; }

   /* Arithmetic operators */
"+"   { yylval.generic = createASToperator(PLUS);      return ADD; }
"-"   { yylval.generic = createASToperator(MINUS);     return SUB; }
"*"   { yylval.generic = createASToperator(MULTIPLY);  return MUL; }
"/"   { yylval.generic = createASToperator(DIVIDE);    return DIV; }
"%"   { yylval.generic = createASToperator(MODULO);    return MOD; }
"//"  { yylval.generic = createASToperator(FLOOR);     return FLDIV; }
"**"  { yylval.generic = createASToperator(EXPONENT);  return EXP; }

   /* Compound assignment operators */
"+="   { yylval.generic = createASToperator(PLUS);        return PLUS_EQ; }
"-="   { yylval.generic = createASToperator(MINUS);       return MINUS_EQ; }
"*="   { yylval.generic = createASToperator(MULTIPLY);    return MUL_EQ; }
"/="   { yylval.generic = createASToperator(DIVIDE);      return DIV_EQ; }
"%="   { yylval.generic = createASToperator(MODULO);      return MOD_EQ; }
"//="  { yylval.generic = createASToperator(FLOOR);       return FLDIV_EQ; }
"**="  { yylval.generic = createASToperator(EXPONENT);    return EXP_EQ; }
"<<="  { yylval.generic = createASToperator(LEFT_SHIFT);  return L_SHIFT_EQ; }
">>="  { yylval.generic = createASToperator(RIGHT_SHIFT); return R_SHIFT_EQ; }
"&="   { yylval.generic = createASToperator(BIT_AND);     return AND_EQ; }
"|="   { yylval.generic = createASToperator(BIT_OR);      return OR_EQ; }
"^="   { yylval.generic = createASToperator(XOR);         return XOR_EQ; }

   /* Numbers */
{FLOAT} {
   double value = atof(yytext);
   yylval.generic = createASTnode(createData(TYPE_FLOAT, &value));
   return DECIMAL;
}

{INT} {
   int value = atoi(yytext);
   yylval.generic = createASTnode(createData(TYPE_INT, &value));
   return INTEGER;
}

   /* Strings */
{STRING_DQ}|{STRING_SQ} {
   int len = yyleng - 2;
   char *content = malloc(len + 1);
   if (!content) {
      fprintf(stderr, "Lexer error: Out of memory\n");
      exit(1);
   }
   strncpy(content, yytext + 1, len);
   content[len] = '\0';
   yylval.generic = createASTnode(createData(TYPE_STR, content));
   free(content);
   return STRING;
}

   /* Identifiers */
{IDENT} {
   /* Check if we need to dedent before returning this token */
   if (at_line_start && indent_stack.levels[indent_stack.top - 1] > 0) {
      at_line_start = 0;
      
      /* Generate dedents back to base level */
      while (indent_stack.top > 1) {
         indent_stack.top--;
         pending_dedents++;
      }
      
      /* Put the identifier back so it can be parsed after dedents */
      yyless(0);
      
      /* Return first DEDENT */
      if (pending_dedents > 0) {
         pending_dedents--;
         return DEDENT;
      }
   }
   
   at_line_start = 0;
   yylval.generic = strdup(yytext);
   return VARIABLE;
}

   /* Whitespace (not newlines) */
[ \r\t]+ { /* Skip */ }

   /* Comments */
#.*  { /* Ignore comments */ }

   /* EOF handling */
<<EOF>> {
   /* If we're not at the start of a line, emit a NEWLINE first */
   if (!at_line_start) {
      at_line_start = 1;
      return NL;
   }
   
   if (pending_dedents > 0) {
      pending_dedents--;
      return DEDENT;
   }
   
   if (indent_stack.top > 1) {
      indent_stack.top--;
      return DEDENT;
   }
   
   yyterminate();
}

   /* Unexpected character */
. {
   throw_error(ERROR_SYNTAX, "line %d: unexpected character '%s'\n", yylineno, yytext);
   exit(1);
}
%%