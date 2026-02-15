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
     IN = 275,
     IF = 276,
     ELIF = 277,
     ELSE = 278,
     GLOBAL = 279,
     NONLOCAL = 280,
     VARIABLE = 281,
     STRING = 282,
     INTEGER = 283,
     DECIMAL = 284,
     ADD = 285,
     SUB = 286,
     MUL = 287,
     DIV = 288,
     MOD = 289,
     FLDIV = 290,
     EXP = 291,
     BITAND = 292,
     BITOR = 293,
     BITXOR = 294,
     L_SHIFT = 295,
     R_SHIFT = 296,
     BITNOT = 297,
     PLUS_EQ = 298,
     MINUS_EQ = 299,
     DIV_EQ = 300,
     MUL_EQ = 301,
     MOD_EQ = 302,
     L_SHIFT_EQ = 303,
     R_SHIFT_EQ = 304,
     XOR_EQ = 305,
     FLDIV_EQ = 306,
     EXP_EQ = 307,
     AND_EQ = 308,
     OR_EQ = 309,
     TRUE = 310,
     FALSE = 311,
     NONE = 312,
     LOGIC_AND = 313,
     LOGIC_OR = 314,
     INVERT = 315,
     EQ = 316,
     NE = 317,
     LT = 318,
     GT = 319,
     GE = 320,
     LE = 321
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
#define IN 275
#define IF 276
#define ELIF 277
#define ELSE 278
#define GLOBAL 279
#define NONLOCAL 280
#define VARIABLE 281
#define STRING 282
#define INTEGER 283
#define DECIMAL 284
#define ADD 285
#define SUB 286
#define MUL 287
#define DIV 288
#define MOD 289
#define FLDIV 290
#define EXP 291
#define BITAND 292
#define BITOR 293
#define BITXOR 294
#define L_SHIFT 295
#define R_SHIFT 296
#define BITNOT 297
#define PLUS_EQ 298
#define MINUS_EQ 299
#define DIV_EQ 300
#define MUL_EQ 301
#define MOD_EQ 302
#define L_SHIFT_EQ 303
#define R_SHIFT_EQ 304
#define XOR_EQ 305
#define FLDIV_EQ 306
#define EXP_EQ 307
#define AND_EQ 308
#define OR_EQ 309
#define TRUE 310
#define FALSE 311
#define NONE 312
#define LOGIC_AND 313
#define LOGIC_OR 314
#define INVERT 315
#define EQ 316
#define NE 317
#define LT 318
#define GT 319
#define GE 320
#define LE 321




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 37 "./parse/parser.y"
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

