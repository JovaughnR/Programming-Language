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
   yylval.generic = createASTop(LOGICAL_AND);
   return LOGIC_AND;
}

"or" {
   yylval.generic = createASTop(LOGICAL_OR);
   return LOGIC_OR;
}

"not" {
   yylval.generic = createASTop(NOT);
   return INVERT;
}

   /* Comparison operators */
"==" { yylval.generic = createASTop(EQUAL); return EQ; }
"!=" { yylval.generic = createASTop(NEQ);   return NE; }
"<=" { yylval.generic = createASTop(LEQ);   return LE; }
">=" { yylval.generic = createASTop(GEQ);   return GE; }
"<"  { yylval.generic = createASTop(LST);   return LT; }
">"  { yylval.generic = createASTop(GRT);   return GT; }

   /* Bitwise operators */
"<<"  { yylval.generic = createASTop(LEFT_SHIFT);  return L_SHIFT; }
">>"  { yylval.generic = createASTop(RIGHT_SHIFT); return R_SHIFT; }
"&"   { yylval.generic = createASTop(BIT_AND);     return BITAND; }
"|"   { yylval.generic = createASTop(BIT_OR);      return BITOR; }
"^"   { yylval.generic = createASTop(XOR);         return BITXOR; }
"~"   { yylval.generic = createASTop(BIT_NOT);     return BITNOT; }

   /* Arithmetic operators */
"+"   { yylval.generic = createASTop(PLUS);      return ADD; }
"-"   { yylval.generic = createASTop(MINUS);     return SUB; }
"*"   { yylval.generic = createASTop(MULTIPLY);  return MUL; }
"/"   { yylval.generic = createASTop(DIVIDE);    return DIV; }
"%"   { yylval.generic = createASTop(MODULO);    return MOD; }
"//"  { yylval.generic = createASTop(FLOOR);     return FLDIV; }
"**"  { yylval.generic = createASTop(EXPONENT);  return EXP; }

   /* Compound assignment operators */
"+="   { yylval.generic = createASTop(PLUS);        return PLUS_EQ; }
"-="   { yylval.generic = createASTop(MINUS);       return MINUS_EQ; }
"*="   { yylval.generic = createASTop(MULTIPLY);    return MUL_EQ; }
"/="   { yylval.generic = createASTop(DIVIDE);      return DIV_EQ; }
"%="   { yylval.generic = createASTop(MODULO);      return MOD_EQ; }
"//="  { yylval.generic = createASTop(FLOOR);       return FLDIV_EQ; }
"**="  { yylval.generic = createASTop(EXPONENT);    return EXP_EQ; }
"<<="  { yylval.generic = createASTop(LEFT_SHIFT);  return L_SHIFT_EQ; }
">>="  { yylval.generic = createASTop(RIGHT_SHIFT); return R_SHIFT_EQ; }
"&="   { yylval.generic = createASTop(BIT_AND);     return AND_EQ; }
"|="   { yylval.generic = createASTop(BIT_OR);      return OR_EQ; }
"^="   { yylval.generic = createASTop(XOR);         return XOR_EQ; }

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