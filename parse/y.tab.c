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




/* Copy the first part of user declarations.  */
#line 1 "./parse/parser.y"

   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include "../lib/type.h"
   #include "../lib/list.h"
   #include "../lib/set.h"
   #include "../lib/dict.h"
   #include "../lib/utils.h"
   #include "../lib/str.h"
   #include "../lib/eval.h"
   #include "../lib/exec.h"
   #include "../lib/error.h"
   #include "../lib/module.h"
   #include "../lib/maloc.h"
   #include "../lib/daloc.h"
   #include "../lib/build.h"
   #include "../lib/error.h"


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

   ThrownError g_thrown;
   int repl_mode = 0;


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
#line 37 "./parse/parser.y"
{
   void *generic;
}
/* Line 193 of yacc.c.  */
#line 268 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 281 "y.tab.c"

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
#define YYFINAL  84
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1713

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  144
/* YYNRULES -- Number of states.  */
#define YYNSTATES  302

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      67,    72,     2,     2,    70,     2,    69,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    71,     2,
       2,    73,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    68,     2,    74,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    75,     2,    76,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    13,    15,    17,
      20,    23,    26,    30,    34,    38,    41,    44,    47,    49,
      51,    53,    55,    57,    59,    61,    64,    69,    74,    79,
      81,    85,    87,    91,    93,    97,   105,   116,   120,   124,
     126,   130,   132,   134,   136,   138,   140,   142,   144,   146,
     148,   150,   152,   154,   156,   160,   164,   168,   172,   176,
     180,   184,   188,   192,   196,   200,   204,   208,   212,   216,
     220,   224,   228,   232,   236,   240,   243,   246,   249,   251,
     256,   263,   267,   272,   275,   277,   279,   281,   283,   285,
     287,   289,   291,   293,   294,   296,   300,   302,   304,   307,
     311,   319,   325,   326,   328,   329,   331,   333,   337,   341,
     344,   348,   350,   354,   356,   360,   364,   368,   370,   372,
     374,   376,   377,   389,   390,   392,   396,   398,   402,   403,
     414,   424,   425,   434,   435,   442,   443,   454,   457,   460,
     464,   471,   479,   486,   495
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      78,     0,    -1,    -1,    79,    -1,    80,    -1,    79,    80,
      -1,    81,    -1,    82,    -1,    11,    -1,     6,    11,    -1,
      88,    11,    -1,    91,    11,    -1,    24,    26,    11,    -1,
      25,    26,    11,    -1,    12,    91,    11,    -1,    12,    11,
      -1,    13,    11,    -1,    14,    11,    -1,   108,    -1,    87,
      -1,   114,    -1,   112,    -1,   117,    -1,    83,    -1,   119,
      -1,     8,    84,    -1,     8,    84,     9,    26,    -1,     5,
      84,     8,    85,    -1,     5,    84,     8,    32,    -1,    26,
      -1,    84,    69,    26,    -1,    86,    -1,    85,    70,    86,
      -1,    26,    -1,    86,    69,    26,    -1,     7,    26,    71,
      11,     3,    79,     4,    -1,     7,    26,    67,    94,    72,
      71,    11,     3,    79,     4,    -1,    89,    73,   100,    -1,
      89,    90,   100,    -1,    92,    -1,    89,    70,    92,    -1,
      43,    -1,    44,    -1,    45,    -1,    46,    -1,    51,    -1,
      52,    -1,    47,    -1,    49,    -1,    48,    -1,    50,    -1,
      53,    -1,    54,    -1,    92,    -1,    67,    91,    72,    -1,
      91,    30,    91,    -1,    91,    31,    91,    -1,    91,    32,
      91,    -1,    91,    33,    91,    -1,    91,    34,    91,    -1,
      91,    35,    91,    -1,    91,    36,    91,    -1,    91,    39,
      91,    -1,    91,    38,    91,    -1,    91,    37,    91,    -1,
      91,    40,    91,    -1,    91,    41,    91,    -1,    91,    58,
      91,    -1,    91,    59,    91,    -1,    91,    66,    91,    -1,
      91,    65,    91,    -1,    91,    62,    91,    -1,    91,    64,
      91,    -1,    91,    63,    91,    -1,    91,    61,    91,    -1,
      60,    91,    -1,    42,    91,    -1,    31,    91,    -1,    93,
      -1,    92,    67,    94,    72,    -1,    92,    67,    94,    70,
     104,    72,    -1,    92,    69,    26,    -1,    92,    68,    91,
      74,    -1,    92,    97,    -1,    28,    -1,    29,    -1,    27,
      -1,    95,    -1,    26,    -1,    96,    -1,   101,    -1,   102,
      -1,    57,    -1,    -1,    91,    -1,    94,    70,    91,    -1,
      55,    -1,    56,    -1,    68,    74,    -1,    68,   100,    74,
      -1,    68,    98,    71,    99,    71,    99,    74,    -1,    68,
      98,    71,    99,    74,    -1,    -1,    91,    -1,    -1,    91,
      -1,    91,    -1,   100,    70,    91,    -1,    75,   100,    76,
      -1,    75,    76,    -1,    75,   103,    76,    -1,   106,    -1,
     103,    70,   106,    -1,   105,    -1,   104,    70,   105,    -1,
      26,    73,    91,    -1,   107,    71,    91,    -1,    27,    -1,
      28,    -1,    29,    -1,    95,    -1,    -1,    10,    26,    67,
     110,    72,    71,    11,   109,     3,    79,     4,    -1,    -1,
     111,    -1,   110,    70,   111,    -1,    26,    -1,    26,    73,
      91,    -1,    -1,    18,    67,    91,    72,    71,    11,   113,
       3,    79,     4,    -1,    21,    91,    71,    11,     3,    79,
       4,   115,   116,    -1,    -1,    22,    91,    71,    11,     3,
      79,     4,   115,    -1,    -1,    23,    71,    11,     3,    79,
       4,    -1,    -1,    19,    26,    20,    92,    71,    11,   118,
       3,    79,     4,    -1,   120,   121,    -1,   120,   122,    -1,
     120,   121,   122,    -1,    15,    71,    11,     3,    79,     4,
      -1,    16,    26,    71,    11,     3,    79,     4,    -1,    16,
      71,    11,     3,    79,     4,    -1,    16,    26,     9,    26,
      71,     3,    79,     4,    -1,    17,    71,    11,     3,    79,
       4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    85,    85,    86,    91,    92,    96,    97,    98,    99,
     103,   104,   105,   106,   107,   108,   109,   110,   114,   115,
     116,   117,   118,   119,   120,   125,   126,   127,   128,   132,
     133,   137,   138,   142,   143,   147,   148,   152,   153,   157,
     158,   163,   163,   163,   163,   163,   163,   163,   164,   164,
     164,   164,   164,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   200,   201,
     202,   203,   204,   205,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   221,   222,   223,   227,   228,   232,   233,
     237,   238,   242,   243,   247,   248,   253,   254,   258,   262,
     263,   267,   268,   272,   273,   277,   281,   285,   286,   287,
     288,   294,   293,   304,   305,   306,   310,   311,   315,   315,
     324,   331,   332,   339,   340,   345,   344,   356,   357,   358,
     362,   366,   367,   368,   372
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INDENT", "DEDENT", "FROM", "PASS",
  "CLASS", "IMPORT", "AS", "FUNCTION", "NL", "RETURN", "CONTINUE", "BREAK",
  "TRY", "CATCH", "FINALLY", "WHILE", "FOR", "IN", "IF", "ELIF", "ELSE",
  "GLOBAL", "NONLOCAL", "VARIABLE", "STRING", "INTEGER", "DECIMAL", "ADD",
  "SUB", "MUL", "DIV", "MOD", "FLDIV", "EXP", "BITAND", "BITOR", "BITXOR",
  "L_SHIFT", "R_SHIFT", "BITNOT", "PLUS_EQ", "MINUS_EQ", "DIV_EQ",
  "MUL_EQ", "MOD_EQ", "L_SHIFT_EQ", "R_SHIFT_EQ", "XOR_EQ", "FLDIV_EQ",
  "EXP_EQ", "AND_EQ", "OR_EQ", "TRUE", "FALSE", "NONE", "LOGIC_AND",
  "LOGIC_OR", "INVERT", "EQ", "NE", "LT", "GT", "GE", "LE", "'('", "'['",
  "'.'", "','", "':'", "')'", "'='", "']'", "'{'", "'}'", "$accept",
  "program", "stmt_list", "statement", "simple_stmt", "compound_stmt",
  "import", "module", "import_items", "import_name", "class", "assignment",
  "left_side", "equals", "value", "postfix", "term", "args", "boolean",
  "list", "sliced_items", "slice_start", "slice_end", "values", "set",
  "dict", "dict_items", "kwargs", "kwarg", "pair", "key", "function", "@1",
  "param_list", "param_item", "while_stmt", "@2", "if_stmt", "elif_chain",
  "else_block", "for_stmt", "@3", "exception", "try_block", "catch_block",
  "final_block", 0
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
     315,   316,   317,   318,   319,   320,   321,    40,    91,    46,
      44,    58,    41,    61,    93,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    77,    78,    78,    79,    79,    80,    80,    80,    80,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    82,
      82,    82,    82,    82,    82,    83,    83,    83,    83,    84,
      84,    85,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    92,    92,
      92,    92,    92,    92,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    94,    94,    94,    95,    95,    96,    96,
      97,    97,    98,    98,    99,    99,   100,   100,   101,   102,
     102,   103,   103,   104,   104,   105,   106,   107,   107,   107,
     107,   109,   108,   110,   110,   110,   111,   111,   113,   112,
     114,   115,   115,   116,   116,   118,   117,   119,   119,   119,
     120,   121,   121,   121,   122
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     1,     1,     2,
       2,     2,     3,     3,     3,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     1,
       3,     1,     3,     1,     3,     7,    10,     3,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     1,     4,
       6,     3,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     3,     1,     1,     2,     3,
       7,     5,     0,     1,     0,     1,     1,     3,     3,     2,
       3,     1,     3,     1,     3,     3,     3,     1,     1,     1,
       1,     0,    11,     0,     1,     3,     1,     3,     0,    10,
       9,     0,     8,     0,     6,     0,    10,     2,     2,     3,
       6,     7,     6,     8,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     8,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    86,    84,    85,
       0,     0,    96,    97,    92,     0,     0,     0,     0,     0,
       3,     4,     6,     7,    23,    19,     0,     0,     0,    53,
      78,    87,    89,    90,    91,    18,    21,    20,    22,    24,
       0,    29,     0,     9,     0,    25,     0,    15,     0,    53,
      16,    17,     0,     0,     0,     0,     0,     0,    77,    76,
      75,     0,    98,   106,     0,    86,    84,    85,   109,    87,
       0,     0,   111,     0,     1,     5,    10,    41,    42,    43,
      44,    47,    49,    48,    50,    45,    46,    51,    52,     0,
       0,     0,    11,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,   102,     0,    83,     0,     0,   137,
     138,     0,     0,    93,     0,     0,   123,    14,     0,     0,
       0,     0,    12,    13,    54,     0,    99,   108,     0,   110,
       0,    40,    37,    38,    55,    56,    57,    58,    59,    60,
      61,    64,    63,    62,    65,    66,    67,    68,    74,    71,
      73,    72,    70,    69,    94,     0,   103,     0,    81,     0,
       0,     0,   139,    33,    28,    27,    31,    30,     0,     0,
      26,   126,     0,   124,     0,     0,     0,     0,   107,   117,
     118,   119,   120,   112,   116,     0,    79,    82,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    95,     0,   113,   105,
       0,     0,     0,     0,     0,    32,    34,     0,     0,   127,
     125,     0,   140,   128,   135,     0,     0,     0,    80,   104,
     101,     0,     0,     0,     0,     0,    35,   121,     0,     0,
     131,   115,     0,   114,     0,     0,     0,   142,   144,     0,
       0,     0,     0,     0,   133,   100,     0,   141,     0,     0,
       0,     0,     0,     0,   130,   143,    36,     0,   129,   136,
       0,     0,   122,     0,     0,     0,     0,     0,     0,   131,
     134,   132
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    29,    30,    31,    32,    33,    34,    52,   185,   186,
      35,    36,    37,   101,    38,    59,    40,   175,    41,    42,
     126,   177,   230,    74,    43,    44,    81,   227,   228,    82,
      83,    45,   270,   192,   193,    46,   258,    47,   274,   284,
      48,   259,    49,    50,   129,   130
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -69
static const yytype_int16 yypact[] =
{
      21,   -21,    32,    12,   -21,    18,   -69,   132,    42,    59,
       1,     8,    54,  1280,    61,    98,   -69,   -69,   -69,   -69,
    1280,  1280,   -69,   -69,   -69,  1280,  1280,  1230,  1224,    79,
      21,   -69,   -69,   -69,   -69,   -69,    84,  1640,  1171,  1628,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
       4,   -69,    -1,   -69,   -55,     2,    71,   -69,  1183,    17,
     -69,   -69,   117,  1280,   124,  1401,   134,   135,   -69,   -69,
     -69,  1346,   -69,  1457,   -33,    77,    80,    81,   -69,    82,
     -62,   -61,   -69,    83,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   273,
    1280,  1280,   -69,  1280,  1280,  1280,  1280,  1280,  1280,  1280,
    1280,  1280,  1280,  1280,  1280,  1280,  1280,  1280,  1280,  1280,
    1280,  1280,  1280,  1280,  1280,   129,   -69,   -20,    86,   133,
     -69,   -13,   136,  1280,   153,   141,   142,   -69,   166,  1389,
     273,   159,   -69,   -69,   -69,  1280,   -69,   -69,    27,   -69,
    1280,    17,   102,   102,    58,    58,   137,   137,   137,   137,
     137,  1592,  1543,  1555,   101,   101,  1506,  1494,  1604,  1604,
    1604,  1604,  1604,  1604,  1457,    -8,  1332,   100,   -69,    -6,
     164,   165,   -69,   -69,   -69,   107,   109,   -69,    53,   176,
     -69,   108,    57,   -69,    21,   111,   -10,   177,  1457,   -69,
     -69,   -69,   -69,   -69,  1457,  1286,   -69,   -69,  1280,   157,
     174,   183,   187,   167,   169,  1280,   120,    21,  1280,   142,
     125,   332,   195,   197,    21,   140,  1457,    69,   -69,  1457,
      -5,   127,   206,    21,    21,   109,   -69,   199,   398,  1457,
     -69,   203,   -69,   -69,   -69,   464,  1280,   190,   -69,  1280,
     -69,   215,    21,   530,   596,   216,   -69,   -69,   217,   219,
     201,  1457,   140,   -69,   151,    21,   662,   -69,   -69,    21,
     223,    21,    21,  1280,   207,   -69,   728,   -69,   794,    21,
     860,   926,  1443,   158,   -69,   -69,   -69,   992,   -69,   -69,
     220,   221,   -69,   232,   233,    21,    21,  1058,  1124,   201,
     -69,   -69
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -69,   -69,   -68,    37,   -69,   -69,   -69,   235,   -69,    24,
     -69,   -69,   -69,   -69,    -3,     0,   -69,   114,   -26,   -69,
     -69,   -69,    -9,   -27,   -69,   -69,   -69,   -69,     3,    93,
     -69,   -69,   -69,   -69,    23,   -69,   -69,   -69,   -51,   -69,
     -69,   -69,   -69,   -69,   -69,   115
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -121
static const yytype_int16 yytable[] =
{
      39,    80,    79,   209,    58,    51,   179,   131,   145,   148,
      65,   135,   133,   183,   147,   149,   134,    68,    69,   184,
     127,   128,    70,    71,    73,    73,     1,     2,     3,     4,
      39,     5,     6,     7,     8,     9,    10,   145,    54,    11,
      12,   146,    13,    53,    56,    14,    15,    16,    17,    18,
      19,   180,    20,    60,   199,   200,   201,   123,   124,   125,
     139,   223,   205,    21,   206,   210,   249,    85,   132,   250,
      61,   132,    62,   152,   153,    63,    22,    23,    24,    84,
      64,    25,    22,    23,   123,   124,   125,    66,    26,    27,
     105,   106,   107,   108,   109,    86,    28,    73,    73,   151,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   176,   202,   215,    67,   216,   221,   219,   138,   220,
     174,   103,   104,   105,   106,   107,   108,   109,   136,   247,
     196,   248,   198,    57,   140,   142,   143,   204,  -117,   238,
     128,  -118,  -119,  -120,   150,   178,   245,   181,    16,    17,
      18,    19,   187,    20,   189,   253,   254,   190,   191,   194,
     197,   208,   145,   109,    21,   211,   212,   213,   214,   217,
     224,   218,   222,   231,   266,   232,   233,    22,    23,    24,
     234,   237,    25,   183,    39,   236,   241,   276,   251,    26,
      27,   278,   226,   280,   281,   229,   243,    28,   244,   252,
     255,   287,   226,   246,   257,   239,   262,    39,   265,   269,
     271,    39,   272,   273,    39,   275,   279,   297,   298,   291,
     283,   293,   294,    39,    39,   295,   296,   235,    39,    55,
     264,   203,   240,   261,   182,    39,   229,   188,   301,     0,
     263,     0,    39,    39,    39,     0,     0,     0,    85,     0,
       0,     0,     0,     0,     0,    39,    39,     0,     0,    39,
     282,    39,    39,     0,     0,    85,    39,     0,    39,    39,
      39,    39,    85,     0,     0,     0,     0,    39,     0,     0,
      85,    85,     0,     0,     0,    39,    39,    39,    39,    16,
      17,    18,    19,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,     0,    85,     0,    85,    85,     0,
       0,     0,     0,     0,    85,     0,     0,     0,    22,    23,
      24,     0,     0,     0,    85,    85,   242,     1,     2,     3,
       4,    27,     5,     6,     7,     8,     9,    10,    28,     0,
      11,    12,     0,    13,     0,     0,    14,    15,    16,    17,
      18,    19,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,    23,    24,
       0,     0,    25,     0,     0,     0,     0,     0,     0,    26,
      27,     0,   256,     1,     2,     3,     4,    28,     5,     6,
       7,     8,     9,    10,     0,     0,    11,    12,     0,    13,
       0,     0,    14,    15,    16,    17,    18,    19,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,    23,    24,     0,     0,    25,     0,
       0,     0,     0,     0,     0,    26,    27,     0,   260,     1,
       2,     3,     4,    28,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,     0,    13,     0,     0,    14,    15,
      16,    17,    18,    19,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
      23,    24,     0,     0,    25,     0,     0,     0,     0,     0,
       0,    26,    27,     0,   267,     1,     2,     3,     4,    28,
       5,     6,     7,     8,     9,    10,     0,     0,    11,    12,
       0,    13,     0,     0,    14,    15,    16,    17,    18,    19,
       0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,    24,     0,     0,
      25,     0,     0,     0,     0,     0,     0,    26,    27,     0,
     268,     1,     2,     3,     4,    28,     5,     6,     7,     8,
       9,    10,     0,     0,    11,    12,     0,    13,     0,     0,
      14,    15,    16,    17,    18,    19,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    23,    24,     0,     0,    25,     0,     0,     0,
       0,     0,     0,    26,    27,     0,   277,     1,     2,     3,
       4,    28,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,     0,    13,     0,     0,    14,    15,    16,    17,
      18,    19,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,    23,    24,
       0,     0,    25,     0,     0,     0,     0,     0,     0,    26,
      27,     0,   285,     1,     2,     3,     4,    28,     5,     6,
       7,     8,     9,    10,     0,     0,    11,    12,     0,    13,
       0,     0,    14,    15,    16,    17,    18,    19,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,    23,    24,     0,     0,    25,     0,
       0,     0,     0,     0,     0,    26,    27,     0,   286,     1,
       2,     3,     4,    28,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,     0,    13,     0,     0,    14,    15,
      16,    17,    18,    19,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
      23,    24,     0,     0,    25,     0,     0,     0,     0,     0,
       0,    26,    27,     0,   288,     1,     2,     3,     4,    28,
       5,     6,     7,     8,     9,    10,     0,     0,    11,    12,
       0,    13,     0,     0,    14,    15,    16,    17,    18,    19,
       0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,    24,     0,     0,
      25,     0,     0,     0,     0,     0,     0,    26,    27,     0,
     289,     1,     2,     3,     4,    28,     5,     6,     7,     8,
       9,    10,     0,     0,    11,    12,     0,    13,     0,     0,
      14,    15,    16,    17,    18,    19,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    23,    24,     0,     0,    25,     0,     0,     0,
       0,     0,     0,    26,    27,     0,   292,     1,     2,     3,
       4,    28,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,     0,    13,     0,     0,    14,    15,    16,    17,
      18,    19,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,    23,    24,
       0,     0,    25,     0,     0,     0,     0,     0,     0,    26,
      27,     0,   299,     1,     2,     3,     4,    28,     5,     6,
       7,     8,     9,    10,     0,     0,    11,    12,     0,    13,
       0,     0,    14,    15,    16,    17,    18,    19,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,    23,    24,     0,     0,    25,     0,
       0,     0,     0,     0,     0,    26,    27,     0,   300,     1,
       2,     3,     4,    28,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,     0,    13,     0,     0,    14,    15,
      16,    17,    18,    19,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
      23,    24,   102,     0,    25,     0,     0,     0,     0,     0,
       0,    26,    27,     0,   137,     0,     0,     0,     0,    28,
       0,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,     0,     0,     0,   115,
     116,     0,   117,   118,   119,   120,   121,   122,     0,     0,
       0,   115,   116,     0,   117,   118,   119,   120,   121,   122,
      16,    75,    76,    77,     0,    20,    16,    17,    18,    19,
       0,    20,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,     0,    22,
      23,    24,     0,     0,    25,    22,    23,    24,     0,     0,
      25,    26,    27,     0,     0,     0,     0,    26,    27,    28,
      78,     0,     0,     0,    72,    28,    16,    17,    18,    19,
       0,    20,   225,    17,    18,    19,     0,    20,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,     0,    22,    23,    24,     0,     0,
      25,    22,    23,    24,     0,     0,    25,    26,    27,     0,
       0,     0,     0,    26,    27,    28,     0,     0,     0,     0,
       0,    28,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,     0,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,     0,
     115,   116,     0,   117,   118,   119,   120,   121,   122,     0,
       0,     0,     0,     0,   115,   116,   207,   117,   118,   119,
     120,   121,   122,     0,     0,     0,     0,     0,   144,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,     0,     0,     0,     0,   115,   116,     0,
     117,   118,   119,   120,   121,   122,     0,     0,     0,   115,
     116,   195,   117,   118,   119,   120,   121,   122,     0,     0,
       0,     0,   141,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,     0,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,     0,
       0,   115,   116,     0,   117,   118,   119,   120,   121,   122,
       0,     0,     0,     0,   290,   115,   116,     0,   117,   118,
     119,   120,   121,   122,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,     0,     0,
       0,     0,   115,     0,     0,   117,   118,   119,   120,   121,
     122,     0,     0,     0,     0,     0,     0,   117,   118,   119,
     120,   121,   122,   103,   104,   105,   106,   107,   108,   109,
     110,     0,   112,   113,   114,   103,   104,   105,   106,   107,
     108,   109,   110,     0,     0,   113,   114,     0,     0,     0,
       0,     0,     0,     0,   117,   118,   119,   120,   121,   122,
       0,     0,     0,     0,     0,     0,   117,   118,   119,   120,
     121,   122,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,   113,   114,   103,   104,   105,   106,   107,   108,
     109,     0,     0,     0,   113,   114,     0,     0,     0,     0,
       0,     0,     0,   117,   118,   119,   120,   121,   122,     0,
       0,     0,     0,     0,     0,  -121,  -121,  -121,  -121,  -121,
    -121,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,
     -39,   -39,   -39,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,   123,   124,   125,   -39,     0,
       0,   -39,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100
};

static const yytype_int16 yycheck[] =
{
       0,    28,    28,     9,     7,    26,    26,     8,    70,    70,
      13,     9,    67,    26,    76,    76,    71,    20,    21,    32,
      16,    17,    25,    26,    27,    28,     5,     6,     7,     8,
      30,    10,    11,    12,    13,    14,    15,    70,    26,    18,
      19,    74,    21,    11,    26,    24,    25,    26,    27,    28,
      29,    71,    31,    11,    27,    28,    29,    67,    68,    69,
      63,    71,    70,    42,    72,    71,    71,    30,    69,    74,
      11,    69,    71,   100,   101,    67,    55,    56,    57,     0,
      26,    60,    55,    56,    67,    68,    69,    26,    67,    68,
      32,    33,    34,    35,    36,    11,    75,   100,   101,    99,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   148,    70,    26,    72,   194,    70,    11,    72,
     133,    30,    31,    32,    33,    34,    35,    36,    67,    70,
     140,    72,   145,    11,    20,    11,    11,   150,    71,   217,
      17,    71,    71,    71,    71,    26,   224,    71,    26,    27,
      28,    29,    26,    31,    11,   233,   234,    26,    26,     3,
      11,    71,    70,    36,    42,    11,    11,    70,    69,     3,
       3,    73,    71,    26,   252,    11,     3,    55,    56,    57,
       3,    71,    60,    26,   194,    26,    71,   265,    71,    67,
      68,   269,   205,   271,   272,   208,    11,    75,    11,     3,
      11,   279,   215,    73,    11,   218,    26,   217,     3,     3,
       3,   221,     3,    22,   224,    74,     3,   295,   296,    71,
      23,    11,    11,   233,   234,     3,     3,   213,   238,     4,
     249,   148,   219,   246,   129,   245,   249,   133,   299,    -1,
     247,    -1,   252,   253,   254,    -1,    -1,    -1,   221,    -1,
      -1,    -1,    -1,    -1,    -1,   265,   266,    -1,    -1,   269,
     273,   271,   272,    -1,    -1,   238,   276,    -1,   278,   279,
     280,   281,   245,    -1,    -1,    -1,    -1,   287,    -1,    -1,
     253,   254,    -1,    -1,    -1,   295,   296,   297,   298,    26,
      27,    28,    29,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   276,    -1,   278,    -1,   280,   281,    -1,
      -1,    -1,    -1,    -1,   287,    -1,    -1,    -1,    55,    56,
      57,    -1,    -1,    -1,   297,   298,     4,     5,     6,     7,
       8,    68,    10,    11,    12,    13,    14,    15,    75,    -1,
      18,    19,    -1,    21,    -1,    -1,    24,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,     4,     5,     6,     7,     8,    75,    10,    11,
      12,    13,    14,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    24,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,     4,     5,
       6,     7,     8,    75,    10,    11,    12,    13,    14,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    -1,    24,    25,
      26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,     4,     5,     6,     7,     8,    75,
      10,    11,    12,    13,    14,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    -1,    24,    25,    26,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
       4,     5,     6,     7,     8,    75,    10,    11,    12,    13,
      14,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    -1,
      24,    25,    26,    27,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,     4,     5,     6,     7,
       8,    75,    10,    11,    12,    13,    14,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    -1,    24,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,     4,     5,     6,     7,     8,    75,    10,    11,
      12,    13,    14,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    24,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,     4,     5,
       6,     7,     8,    75,    10,    11,    12,    13,    14,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    -1,    24,    25,
      26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,     4,     5,     6,     7,     8,    75,
      10,    11,    12,    13,    14,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    -1,    24,    25,    26,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
       4,     5,     6,     7,     8,    75,    10,    11,    12,    13,
      14,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    -1,
      24,    25,    26,    27,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,     4,     5,     6,     7,
       8,    75,    10,    11,    12,    13,    14,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    -1,    24,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,     4,     5,     6,     7,     8,    75,    10,    11,
      12,    13,    14,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    24,    25,    26,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,     4,     5,
       6,     7,     8,    75,    10,    11,    12,    13,    14,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    -1,    24,    25,
      26,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    11,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    11,    -1,    -1,    -1,    -1,    75,
      -1,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      26,    27,    28,    29,    -1,    31,    26,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    -1,    -1,    60,    55,    56,    57,    -1,    -1,
      60,    67,    68,    -1,    -1,    -1,    -1,    67,    68,    75,
      76,    -1,    -1,    -1,    74,    75,    26,    27,    28,    29,
      -1,    31,    26,    27,    28,    29,    -1,    31,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    -1,    -1,
      60,    55,    56,    57,    -1,    -1,    60,    67,    68,    -1,
      -1,    -1,    -1,    67,    68,    75,    -1,    -1,    -1,    -1,
      -1,    75,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    58,    59,    74,    61,    62,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    -1,    -1,    58,
      59,    72,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    71,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    71,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    61,    62,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    65,    66,    30,    31,    32,    33,    34,    35,    36,
      37,    -1,    39,    40,    41,    30,    31,    32,    33,    34,
      35,    36,    37,    -1,    -1,    40,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      65,    66,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    40,    41,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,
      66,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    67,    68,    69,    70,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    73
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,     7,     8,    10,    11,    12,    13,    14,
      15,    18,    19,    21,    24,    25,    26,    27,    28,    29,
      31,    42,    55,    56,    57,    60,    67,    68,    75,    78,
      79,    80,    81,    82,    83,    87,    88,    89,    91,    92,
      93,    95,    96,   101,   102,   108,   112,   114,   117,   119,
     120,    26,    84,    11,    26,    84,    26,    11,    91,    92,
      11,    11,    71,    67,    26,    91,    26,    26,    91,    91,
      91,    91,    74,    91,   100,    27,    28,    29,    76,    95,
     100,   103,   106,   107,     0,    80,    11,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    70,
      73,    90,    11,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    58,    59,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    97,    16,    17,   121,
     122,     8,    69,    67,    71,     9,    67,    11,    11,    91,
      20,    71,    11,    11,    72,    70,    74,    76,    70,    76,
      71,    92,   100,   100,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    94,    91,    98,    26,    26,
      71,    71,   122,    26,    32,    85,    86,    26,    94,    11,
      26,    26,   110,   111,     3,    72,    92,    11,    91,    27,
      28,    29,    95,   106,    91,    70,    72,    74,    71,     9,
      71,    11,    11,    70,    69,    70,    72,     3,    73,    70,
      72,    79,    71,    71,     3,    26,    91,   104,   105,    91,
      99,    26,    11,     3,     3,    86,    26,    71,    79,    91,
     111,    71,     4,    11,    11,    79,    73,    70,    72,    71,
      74,    71,     3,    79,    79,    11,     4,    11,   113,   118,
       4,    91,    26,   105,    99,     3,    79,     4,     4,     3,
     109,     3,     3,    22,   115,    74,    79,     4,    79,     3,
      79,    79,    91,    23,   116,     4,     4,    79,     4,     4,
      71,    71,     4,    11,    11,     3,     3,    79,    79,     4,
       4,   115
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
#line 85 "./parse/parser.y"
    { executeProgram(list_create(0)); }
    break;

  case 3:
#line 86 "./parse/parser.y"
    { executeProgram((yyvsp[(1) - (1)].generic)); }
    break;

  case 4:
#line 91 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 5:
#line 92 "./parse/parser.y"
    { list_append((yyvsp[(2) - (2)].generic), (yyvsp[(1) - (2)].generic)); (yyval.generic) = (yyvsp[(1) - (2)].generic);  }
    break;

  case 6:
#line 96 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 7:
#line 97 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 8:
#line 98 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 9:
#line 99 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 10:
#line 103 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_ASSIGNMENT, (yyvsp[(1) - (2)].generic), yylineno); }
    break;

  case 11:
#line 104 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_EXPRESSION, (yyvsp[(1) - (2)].generic), yylineno); }
    break;

  case 12:
#line 105 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_GLOBAL, (yyvsp[(2) - (3)].generic), yylineno); }
    break;

  case 13:
#line 106 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_NONLOCAL, (yyvsp[(2) - (3)].generic), yylineno); }
    break;

  case 14:
#line 107 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn((yyvsp[(2) - (3)].generic), funcDepth), yylineno); }
    break;

  case 15:
#line 108 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn(NULL, funcDepth), yylineno); }
    break;

  case 16:
#line 109 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_CONTINUE, createJump(loopDepth, 0), yylineno); }
    break;

  case 17:
#line 110 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_CONTINUE, createJump(loopDepth, 1), yylineno); }
    break;

  case 18:
#line 114 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FUNCTION, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 19:
#line 115 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_CLASS, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 20:
#line 116 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FLOW, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 21:
#line 117 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_WHILE, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 22:
#line 118 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FOR, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 23:
#line 119 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_IMPORT, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 24:
#line 120 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_EXCEPTION, (yyvsp[(1) - (1)].generic), yylineno); }
    break;

  case 25:
#line 125 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (2)].generic), NULL, NULL, 0); }
    break;

  case 26:
#line 126 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (4)].generic), NULL, (yyvsp[(4) - (4)].generic), 0); }
    break;

  case 27:
#line 127 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic), NULL, 0); }
    break;

  case 28:
#line 128 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (4)].generic), NULL, NULL, 1); ast_free((yyvsp[(4) - (4)].generic)); }
    break;

  case 29:
#line 132 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 30:
#line 133 "./parse/parser.y"
    { (yyval.generic) = str_concat(str_concat((yyvsp[(1) - (3)].generic), "."), (yyvsp[(3) - (3)].generic)); }
    break;

  case 31:
#line 137 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 32:
#line 138 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 33:
#line 142 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 34:
#line 143 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 35:
#line 147 "./parse/parser.y"
    {  (yyval.generic) = createClass((yyvsp[(2) - (7)].generic), (yyvsp[(6) - (7)].generic), NULL); }
    break;

  case 36:
#line 148 "./parse/parser.y"
    { (yyval.generic) = createClass((yyvsp[(2) - (10)].generic), (yyvsp[(9) - (10)].generic), (yyvsp[(4) - (10)].generic));}
    break;

  case 37:
#line 152 "./parse/parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), NULL); }
    break;

  case 38:
#line 153 "./parse/parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), (yyvsp[(2) - (3)].generic)); }
    break;

  case 39:
#line 157 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 40:
#line 158 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 53:
#line 168 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 54:
#line 169 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (3)].generic); }
    break;

  case 55:
#line 170 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 56:
#line 171 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 57:
#line 172 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 58:
#line 173 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 59:
#line 174 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 60:
#line 175 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 61:
#line 176 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 62:
#line 177 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 63:
#line 178 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 64:
#line 179 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 65:
#line 180 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 66:
#line 181 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 67:
#line 182 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 68:
#line 183 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 69:
#line 184 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 70:
#line 185 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 71:
#line 186 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 72:
#line 187 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 73:
#line 188 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 74:
#line 189 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 75:
#line 190 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 76:
#line 191 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 77:
#line 193 "./parse/parser.y"
    {
      ASTnode *left = createASTnode(createData(TYPE_INT, &(int){0}));
      (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), left, (yyvsp[(2) - (2)].generic));
   }
    break;

  case 78:
#line 200 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 79:
#line 201 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic), NULL))); }
    break;

  case 80:
#line 202 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (6)].generic), (yyvsp[(3) - (6)].generic), (yyvsp[(5) - (6)].generic)))); }
    break;

  case 81:
#line 203 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 82:
#line 204 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INDEX, createIndexed((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic)))); }
    break;

  case 83:
#line 205 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_SLICE, createIndexed((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic)))); }
    break;

  case 84:
#line 209 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 85:
#line 210 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 86:
#line 211 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 87:
#line 212 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 88:
#line 213 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 89:
#line 214 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LIST, (yyvsp[(1) - (1)].generic))); }
    break;

  case 90:
#line 215 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_SET, (yyvsp[(1) - (1)].generic))); }
    break;

  case 91:
#line 216 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_DICT, (yyvsp[(1) - (1)].generic))); }
    break;

  case 92:
#line 217 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 93:
#line 221 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 94:
#line 222 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 95:
#line 223 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 96:
#line 227 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 97:
#line 228 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 98:
#line 232 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_LIST ,list_create(__len__)); }
    break;

  case 99:
#line 233 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_LIST, (yyvsp[(2) - (3)].generic)); }
    break;

  case 100:
#line 237 "./parse/parser.y"
    { (yyval.generic) = createSlice((yyvsp[(2) - (7)].generic), (yyvsp[(4) - (7)].generic), (yyvsp[(6) - (7)].generic)); }
    break;

  case 101:
#line 238 "./parse/parser.y"
    { (yyval.generic) = createSlice((yyvsp[(2) - (5)].generic), (yyvsp[(4) - (5)].generic), NULL); }
    break;

  case 102:
#line 242 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INT, &(int){0})); }
    break;

  case 103:
#line 243 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 104:
#line 247 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INT, &(int){0})); }
    break;

  case 105:
#line 248 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 106:
#line 253 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 107:
#line 254 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 108:
#line 258 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_SET, (yyvsp[(2) - (3)].generic)); }
    break;

  case 109:
#line 262 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_DICT, list_create(__len__)); }
    break;

  case 110:
#line 263 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_DICT, (yyvsp[(2) - (3)].generic)); }
    break;

  case 111:
#line 267 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 112:
#line 268 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 113:
#line 272 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 114:
#line 273 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 115:
#line 277 "./parse/parser.y"
    { (yyval.generic) = createPair(createData(TYPE_STR, (yyvsp[(1) - (3)].generic)), (yyvsp[(3) - (3)].generic)); }
    break;

  case 116:
#line 281 "./parse/parser.y"
    { (yyval.generic) = createPair((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 117:
#line 285 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 118:
#line 286 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 119:
#line 287 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 120:
#line 288 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 121:
#line 294 "./parse/parser.y"
    { funcDepth++; }
    break;

  case 122:
#line 296 "./parse/parser.y"
    { 
        funcDepth = funcDepth > 0 ? funcDepth - 1 : 0;
        (yyval.generic) = createFunction((yyvsp[(2) - (11)].generic), (yyvsp[(4) - (11)].generic), (yyvsp[(10) - (11)].generic));
     }
    break;

  case 123:
#line 304 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 124:
#line 305 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 125:
#line 306 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 126:
#line 310 "./parse/parser.y"
    { (yyval.generic) = createParams((yyvsp[(1) - (1)].generic), NULL); }
    break;

  case 127:
#line 311 "./parse/parser.y"
    { (yyval.generic) = createParams((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 128:
#line 315 "./parse/parser.y"
    { loopDepth++; }
    break;

  case 129:
#line 317 "./parse/parser.y"
    {
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        (yyval.generic) = createWhile((yyvsp[(3) - (10)].generic), (yyvsp[(9) - (10)].generic));
     }
    break;

  case 130:
#line 325 "./parse/parser.y"
    { 
      (yyval.generic) = createFlow((yyvsp[(2) - (9)].generic), (yyvsp[(6) - (9)].generic), (yyvsp[(8) - (9)].generic), (yyvsp[(9) - (9)].generic)); 
    }
    break;

  case 131:
#line 331 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 132:
#line 333 "./parse/parser.y"
    { 
      (yyval.generic) = createFlow((yyvsp[(2) - (8)].generic), (yyvsp[(6) - (8)].generic), (yyvsp[(8) - (8)].generic), NULL); 
    }
    break;

  case 133:
#line 339 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 134:
#line 340 "./parse/parser.y"
    { (yyval.generic) = createFlow(NULL, (yyvsp[(5) - (6)].generic), NULL, NULL); }
    break;

  case 135:
#line 345 "./parse/parser.y"
    { 
      loopDepth++; 
     }
    break;

  case 136:
#line 349 "./parse/parser.y"
    { 
        loopDepth = loopDepth > 0 ? loopDepth - 1 : 0;
        (yyval.generic) = createFor((yyvsp[(2) - (10)].generic), (yyvsp[(4) - (10)].generic), (yyvsp[(9) - (10)].generic)); 
     }
    break;

  case 137:
#line 356 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic), NULL); }
    break;

  case 138:
#line 357 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic), NULL);  }
    break;

  case 139:
#line 358 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (3)].generic), (yyvsp[(2) - (3)].generic), (yyvsp[(3) - (3)].generic));  }
    break;

  case 140:
#line 362 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(5) - (6)].generic); }
    break;

  case 141:
#line 366 "./parse/parser.y"
    { (yyval.generic) = createCatch((yyvsp[(2) - (7)].generic), NULL, (yyvsp[(6) - (7)].generic)); }
    break;

  case 142:
#line 367 "./parse/parser.y"
    { (yyval.generic) = createCatch(NULL, NULL, (yyvsp[(5) - (6)].generic)); }
    break;

  case 143:
#line 368 "./parse/parser.y"
    { (yyval.generic) = createCatch((yyvsp[(2) - (8)].generic), (yyvsp[(4) - (8)].generic), (yyvsp[(7) - (8)].generic)); }
    break;

  case 144:
#line 372 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(5) - (6)].generic); }
    break;


/* Line 1267 of yacc.c.  */
#line 2709 "y.tab.c"
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


#line 375 "./parse/parser.y"


int main(int argc, char **argv) {
    fprintf(stderr, "=== STARTING MAIN ===\n");

    rt = createRuntime();

    initializeBuiltins(rt);

    int isInteractive = isatty(fileno(stdin));

    if (isInteractive) {
        printf("Custom Language Interpreter\n");
        printf("===========================\n");
        printf("Ready. Type code and press Enter.\n");
        printf("Press Ctrl+D to exit.\n\n");
        repl_mode = 1;
    }

    int result = yyparse();

    if (rt)
        runtime_free(rt);

    if (global_statements)
        list_free(global_statements, statement_free);

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
      data_free((Data *)returns);
   }

   printf("\n%s....Program execution completed.....\n", COLOR_GREEN);
}
