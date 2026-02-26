/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INDENT = 258,
     DEDENT = 259,
     FROM = 260,
     PASS = 261,
     CLASS = 262,
     IMPORT = 263,
     AS = 264,
     FUNCTION = 265,
     NL = 266,
     RETURN = 267,
     CONTINUE = 268,
     BREAK = 269,
     TRY = 270,
     CATCH = 271,
     FINALLY = 272,
     WHILE = 273,
     FOR = 274,
     IF = 275,
     ELIF = 276,
     ELSE = 277,
     GLOBAL = 278,
     NONLOCAL = 279,
     VARIABLE = 280,
     STRING = 281,
     INTEGER = 282,
     DECIMAL = 283,
     ADD = 284,
     SUB = 285,
     MUL = 286,
     DIV = 287,
     MOD = 288,
     FLDIV = 289,
     EXP = 290,
     BITAND = 291,
     BITOR = 292,
     BITXOR = 293,
     L_SHIFT = 294,
     R_SHIFT = 295,
     BITNOT = 296,
     PLUS_EQ = 297,
     MINUS_EQ = 298,
     DIV_EQ = 299,
     MUL_EQ = 300,
     MOD_EQ = 301,
     L_SHIFT_EQ = 302,
     R_SHIFT_EQ = 303,
     XOR_EQ = 304,
     FLDIV_EQ = 305,
     EXP_EQ = 306,
     AND_EQ = 307,
     OR_EQ = 308,
     TRUE = 309,
     FALSE = 310,
     NONE = 311,
     LOGIC_AND = 312,
     LOGIC_OR = 313,
     INVERT = 314,
     EQ = 315,
     NE = 316,
     LT = 317,
     GT = 318,
     GE = 319,
     LE = 320,
     IN = 321
   };
#endif
/* Tokens.  */
#define INDENT 258
#define DEDENT 259
#define FROM 260
#define PASS 261
#define CLASS 262
#define IMPORT 263
#define AS 264
#define FUNCTION 265
#define NL 266
#define RETURN 267
#define CONTINUE 268
#define BREAK 269
#define TRY 270
#define CATCH 271
#define FINALLY 272
#define WHILE 273
#define FOR 274
#define IF 275
#define ELIF 276
#define ELSE 277
#define GLOBAL 278
#define NONLOCAL 279
#define VARIABLE 280
#define STRING 281
#define INTEGER 282
#define DECIMAL 283
#define ADD 284
#define SUB 285
#define MUL 286
#define DIV 287
#define MOD 288
#define FLDIV 289
#define EXP 290
#define BITAND 291
#define BITOR 292
#define BITXOR 293
#define L_SHIFT 294
#define R_SHIFT 295
#define BITNOT 296
#define PLUS_EQ 297
#define MINUS_EQ 298
#define DIV_EQ 299
#define MUL_EQ 300
#define MOD_EQ 301
#define L_SHIFT_EQ 302
#define R_SHIFT_EQ 303
#define XOR_EQ 304
#define FLDIV_EQ 305
#define EXP_EQ 306
#define AND_EQ 307
#define OR_EQ 308
#define TRUE 309
#define FALSE 310
#define NONE 311
#define LOGIC_AND 312
#define LOGIC_OR 313
#define INVERT 314
#define EQ 315
#define NE 316
#define LT 317
#define GT 318
#define GE 319
#define LE 320
#define IN 321




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 42 "./parse/parser.y"
{
   void *generic;
}
/* Line 1529 of yacc.c.  */
#line 185 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

