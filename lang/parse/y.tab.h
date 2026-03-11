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
     SWITCH = 283,
     CASE = 284,
     DEFAULT = 285,
     ASYNC = 286,
     AWAIT = 287,
     IDENT = 288,
     STRING = 289,
     INTEGER = 290,
     DECIMAL = 291,
     LSHIFT = 292,
     RSHIFT = 293,
     PLUS = 294,
     MINUS = 295,
     STAR = 296,
     DIVIDE = 297,
     MODULO = 298,
     FLOOR = 299,
     EXPONENT = 300,
     BITAND = 301,
     BITOR = 302,
     BITXOR = 303,
     MINUS_EQ = 304,
     DIV_EQ = 305,
     BITNOT = 306,
     MUL_EQ = 307,
     MOD_EQ = 308,
     LE = 309,
     IN = 310,
     EXP_EQ = 311,
     FLDIV_EQ = 312,
     R_SHIFT_EQ = 313,
     XOR_EQ = 314,
     NONE = 315,
     AND_EQ = 316,
     OR_EQ = 317,
     TRUE = 318,
     FALSE = 319,
     LAND = 320,
     PLUS_EQ = 321,
     INVERT = 322,
     EQ = 323,
     NE = 324,
     LT = 325,
     GT = 326,
     GE = 327,
     LOR = 328,
     FSTRING = 329,
     L_SHIFT_EQ = 330
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
#define SWITCH 283
#define CASE 284
#define DEFAULT 285
#define ASYNC 286
#define AWAIT 287
#define IDENT 288
#define STRING 289
#define INTEGER 290
#define DECIMAL 291
#define LSHIFT 292
#define RSHIFT 293
#define PLUS 294
#define MINUS 295
#define STAR 296
#define DIVIDE 297
#define MODULO 298
#define FLOOR 299
#define EXPONENT 300
#define BITAND 301
#define BITOR 302
#define BITXOR 303
#define MINUS_EQ 304
#define DIV_EQ 305
#define BITNOT 306
#define MUL_EQ 307
#define MOD_EQ 308
#define LE 309
#define IN 310
#define EXP_EQ 311
#define FLDIV_EQ 312
#define R_SHIFT_EQ 313
#define XOR_EQ 314
#define NONE 315
#define AND_EQ 316
#define OR_EQ 317
#define TRUE 318
#define FALSE 319
#define LAND 320
#define PLUS_EQ 321
#define INVERT 322
#define EQ 323
#define NE 324
#define LT 325
#define GT 326
#define GE 327
#define LOR 328
#define FSTRING 329
#define L_SHIFT_EQ 330




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 31 "./parse/parser.y"
{
   void *generic;
}
/* Line 1529 of yacc.c.  */
#line 203 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

