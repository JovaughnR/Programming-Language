/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 33 "parser.y"
{
   void *generic;
}
/* Line 193 of yacc.c.  */
#line 256 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 269 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  78
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1219

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  127
/* YYNRULES -- Number of states.  */
#define YYNSTATES  256

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      63,    68,     2,     2,    66,     2,    65,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    67,     2,
       2,    69,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    64,     2,    70,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    71,     2,    72,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    13,    15,    17,
      20,    23,    26,    30,    34,    38,    41,    44,    47,    49,
      51,    53,    55,    57,    59,    62,    67,    72,    77,    79,
      83,    85,    89,    91,    95,   103,   114,   118,   122,   124,
     128,   130,   132,   134,   136,   138,   140,   142,   144,   146,
     148,   150,   152,   154,   158,   162,   166,   170,   174,   178,
     182,   186,   190,   194,   198,   202,   206,   210,   214,   218,
     222,   226,   230,   234,   238,   241,   244,   246,   250,   255,
     262,   267,   269,   271,   273,   275,   277,   279,   281,   283,
     285,   286,   288,   292,   294,   296,   299,   303,   305,   309,
     313,   316,   320,   322,   326,   328,   332,   336,   340,   342,
     344,   346,   348,   349,   361,   362,   364,   368,   370,   374,
     375,   386,   396,   397,   406,   407,   414,   415
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      74,     0,    -1,    -1,    75,    -1,    76,    -1,    75,    76,
      -1,    77,    -1,    78,    -1,    11,    -1,     6,    11,    -1,
      84,    11,    -1,    87,    11,    -1,    21,    23,    11,    -1,
      22,    23,    11,    -1,    12,    87,    11,    -1,    12,    11,
      -1,    13,    11,    -1,    14,    11,    -1,   101,    -1,    83,
      -1,   107,    -1,   105,    -1,   110,    -1,    79,    -1,     8,
      80,    -1,     8,    80,     9,    23,    -1,     5,    80,     8,
      81,    -1,     5,    80,     8,    29,    -1,    23,    -1,    80,
      65,    23,    -1,    82,    -1,    81,    66,    82,    -1,    23,
      -1,    82,    65,    23,    -1,     7,    23,    67,    11,     3,
      75,     4,    -1,     7,    23,    63,    90,    68,    67,    11,
       3,    75,     4,    -1,    85,    69,    93,    -1,    85,    86,
      93,    -1,    88,    -1,    85,    66,    88,    -1,    39,    -1,
      40,    -1,    41,    -1,    42,    -1,    47,    -1,    48,    -1,
      43,    -1,    45,    -1,    44,    -1,    46,    -1,    49,    -1,
      50,    -1,    88,    -1,    63,    87,    68,    -1,    87,    27,
      87,    -1,    87,    28,    87,    -1,    87,    29,    87,    -1,
      87,    30,    87,    -1,    87,    31,    87,    -1,    87,    32,
      87,    -1,    87,    33,    87,    -1,    87,    36,    87,    -1,
      87,    35,    87,    -1,    87,    34,    87,    -1,    87,    37,
      87,    -1,    87,    38,    87,    -1,    87,    54,    87,    -1,
      87,    55,    87,    -1,    87,    62,    87,    -1,    87,    61,
      87,    -1,    87,    58,    87,    -1,    87,    60,    87,    -1,
      87,    59,    87,    -1,    87,    57,    87,    -1,    56,    87,
      -1,    28,    87,    -1,    89,    -1,    88,    65,    23,    -1,
      88,    63,    90,    68,    -1,    88,    63,    90,    66,    97,
      68,    -1,    88,    64,    87,    70,    -1,    25,    -1,    26,
      -1,    24,    -1,    91,    -1,    23,    -1,    92,    -1,    94,
      -1,    95,    -1,    53,    -1,    -1,    87,    -1,    90,    66,
      87,    -1,    51,    -1,    52,    -1,    64,    70,    -1,    64,
      93,    70,    -1,    87,    -1,    93,    66,    87,    -1,    71,
      93,    72,    -1,    71,    72,    -1,    71,    96,    72,    -1,
      99,    -1,    96,    66,    99,    -1,    98,    -1,    97,    66,
      98,    -1,    23,    69,    87,    -1,   100,    67,    87,    -1,
      24,    -1,    25,    -1,    26,    -1,    91,    -1,    -1,    10,
      23,    63,   103,    68,    67,    11,   102,     3,    75,     4,
      -1,    -1,   104,    -1,   103,    66,   104,    -1,    23,    -1,
      23,    69,    87,    -1,    -1,    15,    63,    87,    68,    67,
      11,   106,     3,    75,     4,    -1,    18,    87,    67,    11,
       3,    75,     4,   108,   109,    -1,    -1,    19,    87,    67,
      11,     3,    75,     4,   108,    -1,    -1,    20,    67,    11,
       3,    75,     4,    -1,    -1,    16,    23,    17,    88,    67,
      11,   111,     3,    75,     4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    80,    80,    81,    86,    87,    91,    92,    93,    94,
      98,    99,   100,   101,   102,   103,   104,   105,   109,   110,
     111,   112,   113,   114,   119,   120,   121,   122,   126,   127,
     131,   132,   136,   137,   141,   142,   146,   147,   151,   152,
     157,   157,   157,   157,   157,   157,   157,   158,   158,   158,
     158,   158,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   195,   196,   197,   198,
     199,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     215,   216,   217,   221,   222,   226,   227,   231,   232,   236,
     240,   241,   245,   246,   250,   251,   255,   259,   263,   264,
     265,   266,   272,   271,   282,   283,   284,   288,   289,   293,
     293,   302,   309,   310,   317,   318,   323,   322
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INDENT", "DEDENT", "FROM", "PASS",
  "CLASS", "IMPORT", "AS", "FUNCTION", "NEWLINE", "RETURN", "CONTINUE",
  "BREAK", "WHILE", "FOR", "IN", "IF", "ELIF", "ELSE", "GLOBAL",
  "NONLOCAL", "VARIABLE", "STRING", "INTEGER", "DECIMAL", "ADD", "SUB",
  "MUL", "DIV", "MOD", "FLDIV", "EXP", "BITAND", "BITOR", "BITXOR",
  "L_SHIFT", "R_SHIFT", "PLUS_EQ", "MINUS_EQ", "DIV_EQ", "MUL_EQ",
  "MOD_EQ", "L_SHIFT_EQ", "R_SHIFT_EQ", "XOR_EQ", "FLDIV_EQ", "EXP_EQ",
  "AND_EQ", "OR_EQ", "TRUE", "FALSE", "NONE", "LOGIC_AND", "LOGIC_OR",
  "INVERT", "EQ", "NE", "LT", "GT", "GE", "LE", "'('", "'['", "'.'", "','",
  "':'", "')'", "'='", "']'", "'{'", "'}'", "$accept", "program",
  "stmt_list", "statement", "simple_stmt", "compound_stmt", "import",
  "module", "import_items", "import_name", "class", "assignment",
  "left_side", "equals", "value", "postfix", "term", "args", "boolean",
  "list", "values", "set", "dict", "dict_items", "kwargs", "kwarg", "pair",
  "key", "function", "@1", "param_list", "param_item", "while_stmt", "@2",
  "if_stmt", "elif_chain", "else_block", "for_stmt", "@3", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,    40,    91,    46,    44,    58,    41,    61,
      93,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    73,    74,    74,    75,    75,    76,    76,    76,    76,
      77,    77,    77,    77,    77,    77,    77,    77,    78,    78,
      78,    78,    78,    78,    79,    79,    79,    79,    80,    80,
      81,    81,    82,    82,    83,    83,    84,    84,    85,    85,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    88,    88,    88,    88,
      88,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      90,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      95,    95,    96,    96,    97,    97,    98,    99,   100,   100,
     100,   100,   102,   101,   103,   103,   103,   104,   104,   106,
     105,   107,   108,   108,   109,   109,   111,   110
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     1,     1,     2,
       2,     2,     3,     3,     3,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     1,     3,
       1,     3,     1,     3,     7,    10,     3,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     1,     3,     4,     6,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     3,     1,     1,     2,     3,     1,     3,     3,
       2,     3,     1,     3,     1,     3,     3,     3,     1,     1,
       1,     1,     0,    11,     0,     1,     3,     1,     3,     0,
      10,     9,     0,     8,     0,     6,     0,    10
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     8,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    83,    81,    82,     0,
      93,    94,    89,     0,     0,     0,     0,     0,     3,     4,
       6,     7,    23,    19,     0,     0,     0,    52,    76,    84,
      86,    87,    88,    18,    21,    20,    22,    28,     0,     9,
       0,    24,     0,    15,     0,    52,    16,    17,     0,     0,
       0,     0,     0,    75,    74,     0,    95,    97,     0,    83,
      81,    82,   100,    84,     0,     0,   102,     0,     1,     5,
      10,    40,    41,    42,    43,    46,    48,    47,    49,    44,
      45,    50,    51,     0,     0,     0,    11,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,     0,     0,
       0,     0,    90,     0,     0,   114,    14,     0,     0,     0,
      12,    13,    53,     0,    96,    99,     0,   101,     0,    39,
      36,    37,    54,    55,    56,    57,    58,    59,    60,    63,
      62,    61,    64,    65,    66,    67,    73,    70,    72,    71,
      69,    68,    91,     0,     0,    77,    32,    27,    26,    30,
      29,     0,     0,    25,   117,     0,   115,     0,     0,     0,
      98,   108,   109,   110,   111,   103,   107,     0,    78,    80,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,    92,     0,   104,    31,    33,     0,     0,   118,
     116,     0,   119,   126,     0,     0,     0,    79,     0,    34,
     112,     0,     0,   122,   106,     0,   105,     0,     0,     0,
       0,     0,   124,     0,     0,     0,     0,     0,     0,   121,
      35,     0,   120,   127,     0,     0,   113,     0,     0,     0,
       0,     0,     0,   122,   125,   123
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    27,    28,    29,    30,    31,    32,    48,   168,   169,
      33,    34,    35,    95,    36,    55,    38,   163,    39,    40,
      68,    41,    42,    75,   203,   204,    76,    77,    43,   228,
     175,   176,    44,   221,    45,   232,   239,    46,   222
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -188
static const yytype_int16 yypact[] =
{
     683,   -13,    27,    23,   -13,    25,  -188,     1,    28,    47,
       5,    36,   803,    43,    48,  -188,  -188,  -188,  -188,   803,
    -188,  -188,  -188,   803,   803,   787,   781,    82,   683,  -188,
    -188,  -188,  -188,  -188,    76,   127,   729,   245,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,    -5,  -188,
     -52,    -4,    26,  -188,   741,    21,  -188,  -188,   803,   100,
     958,   111,   113,  -188,  -188,   904,  -188,  1013,   -29,    58,
      59,    60,  -188,    62,   -58,   -16,  -188,    64,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,   845,   803,   803,  -188,   803,   803,   803,
     803,   803,   803,   803,   803,   803,   803,   803,   803,   803,
     803,   803,   803,   803,   803,   803,   803,   803,   803,   109,
      44,   110,   803,   123,   114,   120,  -188,   946,   845,   125,
    -188,  -188,  -188,   803,  -188,  -188,    -7,  -188,   803,    21,
      85,    85,    45,    45,   121,   121,   121,   121,   121,  1145,
    1097,  1109,     3,     3,  1061,  1049,  1157,  1157,  1157,  1157,
    1157,  1157,  1013,   -17,   890,  -188,  -188,  -188,    87,    96,
    -188,    22,   160,  -188,    95,    52,  -188,    98,    16,   175,
    1013,  -188,  -188,  -188,  -188,  -188,  1013,   837,  -188,  -188,
     156,   157,   803,   116,   683,   803,   120,   124,   170,   184,
     683,   130,  1013,    55,  -188,    96,  -188,   190,   134,  1013,
    -188,   191,  -188,  -188,   249,   803,   180,  -188,   201,  -188,
    -188,   203,   208,   194,  1013,   130,  -188,   683,   213,   683,
     683,   803,   197,   311,   683,   373,   435,   999,   151,  -188,
    -188,   497,  -188,  -188,   209,   210,  -188,   216,   219,   683,
     683,   559,   621,   194,  -188,  -188
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -188,  -188,  -187,   -26,  -188,  -188,  -188,   220,  -188,    33,
    -188,  -188,  -188,  -188,    -3,     0,  -188,   115,   -20,  -188,
     -25,  -188,  -188,  -188,  -188,    15,   102,  -188,  -188,  -188,
    -188,    46,  -188,  -188,  -188,   -21,  -188,  -188,  -188
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -112
static const yytype_int16 yytable[] =
{
      37,    74,    79,   120,    54,   124,    73,   208,   133,    60,
      47,   122,    53,   214,   135,   123,    63,   181,   182,   183,
      64,    65,    67,    67,    15,    16,    17,    18,    37,    19,
      97,    98,    99,   100,   101,   102,   103,   133,    49,    56,
     233,   134,   235,   236,    20,    21,    50,   241,    52,   187,
     136,   188,    20,    21,    22,   127,   137,    23,    57,    59,
     121,   121,   251,   252,    24,    25,    61,   166,    58,   140,
     141,    62,    26,   167,    99,   100,   101,   102,   103,   117,
     118,   119,    78,   199,   117,   118,   119,    80,   192,   125,
     193,    67,    67,   139,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   164,   184,   128,   196,   162,
     197,   216,   130,   217,   131,  -108,  -109,  -110,   178,  -111,
     180,   138,   165,   170,   172,   186,   179,   173,   219,     1,
       2,     3,     4,   174,     5,     6,     7,     8,     9,    10,
      11,   133,    12,   190,   103,    13,    14,    15,    16,    17,
      18,   191,    19,   194,   195,   198,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,   200,   166,
     206,   212,    79,   207,   202,    20,    21,    22,    79,   202,
      23,   211,   209,    93,    37,   213,    94,    24,    25,   215,
      37,   218,   220,   225,   227,    26,   229,    79,    37,    79,
      79,   230,   224,   231,    37,    79,   234,   238,   245,   249,
     247,   248,   250,   205,    51,    79,    79,    37,   237,    37,
      37,   226,   255,    37,    37,    37,    37,   171,   185,     0,
       0,    37,   210,     0,     0,     0,     0,     0,     0,    37,
      37,    37,    37,   223,     1,     2,     3,     4,     0,     5,
       6,     7,     8,     9,    10,    11,     0,    12,     0,     0,
      13,    14,    15,    16,    17,    18,     0,    19,     0,     0,
       0,     0,     0,     0,   -38,   -38,   -38,   -38,   -38,   -38,
     -38,   -38,   -38,   -38,   -38,   -38,     0,     0,     0,     0,
      20,    21,    22,     0,     0,    23,     0,     0,   117,   118,
     119,   -38,    24,    25,   -38,   240,     1,     2,     3,     4,
      26,     5,     6,     7,     8,     9,    10,    11,     0,    12,
       0,     0,    13,    14,    15,    16,    17,    18,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    21,    22,     0,     0,    23,     0,     0,
       0,     0,     0,     0,    24,    25,     0,   242,     1,     2,
       3,     4,    26,     5,     6,     7,     8,     9,    10,    11,
       0,    12,     0,     0,    13,    14,    15,    16,    17,    18,
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    21,    22,     0,     0,    23,
       0,     0,     0,     0,     0,     0,    24,    25,     0,   243,
       1,     2,     3,     4,    26,     5,     6,     7,     8,     9,
      10,    11,     0,    12,     0,     0,    13,    14,    15,    16,
      17,    18,     0,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,    21,    22,     0,
       0,    23,     0,     0,     0,     0,     0,     0,    24,    25,
       0,   246,     1,     2,     3,     4,    26,     5,     6,     7,
       8,     9,    10,    11,     0,    12,     0,     0,    13,    14,
      15,    16,    17,    18,     0,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,    21,
      22,     0,     0,    23,     0,     0,     0,     0,     0,     0,
      24,    25,     0,   253,     1,     2,     3,     4,    26,     5,
       6,     7,     8,     9,    10,    11,     0,    12,     0,     0,
      13,    14,    15,    16,    17,    18,     0,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      20,    21,    22,     0,     0,    23,     0,     0,     0,     0,
       0,     0,    24,    25,     0,   254,     1,     2,     3,     4,
      26,     5,     6,     7,     8,     9,    10,    11,     0,    12,
       0,     0,    13,    14,    15,    16,    17,    18,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    21,    22,     0,     0,    23,     0,     0,
       0,     0,     0,     0,    24,    25,     0,     0,     1,     2,
       3,     4,    26,     5,     6,     7,     8,     9,    10,    11,
       0,    12,     0,     0,    13,    14,    15,    16,    17,    18,
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    21,    22,     0,     0,    23,
      96,     0,     0,     0,     0,     0,    24,    25,     0,     0,
       0,     0,   126,     0,    26,     0,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
       0,     0,     0,   109,   110,     0,   111,   112,   113,   114,
     115,   116,     0,     0,     0,   109,   110,     0,   111,   112,
     113,   114,   115,   116,    15,    69,    70,    71,     0,    19,
      15,    16,    17,    18,     0,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    15,    16,    17,    18,
       0,    19,    20,    21,    22,     0,     0,    23,    20,    21,
      22,     0,     0,    23,    24,    25,     0,     0,     0,     0,
      24,    25,    26,    72,    20,    21,    22,    66,    26,    23,
     201,    16,    17,    18,     0,    19,    24,    25,    15,    16,
      17,    18,     0,     0,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,    21,
      22,     0,     0,    23,     0,     0,    20,    21,    22,     0,
      24,    25,     0,     0,     0,     0,     0,     0,    26,    25,
       0,     0,     0,     0,     0,     0,    26,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,     0,
       0,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,     0,   109,   110,     0,   111,   112,   113,
     114,   115,   116,     0,     0,     0,     0,     0,   109,   110,
     189,   111,   112,   113,   114,   115,   116,     0,     0,     0,
       0,     0,   132,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,     0,     0,     0,
     109,   110,     0,   111,   112,   113,   114,   115,   116,     0,
       0,     0,   109,   110,   177,   111,   112,   113,   114,   115,
     116,     0,     0,     0,     0,   129,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,     0,     0,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,     0,   109,   110,     0,   111,   112,   113,   114,
     115,   116,     0,     0,     0,     0,   244,   109,   110,     0,
     111,   112,   113,   114,   115,   116,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
       0,     0,     0,   109,     0,     0,   111,   112,   113,   114,
     115,   116,     0,     0,     0,     0,     0,     0,   111,   112,
     113,   114,   115,   116,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   106,   107,   108,    97,    98,    99,   100,
     101,   102,   103,   104,     0,     0,   107,   108,     0,     0,
       0,     0,     0,     0,   111,   112,   113,   114,   115,   116,
       0,     0,     0,     0,     0,     0,   111,   112,   113,   114,
     115,   116,    97,    98,    99,   100,   101,   102,   103,     0,
       0,     0,   107,   108,    97,    98,    99,   100,   101,   102,
     103,     0,     0,     0,   107,   108,     0,     0,     0,     0,
       0,     0,   111,   112,   113,   114,   115,   116,     0,     0,
       0,     0,     0,     0,  -112,  -112,  -112,  -112,  -112,  -112
};

static const yytype_int16 yycheck[] =
{
       0,    26,    28,     8,     7,     9,    26,   194,    66,    12,
      23,    63,    11,   200,    72,    67,    19,    24,    25,    26,
      23,    24,    25,    26,    23,    24,    25,    26,    28,    28,
      27,    28,    29,    30,    31,    32,    33,    66,    11,    11,
     227,    70,   229,   230,    51,    52,    23,   234,    23,    66,
      66,    68,    51,    52,    53,    58,    72,    56,    11,    23,
      65,    65,   249,   250,    63,    64,    23,    23,    63,    94,
      95,    23,    71,    29,    29,    30,    31,    32,    33,    63,
      64,    65,     0,    67,    63,    64,    65,    11,    66,    63,
      68,    94,    95,    93,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   136,    17,    66,   122,
      68,    66,    11,    68,    11,    67,    67,    67,   128,    67,
     133,    67,    23,    23,    11,   138,    11,    23,     4,     5,
       6,     7,     8,    23,    10,    11,    12,    13,    14,    15,
      16,    66,    18,    66,    33,    21,    22,    23,    24,    25,
      26,    65,    28,     3,    69,    67,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     3,    23,
      23,    11,   208,    67,   187,    51,    52,    53,   214,   192,
      56,    67,   195,    66,   194,    11,    69,    63,    64,    69,
     200,    11,    11,    23,     3,    71,     3,   233,   208,   235,
     236,     3,   215,    19,   214,   241,     3,    20,    67,     3,
      11,    11,     3,   190,     4,   251,   252,   227,   231,   229,
     230,   216,   253,   233,   234,   235,   236,   122,   136,    -1,
      -1,   241,   196,    -1,    -1,    -1,    -1,    -1,    -1,   249,
     250,   251,   252,     4,     5,     6,     7,     8,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    -1,    -1,
      21,    22,    23,    24,    25,    26,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
      51,    52,    53,    -1,    -1,    56,    -1,    -1,    63,    64,
      65,    66,    63,    64,    69,     4,     5,     6,     7,     8,
      71,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      -1,    -1,    21,    22,    23,    24,    25,    26,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    52,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,     4,     5,     6,
       7,     8,    71,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    -1,    -1,    21,    22,    23,    24,    25,    26,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    52,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,     4,
       5,     6,     7,     8,    71,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    -1,    -1,    21,    22,    23,    24,
      25,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,     4,     5,     6,     7,     8,    71,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    21,    22,
      23,    24,    25,    26,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,     4,     5,     6,     7,     8,    71,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    -1,    -1,
      21,    22,    23,    24,    25,    26,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    52,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,     4,     5,     6,     7,     8,
      71,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      -1,    -1,    21,    22,    23,    24,    25,    26,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    52,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,     5,     6,
       7,     8,    71,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    -1,    -1,    21,    22,    23,    24,    25,    26,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    52,    53,    -1,    -1,    56,
      11,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    11,    -1,    71,    -1,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    54,    55,    -1,    57,    58,
      59,    60,    61,    62,    23,    24,    25,    26,    -1,    28,
      23,    24,    25,    26,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      -1,    28,    51,    52,    53,    -1,    -1,    56,    51,    52,
      53,    -1,    -1,    56,    63,    64,    -1,    -1,    -1,    -1,
      63,    64,    71,    72,    51,    52,    53,    70,    71,    56,
      23,    24,    25,    26,    -1,    28,    63,    64,    23,    24,
      25,    26,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
      53,    -1,    -1,    56,    -1,    -1,    51,    52,    53,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    -1,    54,    55,    -1,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,    -1,    54,    55,
      70,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      -1,    -1,    68,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      54,    55,    -1,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    54,    55,    68,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    67,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    -1,    54,    55,    -1,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    67,    54,    55,    -1,
      57,    58,    59,    60,    61,    62,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    54,    -1,    -1,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    62,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    38,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    37,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    37,    38,    27,    28,    29,    30,    31,    32,
      33,    -1,    -1,    -1,    37,    38,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,     7,     8,    10,    11,    12,    13,    14,
      15,    16,    18,    21,    22,    23,    24,    25,    26,    28,
      51,    52,    53,    56,    63,    64,    71,    74,    75,    76,
      77,    78,    79,    83,    84,    85,    87,    88,    89,    91,
      92,    94,    95,   101,   105,   107,   110,    23,    80,    11,
      23,    80,    23,    11,    87,    88,    11,    11,    63,    23,
      87,    23,    23,    87,    87,    87,    70,    87,    93,    24,
      25,    26,    72,    91,    93,    96,    99,   100,     0,    76,
      11,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    66,    69,    86,    11,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    54,
      55,    57,    58,    59,    60,    61,    62,    63,    64,    65,
       8,    65,    63,    67,     9,    63,    11,    87,    17,    67,
      11,    11,    68,    66,    70,    72,    66,    72,    67,    88,
      93,    93,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    90,    87,    23,    23,    29,    81,    82,
      23,    90,    11,    23,    23,   103,   104,    68,    88,    11,
      87,    24,    25,    26,    91,    99,    87,    66,    68,    70,
      66,    65,    66,    68,     3,    69,    66,    68,    67,    67,
       3,    23,    87,    97,    98,    82,    23,    67,    75,    87,
     104,    67,    11,    11,    75,    69,    66,    68,    11,     4,
      11,   106,   111,     4,    87,    23,    98,     3,   102,     3,
       3,    19,   108,    75,     3,    75,    75,    87,    20,   109,
       4,    75,     4,     4,    67,    67,     4,    11,    11,     3,
       3,    75,    75,     4,     4,   108
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 80 "parser.y"
    { executeProgram(createList(0)); }
    break;

  case 3:
#line 81 "parser.y"
    { executeProgram((yyvsp[(1) - (1)].generic)); }
    break;

  case 4:
#line 86 "parser.y"
    { (yyval.generic) = createList(RUNTIME_SIZE); if ((yyvsp[(1) - (1)].generic)) append((Stmt *)(yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 5:
#line 87 "parser.y"
    { if ((yyvsp[(2) - (2)].generic)) append((Stmt *)(yyvsp[(2) - (2)].generic), (yyvsp[(1) - (2)].generic)); (yyval.generic) = (yyvsp[(1) - (2)].generic);  }
    break;

  case 6:
#line 91 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 7:
#line 92 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 8:
#line 93 "parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 9:
#line 94 "parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 10:
#line 98 "parser.y"
    { (yyval.generic) = createStatement(STMT_ASSIGNMENT, (yyvsp[(1) - (2)].generic), yylineno); }
    break;

  case 11:
#line 99 "parser.y"
    { (yyval.generic) = createStatement(STMT_EXPRESSION, (yyvsp[(1) - (2)].generic), yylineno); }
    break;

  case 12:
#line 100 "parser.y"
    { (yyval.generic) = createStatement(STMT_GLOBAL, createVarDecl(DECL_GLOBAL, (yyvsp[(2) - (3)].generic)), yylineno); }
    break;

  case 13:
#line 101 "parser.y"
    { (yyval.generic) = createStatement(STMT_NONLOCAL, createVarDecl(DECL_NONLOCAL, (yyvsp[(2) - (3)].generic)), yylineno); }
    break;

  case 14:
#line 102 "parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn((yyvsp[(2) - (3)].generic), funcDepth), yylineno); }
    break;

  case 15:
#line 103 "parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn(NULL, funcDepth), yylineno); }
    break;

  case 16:
#line 104 "parser.y"
    { (yyval.generic) = createStatement(STMT_CONTINUE, createJump(loopDepth, 0), yylineno); }
    break;

  case 17:
#line 105 "parser.y"
    { (yyval.generic) = createStatement(STMT_CONTINUE, createJump(loopDepth, 1), yylineno); }
    break;

  case 18:
#line 109 "parser.y"
    { (yyval.generic) = createStatement(STMT_FUNCTION, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 19:
#line 110 "parser.y"
    { (yyval.generic) = createStatement(STMT_CLASS, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 20:
#line 111 "parser.y"
    { (yyval.generic) = createStatement(STMT_FLOW, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 21:
#line 112 "parser.y"
    { (yyval.generic) = createStatement(STMT_WHILE, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 22:
#line 113 "parser.y"
    { (yyval.generic) = createStatement(STMT_FOR, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 23:
#line 114 "parser.y"
    { (yyval.generic) = createStatement(STMT_IMPORT, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 24:
#line 119 "parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (2)].generic), NULL, NULL, 0); }
    break;

  case 25:
#line 120 "parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (4)].generic), NULL, (yyvsp[(4) - (4)].generic), 0); }
    break;

  case 26:
#line 121 "parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic), NULL, 0); }
    break;

  case 27:
#line 122 "parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (4)].generic), NULL, NULL, 1); freeAST((yyvsp[(4) - (4)].generic)); }
    break;

  case 28:
#line 126 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 29:
#line 127 "parser.y"
    { (yyval.generic) = createData(TYPE_STR, getFileName((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic))); }
    break;

  case 30:
#line 131 "parser.y"
    { (yyval.generic) = createList(PARSE_SIZE); append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 31:
#line 132 "parser.y"
    { append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 32:
#line 136 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 33:
#line 137 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, attribute_create((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 34:
#line 141 "parser.y"
    {  (yyval.generic) = createClass((yyvsp[(2) - (7)].generic), (yyvsp[(6) - (7)].generic), NULL); }
    break;

  case 35:
#line 142 "parser.y"
    { (yyval.generic) = createClass((yyvsp[(2) - (10)].generic), (yyvsp[(9) - (10)].generic), (yyvsp[(4) - (10)].generic));}
    break;

  case 36:
#line 146 "parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), NULL); }
    break;

  case 37:
#line 147 "parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), (yyvsp[(2) - (3)].generic)); }
    break;

  case 38:
#line 151 "parser.y"
    { (yyval.generic) = createList(PARSE_SIZE); append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 39:
#line 152 "parser.y"
    { append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 52:
#line 162 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 53:
#line 163 "parser.y"
    { (yyval.generic) = (yyvsp[(2) - (3)].generic); }
    break;

  case 54:
#line 164 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 55:
#line 165 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 56:
#line 166 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 57:
#line 167 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 58:
#line 168 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 59:
#line 169 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 60:
#line 170 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 61:
#line 171 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 62:
#line 172 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 63:
#line 173 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 64:
#line 174 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 65:
#line 175 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 66:
#line 176 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 67:
#line 177 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 68:
#line 178 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 69:
#line 179 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 70:
#line 180 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 71:
#line 181 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 72:
#line 182 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 73:
#line 183 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 74:
#line 184 "parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 75:
#line 186 "parser.y"
    {
      int zero = 0; 
      Instance *inst = createInstance(TYPE_INT, &zero);
      Ast *left = createASTnode(createData(TYPE_INSTANCE, inst));
      (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), left, (yyvsp[(2) - (2)].generic));
   }
    break;

  case 76:
#line 195 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 77:
#line 196 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, attribute_create((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 78:
#line 197 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic), NULL))); }
    break;

  case 79:
#line 198 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (6)].generic), (yyvsp[(3) - (6)].generic), (yyvsp[(5) - (6)].generic)))); }
    break;

  case 80:
#line 199 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INDEX, createIndexed((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic)))); }
    break;

  case 81:
#line 203 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 82:
#line 204 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 83:
#line 205 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 84:
#line 206 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 85:
#line 207 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 86:
#line 208 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INSTANCE, (yyvsp[(1) - (1)].generic))); }
    break;

  case 87:
#line 209 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INSTANCE, (yyvsp[(1) - (1)].generic))); }
    break;

  case 88:
#line 210 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INSTANCE, (yyvsp[(1) - (1)].generic))); }
    break;

  case 89:
#line 211 "parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INSTANCE, (yyvsp[(1) - (1)].generic))); }
    break;

  case 90:
#line 215 "parser.y"
    { (yyval.generic) = createList(0); }
    break;

  case 91:
#line 216 "parser.y"
    { (yyval.generic) = createList(PARSE_SIZE); append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 92:
#line 217 "parser.y"
    { append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 93:
#line 221 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 94:
#line 222 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 95:
#line 226 "parser.y"
    { (yyval.generic) = createInstance(list_create(PARSE_SIZE)); }
    break;

  case 96:
#line 227 "parser.y"
    { (yyval.generic) = createInstance(TYPE_LIST, (yyvsp[(2) - (3)].generic)); }
    break;

  case 97:
#line 231 "parser.y"
    { (yyval.generic) = createList(PARSE_SIZE); append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 98:
#line 232 "parser.y"
    { append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 99:
#line 236 "parser.y"
    { (yyval.generic) = createInstance(TYPE_SET, (yyvsp[(2) - (3)].generic)); }
    break;

  case 100:
#line 240 "parser.y"
    { (yyval.generic) = createInstance(TYPE_DICT, list_create(PARSE_SIZE)); }
    break;

  case 101:
#line 241 "parser.y"
    { (yyval.generic) = createInstane(TYPE_DICT, (yyvsp[(2) - (3)].generic)); }
    break;

  case 102:
#line 245 "parser.y"
    { (yyval.generic) = list_create(PARSE_SIZE); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 103:
#line 246 "parser.y"
    { append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 104:
#line 250 "parser.y"
    { (yyval.generic) = list_create(PARSE_SIZE); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 105:
#line 251 "parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 106:
#line 255 "parser.y"
    { (yyval.generic) = createPair(createData(TYPE_STR, (yyvsp[(1) - (3)].generic)), (yyvsp[(3) - (3)].generic)); }
    break;

  case 107:
#line 259 "parser.y"
    { (yyval.generic) = createPair((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 108:
#line 263 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 109:
#line 264 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 110:
#line 265 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 111:
#line 266 "parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 112:
#line 272 "parser.y"
    { funcDepth++; }
    break;

  case 113:
#line 274 "parser.y"
    { 
        funcDepth = funcDepth > 0 ? funcDepth - 1 : 0;
        (yyval.generic) = createFunction((yyvsp[(2) - (11)].generic), (yyvsp[(4) - (11)].generic), (yyvsp[(10) - (11)].generic));
     }
    break;

  case 114:
#line 282 "parser.y"
    { (yyval.generic) = createList(0); }
    break;

  case 115:
#line 283 "parser.y"
    { (yyval.generic) = createList(PARSE_SIZE); append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 116:
#line 284 "parser.y"
    { append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 117:
#line 288 "parser.y"
    { (yyval.generic) = createParamInfo((yyvsp[(1) - (1)].generic), NULL); }
    break;

  case 118:
#line 289 "parser.y"
    { (yyval.generic) = createParamInfo((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 119:
#line 293 "parser.y"
    { loopDepth++; }
    break;

  case 120:
#line 295 "parser.y"
    {
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        (yyval.generic) = createWhileLoop((yyvsp[(3) - (10)].generic), (yyvsp[(9) - (10)].generic));
     }
    break;

  case 121:
#line 303 "parser.y"
    { 
      (yyval.generic) = createFlow((yyvsp[(2) - (9)].generic), (yyvsp[(6) - (9)].generic), (yyvsp[(8) - (9)].generic), (yyvsp[(9) - (9)].generic)); 
    }
    break;

  case 122:
#line 309 "parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 123:
#line 311 "parser.y"
    { 
      (yyval.generic) = createFlow((yyvsp[(2) - (8)].generic), (yyvsp[(6) - (8)].generic), (yyvsp[(8) - (8)].generic), NULL); 
    }
    break;

  case 124:
#line 317 "parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 125:
#line 318 "parser.y"
    { (yyval.generic) = createDefaultFlow((yyvsp[(5) - (6)].generic)); }
    break;

  case 126:
#line 323 "parser.y"
    { 
      loopDepth++; 
     }
    break;

  case 127:
#line 327 "parser.y"
    { 
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        (yyval.generic) = createForLoop((yyvsp[(2) - (10)].generic), (yyvsp[(4) - (10)].generic), (yyvsp[(9) - (10)].generic)); 
     }
    break;


/* Line 1267 of yacc.c.  */
#line 2478 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 333 "parser.y"


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
