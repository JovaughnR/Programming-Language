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
     NEWLINE = 266,
     RETURN = 267,
     CONTINUE = 268,
     BREAK = 269,
     WHILE = 270,
     FOR = 271,
     IN = 272,
     IF = 273,
     ELIF = 274,
     ELSE = 275,
     GLOBAL = 276,
     NONLOCAL = 277,
     VARIABLE = 278,
     STRING = 279,
     INTEGER = 280,
     DECIMAL = 281,
     ADD = 282,
     SUB = 283,
     MUL = 284,
     DIV = 285,
     MOD = 286,
     FLDIV = 287,
     EXP = 288,
     BITAND = 289,
     BITOR = 290,
     BITXOR = 291,
     L_SHIFT = 292,
     R_SHIFT = 293,
     PLUS_EQ = 294,
     MINUS_EQ = 295,
     DIV_EQ = 296,
     MUL_EQ = 297,
     MOD_EQ = 298,
     L_SHIFT_EQ = 299,
     R_SHIFT_EQ = 300,
     XOR_EQ = 301,
     FLDIV_EQ = 302,
     EXP_EQ = 303,
     AND_EQ = 304,
     OR_EQ = 305,
     TRUE = 306,
     FALSE = 307,
     NONE = 308,
     LOGIC_AND = 309,
     LOGIC_OR = 310,
     INVERT = 311,
     EQ = 312,
     NE = 313,
     LT = 314,
     GT = 315,
     GE = 316,
     LE = 317
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
#define NEWLINE 266
#define RETURN 267
#define CONTINUE 268
#define BREAK 269
#define WHILE 270
#define FOR 271
#define IN 272
#define IF 273
#define ELIF 274
#define ELSE 275
#define GLOBAL 276
#define NONLOCAL 277
#define VARIABLE 278
#define STRING 279
#define INTEGER 280
#define DECIMAL 281
#define ADD 282
#define SUB 283
#define MUL 284
#define DIV 285
#define MOD 286
#define FLDIV 287
#define EXP 288
#define BITAND 289
#define BITOR 290
#define BITXOR 291
#define L_SHIFT 292
#define R_SHIFT 293
#define PLUS_EQ 294
#define MINUS_EQ 295
#define DIV_EQ 296
#define MUL_EQ 297
#define MOD_EQ 298
#define L_SHIFT_EQ 299
#define R_SHIFT_EQ 300
#define XOR_EQ 301
#define FLDIV_EQ 302
#define EXP_EQ 303
#define AND_EQ 304
#define OR_EQ 305
#define TRUE 306
#define FALSE 307
#define NONE 308
#define LOGIC_AND 309
#define LOGIC_OR 310
#define INVERT 311
#define EQ 312
#define NE 313
#define LT 314
#define GT 315
#define GE 316
#define LE 317




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 33 "parser.y"
{
   void *generic;
}
/* Line 1529 of yacc.c.  */
#line 177 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

