%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "y.tab.h"

#include "../main.h"

#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/error.h"

#define YY_USER_ACTION column += yyleng;
int column = 0;

static char *process_escapes(const char *src, int len)
{
   char *result = malloc(len + 1);
   int i = 0, j = 0;

   while (i < len)
   {
      if (src[i] == '\\' && i + 1 < len)
      {
         i++;
         switch (src[i])
         {
         case 'n':  result[j++] = '\n'; break;
         case 't':  result[j++] = '\t'; break;
         case 'r':  result[j++] = '\r'; break;
         case '\\': result[j++] = '\\'; break;
         case '\'': result[j++] = '\''; break;
         case '"':  result[j++] = '"';  break;
         case 'a':  result[j++] = '\a'; break;
         case 'b':  result[j++] = '\b'; break;
         case 'f':  result[j++] = '\f'; break;
         case 'v':  result[j++] = '\v'; break;
         case '0':  result[j++] = '\0'; break;
         case 'x':
         {
            // Hex escape \xNN
            if (i + 2 < len && isxdigit(src[i+1]) && isxdigit(src[i+2]))
            {
               char hex[3] = { src[i+1], src[i+2], '\0' };
               result[j++] = (char)strtol(hex, NULL, 16);
               i += 2;
            }
            else
               result[j++] = 'x';
            break;
         }
         case 'u':
         {
            // Unicode escape \uNNNN — store as UTF-8
            if (i + 4 < len)
            {
               char hex[5] = { src[i+1], src[i+2], src[i+3], src[i+4], '\0' };
               unsigned int codepoint = (unsigned int)strtol(hex, NULL, 16);
               if (codepoint < 0x80)
                  result[j++] = (char)codepoint;
               else if (codepoint < 0x800)
               {
                  result[j++] = (char)(0xC0 | (codepoint >> 6));
                  result[j++] = (char)(0x80 | (codepoint & 0x3F));
               }
               else
               {
                  result[j++] = (char)(0xE0 | (codepoint >> 12));
                  result[j++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
                  result[j++] = (char)(0x80 | (codepoint & 0x3F));
               }
               i += 4;
            }
            break;
         }
         default:
            // Unknown escape — keep as-is
            result[j++] = '\\';
            result[j++] = src[i];
            break;
         }
      }
      else
         result[j++] = src[i];
      i++;
   }

   result[j] = '\0';
   return result;
}

YY_BUFFER_STATE get_current_buffer(void) {
    return YY_CURRENT_BUFFER;
}
%}

%option noyywrap
%option yylineno
%option nounput
%option noinput

%x AFTER_SEMI

DIGIT       [0-9]
LETTER      [_a-zA-Z]
IDENT       {LETTER}({LETTER}|{DIGIT})*
INT         {DIGIT}+
FLOAT       {DIGIT}+"."{DIGIT}+
STRING_DQ   \"([^"\\]|\\.)*\"
STRING_SQ   \'([^'\\]|\\.)*\'
FSTRING_DQ   f\"([^"\\]|\\.)*\"
FSTRING_SQ   f\'([^'\\]|\\.)*\'

%%

   /* Newlines - returned as NL for statement termination */
\n                     { column = 0; BEGIN(AFTER_SEMI); }

<AFTER_SEMI>[ \t\r]+   { /* skip whitespace */ }
<AFTER_SEMI>\n         { /* skip blank lines, stay in AFTER_SEMI */ }
<AFTER_SEMI>else       { BEGIN(INITIAL); return ELSE; }
<AFTER_SEMI>elif       { BEGIN(INITIAL); return ELIF; }
<AFTER_SEMI>catch      { BEGIN(INITIAL); return CATCH; }
<AFTER_SEMI>finally     { BEGIN(INITIAL); return FINALLY; }
<AFTER_SEMI>as         { BEGIN(INITIAL); return AS; }
<AFTER_SEMI>"}"        { BEGIN(AFTER_SEMI); return RBRACE; }
<AFTER_SEMI>"{"        { BEGIN(INITIAL); return LBRACE; }
<AFTER_SEMI>.          { BEGIN(INITIAL); yyless(0); return NL; }


   /* Block delimiters */
"{"         { return LBRACE; }
"}"         { BEGIN(AFTER_SEMI); return RBRACE; }

";"         { return ';'; }

   /* Keywords */
"func"       { return FUNC; }
"class"      { return CLASS; }
"enum"       { return ENUM; }
"return"     { return RETURN; }
"try"        { return TRY; }
"catch"      { return CATCH; }
"finally"     { return FINALLY; }
"throw"      { return THROW; }
"async"      { return ASYNC; }
"await"      { return AWAIT; }
"continue"   { return CONTINUE; }
"break"      { return BREAK; }
"import"     { return IMPORT; }
"from"       { return FROM; }
"as"         { return AS; }
"if"         { return IF; }
"elif"       { return ELIF; }
"else"       { return ELSE; }
"while"      { return WHILE; }
"for"        { return FOR; }
"global"     { return GLOBAL; }
"nonlocal"   { return NONLOCAL; }
"lambda"     { return LAMBDA; }
"none"       { return NONE; }

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
   yylval.generic = createASTop(AND);
   return LAND;
}

"or" {
   yylval.generic = createASTop(OR);
   return LOR;
}

"not" {
   yylval.generic = createASTop(NOT);
   return INVERT;
}

"in"  { yylval.generic = createASTop(INTO); return IN; }

   /* Comparison operators */
"==" { yylval.generic = createASTop(EQUAL); return EQ; }
"!=" { yylval.generic = createASTop(NEQ);   return NE; }
"<=" { yylval.generic = createASTop(LEQ);   return LE; }
">=" { yylval.generic = createASTop(GEQ);   return GE; }
"<"  { yylval.generic = createASTop(LST);   return LT; }
">"  { yylval.generic = createASTop(GRT);   return GT; }

   /* Bitwise operators */
"<<"  { yylval.generic = createASTop(LEFT_SHIFT);  return LSHIFT; }
">>"  { yylval.generic = createASTop(RIGHT_SHIFT); return RSHIFT; }
"&"   { yylval.generic = createASTop(BIT_AND);     return BITAND; }
"|"   { yylval.generic = createASTop(BIT_OR);      return BITOR; }
"^"   { yylval.generic = createASTop(XOR);         return BITXOR; }
"~"   { yylval.generic = createASTop(BIT_NOT);     return BITNOT; }

   /* Arithmetic operators */
"+"   { yylval.generic = createASTop(ADD);       return PLUS; }
"-"   { yylval.generic = createASTop(SUB);       return MINUS; }
"*"   { yylval.generic = createASTop(MUL);       return STAR; }
"/"   { yylval.generic = createASTop(DIV);       return DIVIDE; }
"%"   { yylval.generic = createASTop(MOD);       return MODULO; }
"//"  { yylval.generic = createASTop(FLDIV);     return FLOOR; }
"**"  { yylval.generic = createASTop(EXP);       return EXPONENT; }

   /* Compound assignment operators */
"+="   { yylval.generic = createASTop(ADD);         return PLUS_EQ; }
"-="   { yylval.generic = createASTop(SUB);         return MINUS_EQ; }
"*="   { yylval.generic = createASTop(MUL);         return MUL_EQ; }
"/="   { yylval.generic = createASTop(DIV);         return DIV_EQ; }
"%="   { yylval.generic = createASTop(MOD);         return MOD_EQ; }
"//="  { yylval.generic = createASTop(FLDIV);       return FLDIV_EQ; }
"**="  { yylval.generic = createASTop(EXP);         return EXP_EQ; }
"<<="  { yylval.generic = createASTop(LEFT_SHIFT);  return L_SHIFT_EQ; }
">>="  { yylval.generic = createASTop(RIGHT_SHIFT); return R_SHIFT_EQ; }
"&="   { yylval.generic = createASTop(BIT_AND);     return AND_EQ; }
"|="   { yylval.generic = createASTop(BIT_OR);      return OR_EQ; }
"^="   { yylval.generic = createASTop(XOR);         return XOR_EQ; }

   /* Delimiters */
")"   { return ')'; }
"("   { return '('; }
"["   { return '['; }
"]"   { return ']'; }
","   { return ','; }
":"   { return ':'; }
"?"   { return QMARK; }
"."   { return '.'; }
"="   { return '='; }

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
   char *raw = malloc(len + 1);
   if (!raw) { fprintf(stderr, "Lexer error: Out of memory\n"); exit(1); }
   strncpy(raw, yytext + 1, len);
   raw[len] = '\0';

   char *content = process_escapes(raw, len);
   free(raw);

   yylval.generic = createASTnode(createData(TYPE_STR, content));
   free(content);
   return STRING;
}

{FSTRING_DQ}|{FSTRING_SQ} {
   int len = yyleng - 3;
   char *content = malloc(len + 1);
   strncpy(content, yytext + 2, len);
   content[len] = '\0';
   yylval.generic = createASTnode(createData(TYPE_FSTRING, content));
   free(content);
   return FSTRING;
}
   /* Identifiers */
{IDENT} {
   yylval.generic = strdup(yytext);
   return IDENT;
}

   /* Whitespace and comments - skip */
[ \t\r]+    { /* skip */ }
#.*         { /* skip comments */ }

   /* EOF */
<<EOF>> {
   yyterminate();
}

   /* Unexpected character */
. {
   g_thrown.lineno = yylineno;
   throw_error(ERROR_SYNTAX, "unexpected character '%s'\n", yytext);
   exit(1);
}

%%
