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
     IMPORT = 258,
     AS = 259,
     LAMBDA = 260,
     QMARK = 261,
     FUNC = 262,
     NL = 263,
     RETURN = 264,
     THROW = 265,
     TRY = 266,
     CATCH = 267,
     FINALLY = 268,
     WHILE = 269,
     FOR = 270,
     CONTINUE = 271,
     BREAK = 272,
     IF = 273,
     ELIF = 274,
     ELSE = 275,
     GLOBAL = 276,
     NONLOCAL = 277,
     ENUM = 278,
     LBRACE = 279,
     RBRACE = 280,
     FROM = 281,
     CLASS = 282,
     ASYNC = 283,
     AWAIT = 284,
     IDENT = 285,
     STRING = 286,
     INTEGER = 287,
     DECIMAL = 288,
     LSHIFT = 289,
     RSHIFT = 290,
     PLUS = 291,
     MINUS = 292,
     STAR = 293,
     DIVIDE = 294,
     MODULO = 295,
     FLOOR = 296,
     EXPONENT = 297,
     BITAND = 298,
     BITOR = 299,
     BITXOR = 300,
     MINUS_EQ = 301,
     DIV_EQ = 302,
     BITNOT = 303,
     MUL_EQ = 304,
     MOD_EQ = 305,
     LE = 306,
     IN = 307,
     EXP_EQ = 308,
     FLDIV_EQ = 309,
     R_SHIFT_EQ = 310,
     XOR_EQ = 311,
     NONE = 312,
     AND_EQ = 313,
     OR_EQ = 314,
     TRUE = 315,
     FALSE = 316,
     LAND = 317,
     PLUS_EQ = 318,
     INVERT = 319,
     EQ = 320,
     NE = 321,
     LT = 322,
     GT = 323,
     GE = 324,
     LOR = 325,
     FSTRING = 326,
     L_SHIFT_EQ = 327
   };
#endif
/* Tokens.  */
#define IMPORT 258
#define AS 259
#define LAMBDA 260
#define QMARK 261
#define FUNC 262
#define NL 263
#define RETURN 264
#define THROW 265
#define TRY 266
#define CATCH 267
#define FINALLY 268
#define WHILE 269
#define FOR 270
#define CONTINUE 271
#define BREAK 272
#define IF 273
#define ELIF 274
#define ELSE 275
#define GLOBAL 276
#define NONLOCAL 277
#define ENUM 278
#define LBRACE 279
#define RBRACE 280
#define FROM 281
#define CLASS 282
#define ASYNC 283
#define AWAIT 284
#define IDENT 285
#define STRING 286
#define INTEGER 287
#define DECIMAL 288
#define LSHIFT 289
#define RSHIFT 290
#define PLUS 291
#define MINUS 292
#define STAR 293
#define DIVIDE 294
#define MODULO 295
#define FLOOR 296
#define EXPONENT 297
#define BITAND 298
#define BITOR 299
#define BITXOR 300
#define MINUS_EQ 301
#define DIV_EQ 302
#define BITNOT 303
#define MUL_EQ 304
#define MOD_EQ 305
#define LE 306
#define IN 307
#define EXP_EQ 308
#define FLDIV_EQ 309
#define R_SHIFT_EQ 310
#define XOR_EQ 311
#define NONE 312
#define AND_EQ 313
#define OR_EQ 314
#define TRUE 315
#define FALSE 316
#define LAND 317
#define PLUS_EQ 318
#define INVERT 319
#define EQ 320
#define NE 321
#define LT 322
#define GT 323
#define GE 324
#define LOR 325
#define FSTRING 326
#define L_SHIFT_EQ 327




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 33 "./parse/parser.y"
{
   void *generic;
}
/* Line 1529 of yacc.c.  */
#line 197 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

