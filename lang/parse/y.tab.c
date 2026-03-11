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




/* Copy the first part of user declarations.  */
#line 1 "./parse/parser.y"

   #include <stdlib.h>

   #include "../main.h"
   
   // Include Core types
   #include "../core/lib/type.h"
   #include "../core/lib/list.h"
   #include "../core/lib/str.h"
   #include "../core/lib/maloc.h"
   #include "../core/lib/daloc.h"

   extern int  yylex(void);
   extern int  yylineno;
   extern char *yytext;
   extern int  yylineno;
   extern int  yychar;   // current lookahead token (set by yacc)
   extern void yyerror(const char *s);


   // Global declarations
   List *global_statements;

   // Function Depth
   static int f_depth = 0; 
   // Loop Depth
   static int l_depth = 0; 



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
#line 31 "./parse/parser.y"
{
   void *generic;
}
/* Line 193 of yacc.c.  */
#line 280 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 293 "y.tab.c"

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
#define YYFINAL  81
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1125

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  179
/* YYNRULES -- Number of states.  */
#define YYNSTATES  330

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   330

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      78,    83,     2,     2,    81,     2,    76,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    79,    80,
       2,    82,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    77,     2,    84,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,    10,    13,    17,    20,    23,
      25,    28,    30,    32,    34,    37,    40,    43,    46,    50,
      54,    58,    61,    64,    67,    71,    72,    75,    76,    79,
      80,    83,    84,    87,    88,    91,    92,    95,    96,    99,
     100,   103,   107,   113,   119,   125,   127,   131,   133,   137,
     139,   143,   149,   151,   155,   157,   161,   164,   168,   175,
     179,   183,   185,   189,   191,   193,   195,   197,   199,   201,
     203,   205,   207,   209,   211,   213,   215,   217,   223,   226,
     228,   233,   235,   239,   243,   247,   251,   255,   259,   263,
     267,   271,   275,   279,   283,   287,   291,   295,   299,   303,
     307,   311,   315,   319,   323,   326,   329,   332,   335,   337,
     342,   349,   353,   358,   361,   363,   365,   367,   369,   371,
     373,   375,   377,   379,   381,   382,   384,   388,   391,   393,
     395,   399,   403,   406,   410,   415,   421,   424,   428,   436,
     442,   443,   445,   446,   448,   450,   453,   457,   459,   463,
     467,   468,   476,   477,   486,   487,   489,   492,   496,   498,
     502,   505,   506,   511,   517,   518,   523,   524,   527,   528,
     535,   536,   545,   548,   551,   555,   558,   564,   567,   575
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      86,     0,    -1,    -1,    88,    -1,    24,    88,    25,    -1,
      24,    25,    -1,    24,     1,    25,    -1,     8,    87,    -1,
      79,    90,    -1,    89,    -1,    88,    89,    -1,    90,    -1,
      91,    -1,     8,    -1,     1,     8,    -1,     1,    80,    -1,
     108,    80,    -1,   111,    80,    -1,    21,    33,    80,    -1,
      22,    33,    80,    -1,     9,   111,    80,    -1,     9,    80,
      -1,    16,    80,    -1,    17,    80,    -1,    10,   111,    80,
      -1,    -1,    92,   129,    -1,    -1,    93,   107,    -1,    -1,
      94,   136,    -1,    -1,    95,   134,    -1,    -1,    96,   139,
      -1,    -1,    97,   100,    -1,    -1,    98,   142,    -1,    -1,
      99,   104,    -1,     3,   101,    80,    -1,     3,   101,     4,
      33,    80,    -1,    26,   101,     3,   102,    80,    -1,    26,
     101,     3,    41,    80,    -1,    33,    -1,   101,    76,    33,
      -1,   103,    -1,   102,    81,   103,    -1,    33,    -1,   103,
      76,    33,    -1,    23,    33,    24,   105,    25,    -1,   106,
      -1,   105,    81,   106,    -1,    33,    -1,    33,    82,    35,
      -1,     8,   106,    -1,    27,    33,    87,    -1,    27,    33,
      78,   117,    83,    87,    -1,   109,    82,   126,    -1,   109,
     110,   126,    -1,   115,    -1,   109,    81,   115,    -1,    66,
      -1,    49,    -1,    50,    -1,    52,    -1,    57,    -1,    56,
      -1,    53,    -1,    58,    -1,    75,    -1,    59,    -1,    61,
      -1,    62,    -1,   112,    -1,   113,    -1,   113,     6,   113,
      79,   112,    -1,    32,   113,    -1,   114,    -1,     5,   132,
      79,   113,    -1,   115,    -1,    78,   111,    83,    -1,   114,
      73,   114,    -1,   114,    65,   114,    -1,   114,    47,   114,
      -1,   114,    48,   114,    -1,   114,    46,   114,    -1,   114,
      68,   114,    -1,   114,    69,   114,    -1,   114,    70,   114,
      -1,   114,    71,   114,    -1,   114,    54,   114,    -1,   114,
      72,   114,    -1,   114,    37,   114,    -1,   114,    38,   114,
      -1,   114,    39,   114,    -1,   114,    40,   114,    -1,   114,
      41,   114,    -1,   114,    42,   114,    -1,   114,    43,   114,
      -1,   114,    44,   114,    -1,   114,    45,   114,    -1,   114,
      55,   114,    -1,    67,   114,    -1,    51,   114,    -1,    40,
     114,    -1,    39,   114,    -1,   116,    -1,   115,    78,   117,
      83,    -1,   115,    78,   117,    81,   127,    83,    -1,   115,
      76,    33,    -1,   115,    77,   111,    84,    -1,   115,   123,
      -1,    35,    -1,    36,    -1,    34,    -1,    74,    -1,   118,
      -1,   122,    -1,   120,    -1,   119,    -1,    33,    -1,    60,
      -1,    -1,   111,    -1,   117,    81,   111,    -1,     8,   111,
      -1,    63,    -1,    64,    -1,    24,   126,    25,    -1,    24,
     121,    25,    -1,    24,    25,    -1,   111,    79,   111,    -1,
       8,   111,    79,   111,    -1,   121,    81,   111,    79,   111,
      -1,    77,    84,    -1,    77,   126,    84,    -1,    77,   124,
      79,   125,    79,   125,    84,    -1,    77,   124,    79,   125,
      84,    -1,    -1,   111,    -1,    -1,   111,    -1,   111,    -1,
       8,   111,    -1,   126,    81,   111,    -1,   128,    -1,   127,
      81,   128,    -1,    33,    82,   111,    -1,    -1,     7,    33,
      78,   132,    83,   130,    87,    -1,    -1,    31,     7,    33,
      78,   132,    83,   131,    87,    -1,    -1,   133,    -1,     8,
     133,    -1,   132,    81,   133,    -1,    33,    -1,    33,    82,
     111,    -1,    41,    33,    -1,    -1,    14,   111,   135,    87,
      -1,    18,   111,    87,   137,   138,    -1,    -1,    19,   111,
      87,   137,    -1,    -1,    20,    87,    -1,    -1,    15,    33,
      55,   111,   140,    87,    -1,    -1,    15,    78,    33,    55,
     111,    83,   141,    87,    -1,   143,   144,    -1,   143,   145,
      -1,   143,   144,   145,    -1,    11,    87,    -1,    12,    78,
      33,    83,    87,    -1,    12,    87,    -1,    12,    78,    33,
       4,    33,    83,    87,    -1,    13,    87,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    94,    94,    95,   100,   101,   102,   103,   104,   107,
     108,   111,   112,   113,   114,   115,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   131,   131,   132,   132,   133,
     133,   134,   134,   135,   135,   136,   136,   137,   137,   138,
     138,   142,   143,   144,   145,   149,   150,   154,   155,   159,
     160,   164,   168,   169,   173,   174,   175,   194,   195,   199,
     200,   204,   205,   209,   209,   209,   209,   210,   210,   210,
     211,   211,   211,   212,   212,   217,   221,   222,   223,   227,
     228,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   259,   263,   264,
     265,   266,   267,   268,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   285,   286,   287,   288,   292,   293,
     297,   301,   302,   307,   308,   309,   314,   315,   319,   320,
     324,   325,   329,   330,   334,   335,   336,   340,   341,   345,
     349,   349,   350,   350,   354,   355,   356,   357,   361,   362,
     363,   368,   368,   372,   376,   377,   381,   382,   386,   386,
     387,   387,   392,   393,   394,   398,   402,   403,   404,   409
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IMPORT", "AS", "LAMBDA", "QMARK",
  "FUNC", "NL", "RETURN", "THROW", "TRY", "CATCH", "FINALLY", "WHILE",
  "FOR", "CONTINUE", "BREAK", "IF", "ELIF", "ELSE", "GLOBAL", "NONLOCAL",
  "ENUM", "LBRACE", "RBRACE", "FROM", "CLASS", "SWITCH", "CASE", "DEFAULT",
  "ASYNC", "AWAIT", "IDENT", "STRING", "INTEGER", "DECIMAL", "LSHIFT",
  "RSHIFT", "PLUS", "MINUS", "STAR", "DIVIDE", "MODULO", "FLOOR",
  "EXPONENT", "BITAND", "BITOR", "BITXOR", "MINUS_EQ", "DIV_EQ", "BITNOT",
  "MUL_EQ", "MOD_EQ", "LE", "IN", "EXP_EQ", "FLDIV_EQ", "R_SHIFT_EQ",
  "XOR_EQ", "NONE", "AND_EQ", "OR_EQ", "TRUE", "FALSE", "LAND", "PLUS_EQ",
  "INVERT", "EQ", "NE", "LT", "GT", "GE", "LOR", "FSTRING", "L_SHIFT_EQ",
  "'.'", "'['", "'('", "':'", "';'", "','", "'='", "')'", "']'", "$accept",
  "program", "block", "stmt_list", "statement", "simple_stmt",
  "compound_stmt", "@1", "@2", "@3", "@4", "@5", "@6", "@7", "@8",
  "import", "module", "import_items", "import_name", "enum",
  "enumerations", "enumeration", "class", "assignment", "left_side",
  "equals", "value", "ternary", "lambda", "binop", "postfix", "term",
  "args", "boolean", "set", "dict", "dict_items", "list", "sliced_items",
  "slice_start", "slice_end", "values", "kwargs", "kwarg", "func", "@9",
  "@10", "params", "param_item", "while_stmt", "@11", "if_stmt",
  "elif_chain", "else_block", "for_stmt", "@12", "@13", "exception",
  "try_block", "catch_block", "final_block", 0
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,    46,    91,    40,    58,
      59,    44,    61,    41,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    86,    87,    87,    87,    87,    87,    88,
      88,    89,    89,    89,    89,    89,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    92,    91,    93,    91,    94,
      91,    95,    91,    96,    91,    97,    91,    98,    91,    99,
      91,   100,   100,   100,   100,   101,   101,   102,   102,   103,
     103,   104,   105,   105,   106,   106,   106,   107,   107,   108,
     108,   109,   109,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   111,   112,   112,   112,   113,
     113,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   115,   115,
     115,   115,   115,   115,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   117,   117,   117,   117,   118,   118,
     119,   120,   120,   121,   121,   121,   122,   122,   123,   123,
     124,   124,   125,   125,   126,   126,   126,   127,   127,   128,
     130,   129,   131,   129,   132,   132,   132,   132,   133,   133,
     133,   135,   134,   136,   137,   137,   138,   138,   140,   139,
     141,   139,   142,   142,   142,   143,   144,   144,   144,   145
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     3,     2,     3,     2,     2,     1,
       2,     1,     1,     1,     2,     2,     2,     2,     3,     3,
       3,     2,     2,     2,     3,     0,     2,     0,     2,     0,
       2,     0,     2,     0,     2,     0,     2,     0,     2,     0,
       2,     3,     5,     5,     5,     1,     3,     1,     3,     1,
       3,     5,     1,     3,     1,     3,     2,     3,     6,     3,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     5,     2,     1,
       4,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     1,     4,
       6,     3,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     3,     2,     1,     1,
       3,     3,     2,     3,     4,     5,     2,     3,     7,     5,
       0,     1,     0,     1,     1,     2,     3,     1,     3,     3,
       0,     7,     0,     8,     0,     1,     2,     3,     1,     3,
       2,     0,     4,     5,     0,     4,     0,     2,     0,     6,
       0,     8,     2,     2,     3,     2,     5,     2,     7,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   154,    13,     0,     0,     0,     0,     0,     0,
       0,     0,   122,   116,   114,   115,     0,     0,     0,   123,
     128,   129,     0,   117,     0,     0,     0,     0,     9,    11,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    79,    81,   108,   118,   121,   120,
     119,    14,    15,     0,   158,     0,     0,   155,    21,     0,
      81,     0,    22,    23,     0,     0,     0,   132,   144,     0,
       0,    78,   107,   106,   105,   104,     0,   136,   144,     0,
       0,     1,    10,     0,     0,    26,     0,    28,     0,    30,
       0,    32,     0,    34,     0,     0,    36,     0,    38,     0,
       0,    40,    16,    64,    65,    66,    69,    68,    67,    70,
      72,    73,    74,    63,    71,     0,     0,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   140,   124,   113,   156,     0,   160,     0,     0,
      20,    24,    18,    19,   145,     0,   131,     0,   130,     0,
     145,   137,    82,     0,     0,     0,     0,   161,     0,     0,
      45,     0,     0,     0,     0,     0,   175,     0,     0,   172,
     173,     0,    62,    59,    60,     0,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    87,    85,    86,    92,   103,
      84,    88,    89,    90,    91,    93,    83,   111,   141,     0,
       0,   125,     0,   159,    80,   157,     0,   133,     0,   146,
     154,     0,   124,    57,   164,     0,     0,     0,     0,     0,
      41,     0,     7,     0,     5,     0,     8,     0,   177,   179,
     174,     0,     0,   112,   142,   127,     0,   109,   134,     0,
       0,   154,     0,     0,   166,   162,   168,     0,     0,    46,
      49,     0,     0,    47,     6,     4,     0,     0,    54,     0,
      52,    77,   143,     0,   122,   126,     0,   147,   135,   150,
       0,     0,     0,     0,     0,   163,     0,     0,    42,    44,
      43,     0,     0,     0,     0,    56,     0,    51,     0,   142,
     139,     0,     0,   110,     0,   152,    58,   164,   167,   169,
     170,    48,    50,     0,   176,    55,    53,     0,   149,     0,
     148,   151,     0,   165,     0,     0,   138,   153,   171,   178
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,   176,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    96,   171,   262,   263,   101,
     269,   270,    87,    39,    40,   117,    41,    42,    43,    44,
      60,    46,   212,    47,    48,    49,    69,    50,   144,   209,
     273,    70,   276,   277,    85,   304,   322,    56,    57,    91,
     225,    89,   254,   285,    93,   286,   324,    98,    99,   179,
     180
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -243
static const yytype_int16 yypact[] =
{
     291,    11,    26,  -243,    18,   716,   -67,   -17,    33,    35,
     597,   786,  -243,  -243,  -243,  -243,   794,   794,   794,  -243,
    -243,  -243,   794,  -243,   195,   716,    77,   370,  -243,  -243,
    -243,    42,    52,    76,    97,    93,    21,   120,   113,    60,
    1043,    61,  -243,   138,   836,  1009,  -243,  -243,  -243,  -243,
    -243,  -243,  -243,    -5,    70,   123,   -48,  -243,  -243,    86,
      29,    89,  -243,  -243,    90,    92,   716,  -243,    80,   -11,
      16,  -243,  -243,  -243,  -243,  -243,   716,  -243,  -243,   -38,
      91,  -243,  -243,   140,   168,  -243,   166,  -243,   716,  -243,
     716,  -243,    -7,  -243,   169,   169,  -243,    14,  -243,    87,
     171,  -243,  -243,  -243,  -243,  -243,  -243,  -243,  -243,  -243,
    -243,  -243,  -243,  -243,  -243,    94,   646,   646,  -243,   786,
     794,   794,   794,   794,   794,   794,   794,   794,   794,   794,
     794,   794,   794,   794,   794,   794,   794,   794,   794,   794,
     794,   172,   716,   668,  -243,  -243,   716,  -243,   786,    -5,
    -243,  -243,  -243,  -243,   122,   716,  -243,   716,  -243,   716,
    -243,  -243,  -243,   129,   175,    31,    14,  -243,   154,   177,
    -243,     7,    12,    14,   448,   575,  -243,    36,    14,   200,
    -243,   190,    29,   134,   134,   137,   241,   241,   105,   105,
     176,   176,   176,   176,   176,   966,   928,   947,   985,   985,
     909,   985,   985,   985,   985,   985,   873,  -243,   133,   141,
     716,  -243,    43,  -243,  -243,  -243,   716,  -243,   144,  -243,
      26,   146,   668,  -243,   206,    14,   716,   178,   193,   199,
    -243,     4,  -243,    10,  -243,   526,  -243,   203,  -243,  -243,
    -243,    32,   716,  -243,   716,  -243,   738,  -243,  -243,   716,
      51,    26,    54,   716,   218,  -243,  -243,   716,   159,  -243,
    -243,   161,    39,   167,  -243,  -243,     6,    32,   162,    23,
    -243,  -243,  -243,   -52,   165,  -243,    79,  -243,  -243,  -243,
      84,   716,    14,    14,    14,  -243,    14,   173,  -243,  -243,
    -243,   215,   217,   219,    14,  -243,   216,  -243,    32,   716,
    -243,   716,   221,  -243,    14,  -243,  -243,   206,  -243,  -243,
    -243,   167,  -243,   174,  -243,  -243,  -243,   179,  -243,   165,
    -243,  -243,    14,  -243,    14,    14,  -243,  -243,  -243,  -243
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -243,  -243,  -161,    96,   -24,    85,  -243,  -243,  -243,  -243,
    -243,  -243,  -243,  -243,  -243,  -243,   170,  -243,   -30,  -243,
    -243,  -242,  -243,  -243,  -243,  -243,    -4,    22,    -3,    58,
       2,  -243,    44,  -243,  -243,  -243,  -243,  -243,  -243,  -243,
     -32,   -15,  -243,   -34,  -243,  -243,  -243,  -190,   -46,  -243,
    -243,  -243,   -36,  -243,  -243,  -243,  -243,  -243,  -243,  -243,
      95
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -62
static const yytype_int16 yytable[] =
{
      59,    61,    45,    82,   223,   224,    68,   145,    71,    79,
     293,   228,   232,    62,   156,   231,   238,   239,    51,    51,
      78,    80,   173,     2,    94,   295,   168,   299,    54,    45,
     250,   148,   300,   149,    53,   264,    55,   260,   174,   173,
     267,   158,    10,   159,   173,   261,   161,    95,   297,    83,
      11,    12,    13,    14,    15,   174,   316,    16,    17,    54,
     174,   280,   154,    63,   255,   268,    64,    55,    65,    18,
     157,   169,   160,    84,    72,    73,    74,    81,    19,    86,
      75,    20,    21,   229,   166,    22,   167,   230,   229,   294,
      52,    52,    23,   175,    88,    24,    25,   159,    58,   177,
     178,   183,   184,   215,   298,   141,   142,   143,    92,   222,
     175,    90,    78,    78,   237,   175,   185,   182,    10,   290,
     291,   306,   307,   308,   246,   309,   247,    12,    13,    14,
      15,    97,   149,   314,   279,   281,   100,   282,   208,   211,
     102,   118,   213,   321,   119,   214,   124,   125,   126,   127,
     128,   217,   146,   218,    19,   219,   147,    20,    21,   155,
     302,   327,   303,   328,   329,   149,   150,   305,    23,   151,
     152,    24,   153,   163,   162,   164,    45,    45,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   165,
       2,   216,   170,    76,   181,   207,   245,   220,   221,   226,
     227,    82,   248,   178,   241,   159,   242,   243,   211,    10,
     244,   128,   256,   249,   251,   253,   258,    11,    12,    13,
      14,    15,   259,   257,    16,    17,   266,    45,   284,   288,
     272,   289,   275,   292,   296,   278,    18,   301,   260,   283,
     312,   315,   313,   287,   319,    19,   310,   325,    20,    21,
     236,   311,    22,   326,   271,   172,   252,   317,   320,    23,
     235,   323,    24,    25,   240,     0,     0,   275,     0,    77,
     122,   123,   124,   125,   126,   127,   128,     0,     0,     0,
       0,    -2,     1,     0,   -35,   272,     2,   318,   -25,     3,
       4,     5,   -37,     0,     0,   -31,   -33,     6,     7,   -29,
       0,     0,     8,     9,   -39,    10,     0,   -35,   -27,     0,
       0,     0,   -25,    11,    12,    13,    14,    15,     0,     0,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,    19,     0,     0,    20,    21,     0,     0,    22,     0,
       0,     0,     0,     0,     0,    23,     0,     0,    24,    25,
      -3,     1,     0,   -35,     0,     2,     0,   -25,     3,     4,
       5,   -37,     0,     0,   -31,   -33,     6,     7,   -29,     0,
       0,     8,     9,   -39,    10,     0,   -35,   -27,     0,     0,
       0,   -25,    11,    12,    13,    14,    15,     0,     0,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     0,     0,    20,    21,     0,     0,    22,     0,     0,
       0,     0,     0,     0,    23,     0,     0,    24,    25,   233,
       0,   -35,     0,     2,     0,   -25,     3,     4,     5,   -37,
       0,     0,   -31,   -33,     6,     7,   -29,     0,     0,     8,
       9,   -39,    10,   234,   -35,   -27,     0,     0,     0,   -25,
      11,    12,    13,    14,    15,     0,     0,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,    19,     0,
       0,    20,    21,     0,     0,    22,     0,     0,     0,     0,
       0,     0,    23,     0,     0,    24,    25,     1,     0,   -35,
       0,     2,     0,   -25,     3,     4,     5,   -37,     0,     0,
     -31,   -33,     6,     7,   -29,     0,     0,     8,     9,   -39,
      10,   265,   -35,   -27,     0,     0,     0,   -25,    11,    12,
      13,    14,    15,     0,     0,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       2,     0,     0,     0,     4,     5,    19,     0,     0,    20,
      21,     6,     7,    22,     0,     0,     8,     9,     0,    10,
      23,     0,     2,    24,    25,    66,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,     0,     0,     0,     0,
       0,    10,    67,     0,     0,     0,    18,     0,     0,    11,
      12,    13,    14,    15,     0,    19,    16,    17,    20,    21,
       0,     0,    22,     0,     0,     0,     0,     0,    18,    23,
       0,     2,    24,    25,    76,     0,     0,    19,     0,     0,
      20,    21,     0,     0,    22,     0,     0,     0,     0,     0,
      10,    23,     0,     2,    24,    25,   210,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,     0,     0,     0,
       0,     0,    10,     0,     0,     0,     0,    18,     0,     0,
      11,    12,    13,    14,    15,     0,    19,    16,    17,    20,
      21,     0,     0,    22,     0,     0,     0,     0,     0,    18,
      23,     2,     0,    24,    25,     0,     0,     0,    19,     0,
       0,    20,    21,     0,     0,    22,     0,     0,     0,     0,
      10,     0,    23,     2,     0,    24,    25,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,     0,     0,     0,
       0,     0,    10,     0,     0,     0,     0,    18,     0,     0,
      11,   274,    13,    14,    15,     0,    19,    16,    17,    20,
      21,     0,     0,    22,     0,     0,     0,     0,     0,    18,
      23,     2,     0,    24,    25,     0,     0,     0,    19,     0,
       0,    20,    21,     0,     0,    22,     0,     0,     0,     0,
      10,     0,    23,     0,     0,    24,    25,     0,    10,    12,
      13,    14,    15,     0,     0,    16,    17,    12,    13,    14,
      15,     0,     0,    16,    17,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,    18,    19,     0,     0,    20,
      21,     0,     0,    22,    19,     0,     0,    20,    21,     0,
      23,    22,     0,    24,    25,     0,     0,     0,    23,     0,
       0,    24,    25,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,     0,     0,     0,     0,
     132,   133,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   134,     0,     0,   135,   136,   137,   138,   139,   140,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     0,     0,     0,     0,     0,   132,   133,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,     0,
       0,   135,   136,   137,   138,   139,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,     0,
       0,     0,     0,   132,   133,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,     0,   131,   135,   136,   137,
     138,   139,   132,   133,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     0,     0,   135,   136,   137,   138,
     139,   132,   133,   120,   121,   122,   123,   124,   125,   126,
     127,   128,     0,     0,     0,   135,   136,   137,   138,   139,
     132,   133,   120,   121,   122,   123,   124,   125,   126,   127,
     128,     0,     0,     0,   135,   136,   137,   138,   139,   -62,
     -62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -62,   -62,   -62,   -62,   -62,   -61,   -61,
       0,   -61,   -61,     0,     0,   -61,   -61,   -61,   -61,     0,
     -61,   -61,     0,     0,     0,   -61,     0,     0,     0,     0,
       0,     0,     0,     0,   -61,   141,   142,   143,     0,     0,
     -61,   -61,   103,   104,     0,   105,   106,     0,     0,   107,
     108,   109,   110,     0,   111,   112,     0,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
       0,     0,     0,     0,   115,   116
};

static const yytype_int16 yycheck[] =
{
       4,     5,     0,    27,   165,   166,    10,    53,    11,    24,
       4,     4,   173,    80,    25,     3,   177,   178,     8,     8,
      24,    25,     8,     5,     3,   267,    33,    79,    33,    27,
     220,    79,    84,    81,     8,    25,    41,    33,    24,     8,
       8,    25,    24,    81,     8,    41,    84,    26,    25,     7,
      32,    33,    34,    35,    36,    24,   298,    39,    40,    33,
      24,   251,    66,    80,   225,    33,    33,    41,    33,    51,
      81,    78,    76,    31,    16,    17,    18,     0,    60,    27,
      22,    63,    64,    76,    88,    67,    90,    80,    76,    83,
      80,    80,    74,    79,    18,    77,    78,    81,    80,    12,
      13,   116,   117,   149,    81,    76,    77,    78,    15,    78,
      79,    14,   116,   117,    78,    79,   119,   115,    24,    80,
      81,   282,   283,   284,    81,   286,    83,    33,    34,    35,
      36,    11,    81,   294,    83,    81,    23,    83,   142,   143,
      80,    80,   146,   304,     6,   148,    41,    42,    43,    44,
      45,   155,    82,   157,    60,   159,    33,    63,    64,    79,
      81,   322,    83,   324,   325,    81,    80,    83,    74,    80,
      80,    77,    80,    33,    83,     7,   174,   175,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    33,
       5,    79,    33,     8,    33,    33,   210,    78,    33,    55,
      33,   235,   216,    13,    24,    81,    79,    84,   222,    24,
      79,    45,   226,    79,    78,    19,    33,    32,    33,    34,
      35,    36,    33,    55,    39,    40,    33,   235,    20,    80,
     244,    80,   246,    76,    82,   249,    51,    82,    33,   253,
      33,    35,    33,   257,    33,    60,    83,    83,    63,    64,
     175,   291,    67,    84,   242,    95,   222,   299,   302,    74,
     174,   307,    77,    78,   179,    -1,    -1,   281,    -1,    84,
      39,    40,    41,    42,    43,    44,    45,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,   299,     5,   301,     7,     8,
       9,    10,    11,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    21,    22,    23,    24,    -1,    26,    27,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,    78,
       0,     1,    -1,     3,    -1,     5,    -1,     7,     8,     9,
      10,    11,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    21,    22,    23,    24,    -1,    26,    27,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    77,    78,     1,
      -1,     3,    -1,     5,    -1,     7,     8,     9,    10,    11,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    -1,    -1,    -1,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    77,    78,     1,    -1,     3,
      -1,     5,    -1,     7,     8,     9,    10,    11,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    21,    22,    23,
      24,    25,    26,    27,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
       5,    -1,    -1,    -1,     9,    10,    60,    -1,    -1,    63,
      64,    16,    17,    67,    -1,    -1,    21,    22,    -1,    24,
      74,    -1,     5,    77,    78,     8,    -1,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    24,    25,    -1,    -1,    -1,    51,    -1,    -1,    32,
      33,    34,    35,    36,    -1,    60,    39,    40,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    51,    74,
      -1,     5,    77,    78,     8,    -1,    -1,    60,    -1,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      24,    74,    -1,     5,    77,    78,     8,    -1,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      32,    33,    34,    35,    36,    -1,    60,    39,    40,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    51,
      74,     5,    -1,    77,    78,    -1,    -1,    -1,    60,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      24,    -1,    74,     5,    -1,    77,    78,    -1,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      32,    33,    34,    35,    36,    -1,    60,    39,    40,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    51,
      74,     5,    -1,    77,    78,    -1,    -1,    -1,    60,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      24,    -1,    74,    -1,    -1,    77,    78,    -1,    24,    33,
      34,    35,    36,    -1,    -1,    39,    40,    33,    34,    35,
      36,    -1,    -1,    39,    40,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    60,    -1,    -1,    63,
      64,    -1,    -1,    67,    60,    -1,    -1,    63,    64,    -1,
      74,    67,    -1,    77,    78,    -1,    -1,    -1,    74,    -1,
      -1,    77,    78,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    -1,    -1,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    71,    72,    73,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    68,    69,    70,    71,    72,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,    54,    55,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    68,    69,    70,
      71,    72,    54,    55,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    68,    69,    70,    71,
      72,    54,    55,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    68,    69,    70,    71,    72,
      54,    55,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    -1,    -1,    68,    69,    70,    71,    72,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    70,    71,    72,    49,    50,
      -1,    52,    53,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    78,    -1,    -1,
      81,    82,    49,    50,    -1,    52,    53,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    62,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    81,    82
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     5,     8,     9,    10,    16,    17,    21,    22,
      24,    32,    33,    34,    35,    36,    39,    40,    51,    60,
      63,    64,    67,    74,    77,    78,    86,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   108,
     109,   111,   112,   113,   114,   115,   116,   118,   119,   120,
     122,     8,    80,     8,    33,    41,   132,   133,    80,   111,
     115,   111,    80,    80,    33,    33,     8,    25,   111,   121,
     126,   113,   114,   114,   114,   114,     8,    84,   111,   126,
     111,     0,    89,     7,    31,   129,    27,   107,    18,   136,
      14,   134,    15,   139,     3,    26,   100,    11,   142,   143,
      23,   104,    80,    49,    50,    52,    53,    56,    57,    58,
      59,    61,    62,    66,    75,    81,    82,   110,    80,     6,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    54,    55,    65,    68,    69,    70,    71,    72,
      73,    76,    77,    78,   123,   133,    82,    33,    79,    81,
      80,    80,    80,    80,   111,    79,    25,    81,    25,    81,
     111,    84,    83,    33,     7,    33,   111,   111,    33,    78,
      33,   101,   101,     8,    24,    79,    87,    12,    13,   144,
     145,    33,   115,   126,   126,   113,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,    33,   111,   124,
       8,   111,   117,   111,   113,   133,    79,   111,   111,   111,
      78,    33,    78,    87,    87,   135,    55,    33,     4,    76,
      80,     3,    87,     1,    25,    88,    90,    78,    87,    87,
     145,    24,    79,    84,    79,   111,    81,    83,   111,    79,
     132,    78,   117,    19,   137,    87,   111,    55,    33,    33,
      33,    41,   102,   103,    25,    25,    33,     8,    33,   105,
     106,   112,   111,   125,    33,   111,   127,   128,   111,    83,
     132,    81,    83,   111,    20,   138,   140,   111,    80,    80,
      80,    81,    76,     4,    83,   106,    82,    25,    81,    79,
      84,    82,    81,    83,   130,    83,    87,    87,    87,    87,
      83,   103,    33,    33,    87,    35,   106,   125,   111,    33,
     128,    87,   131,   137,   141,    83,    84,    87,    87,    87
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
#line 94 "./parse/parser.y"
    { global_statements = list_create(0); }
    break;

  case 3:
#line 95 "./parse/parser.y"
    { global_statements = (yyvsp[(1) - (1)].generic); }
    break;

  case 4:
#line 100 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (3)].generic); }
    break;

  case 5:
#line 101 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 6:
#line 102 "./parse/parser.y"
    { (yyval.generic) = list_create(0); yyerrok; }
    break;

  case 7:
#line 103 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;

  case 8:
#line 104 "./parse/parser.y"
    { (yyval.generic) = list_create(1); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 9:
#line 107 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 10:
#line 108 "./parse/parser.y"
    { list_append((yyvsp[(2) - (2)].generic), (yyvsp[(1) - (2)].generic)); (yyval.generic) = (yyvsp[(1) - (2)].generic); }
    break;

  case 11:
#line 111 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 12:
#line 112 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 13:
#line 113 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 14:
#line 114 "./parse/parser.y"
    { (yyval.generic) = NULL; yyerrok; }
    break;

  case 15:
#line 115 "./parse/parser.y"
    { (yyval.generic) = NULL; yyerrok; }
    break;

  case 16:
#line 119 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_ASMT, (yyvsp[(1) - (2)].generic), yylineno-1); }
    break;

  case 17:
#line 120 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_EXPR, (yyvsp[(1) - (2)].generic), yylineno-1); }
    break;

  case 18:
#line 121 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_GLOBAL, (yyvsp[(2) - (3)].generic), yylineno-1); }
    break;

  case 19:
#line 122 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_NONLOCAL, (yyvsp[(2) - (3)].generic), yylineno-1); }
    break;

  case 20:
#line 123 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn((yyvsp[(2) - (3)].generic), f_depth), yylineno-1); }
    break;

  case 21:
#line 124 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn(NULL, f_depth), yylineno-1); }
    break;

  case 22:
#line 125 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_CONTINUE, createJump(l_depth, 0), yylineno-1); }
    break;

  case 23:
#line 126 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_BREAK, createJump(l_depth, 1), yylineno-1); }
    break;

  case 24:
#line 127 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_THROW, (yyvsp[(2) - (3)].generic), yylineno-1); }
    break;

  case 25:
#line 131 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 26:
#line 131 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FUNC, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 27:
#line 132 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 28:
#line 132 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_CLASS, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 29:
#line 133 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 30:
#line 133 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FLOW, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 31:
#line 134 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 32:
#line 134 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_WHILE, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 33:
#line 135 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 34:
#line 135 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FOR, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 35:
#line 136 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 36:
#line 136 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_IMPORT, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 37:
#line 137 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 38:
#line 137 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_EXCEPTION, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 39:
#line 138 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 40:
#line 138 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_ENUM, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 41:
#line 142 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (3)].generic), NULL, NULL, 0); }
    break;

  case 42:
#line 143 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (5)].generic), NULL, (yyvsp[(4) - (5)].generic), 0); }
    break;

  case 43:
#line 144 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (5)].generic), (yyvsp[(4) - (5)].generic), NULL, 0); }
    break;

  case 44:
#line 145 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (5)].generic), NULL, NULL, 1); ast_free((yyvsp[(4) - (5)].generic)); }
    break;

  case 45:
#line 149 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 46:
#line 150 "./parse/parser.y"
    { (yyval.generic) = str_concat(str_concat((yyvsp[(1) - (3)].generic), "."), (yyvsp[(3) - (3)].generic)); }
    break;

  case 47:
#line 154 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 48:
#line 155 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 49:
#line 159 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 50:
#line 160 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 51:
#line 164 "./parse/parser.y"
    { (yyval.generic) = createEnum((yyvsp[(2) - (5)].generic), (yyvsp[(4) - (5)].generic)); }
    break;

  case 52:
#line 168 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__);  list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 53:
#line 169 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 54:
#line 173 "./parse/parser.y"
    { (yyval.generic) = createEnumItem((yyvsp[(1) - (1)].generic), NULL); }
    break;

  case 55:
#line 174 "./parse/parser.y"
    { (yyval.generic) = createEnumItem((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 56:
#line 175 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;

  case 57:
#line 194 "./parse/parser.y"
    { (yyval.generic) = createClass((yyvsp[(2) - (3)].generic), (yyvsp[(3) - (3)].generic), NULL); }
    break;

  case 58:
#line 195 "./parse/parser.y"
    { (yyval.generic) = createClass((yyvsp[(2) - (6)].generic), (yyvsp[(6) - (6)].generic), (yyvsp[(4) - (6)].generic)); }
    break;

  case 59:
#line 199 "./parse/parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), NULL); }
    break;

  case 60:
#line 200 "./parse/parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), (yyvsp[(2) - (3)].generic)); }
    break;

  case 61:
#line 204 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 62:
#line 205 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 75:
#line 217 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 76:
#line 221 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 77:
#line 222 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_TERNARY, createTernary((yyvsp[(1) - (5)].generic), (yyvsp[(3) - (5)].generic), (yyvsp[(5) - (5)].generic)))); }
    break;

  case 78:
#line 223 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_AWAIT, createAwait((yyvsp[(2) - (2)].generic)))); }
    break;

  case 79:
#line 227 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 80:
#line 228 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createLambda((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic), yylineno)); }
    break;

  case 82:
#line 233 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (3)].generic); }
    break;

  case 83:
#line 234 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 84:
#line 235 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 85:
#line 236 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 86:
#line 237 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 87:
#line 238 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 88:
#line 239 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 89:
#line 240 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 90:
#line 241 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 91:
#line 242 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 92:
#line 243 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 93:
#line 244 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 94:
#line 245 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 95:
#line 246 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 96:
#line 247 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 97:
#line 248 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 98:
#line 249 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 99:
#line 250 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 100:
#line 251 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 101:
#line 252 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 102:
#line 253 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 103:
#line 254 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 104:
#line 255 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 105:
#line 256 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 106:
#line 258 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), createASTnode(createData(TYPE_INT, &(int){0})), (yyvsp[(2) - (2)].generic)); }
    break;

  case 107:
#line 260 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), createASTnode(createData(TYPE_INT, &(int){0})), (yyvsp[(2) - (2)].generic)); }
    break;

  case 108:
#line 263 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 109:
#line 264 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic), NULL))); }
    break;

  case 110:
#line 265 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (6)].generic), (yyvsp[(3) - (6)].generic), (yyvsp[(5) - (6)].generic)))); }
    break;

  case 111:
#line 266 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 112:
#line 267 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INDEX, createIndexed((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic)))); }
    break;

  case 113:
#line 268 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_SLICE, createIndexed((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic)))); }
    break;

  case 114:
#line 272 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 115:
#line 273 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 116:
#line 274 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 117:
#line 275 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 118:
#line 276 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 119:
#line 277 "./parse/parser.y"
    { (yyval.generic) = createASTnode((yyvsp[(1) - (1)].generic)); }
    break;

  case 120:
#line 278 "./parse/parser.y"
    { (yyval.generic) = createASTnode((yyvsp[(1) - (1)].generic)); }
    break;

  case 121:
#line 279 "./parse/parser.y"
    { (yyval.generic) = createASTnode((yyvsp[(1) - (1)].generic)); }
    break;

  case 122:
#line 280 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 123:
#line 281 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_NONE, NULL)); }
    break;

  case 124:
#line 285 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 125:
#line 286 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 126:
#line 287 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 127:
#line 288 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 128:
#line 292 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 129:
#line 293 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 130:
#line 297 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_SET_EXPR, (yyvsp[(2) - (3)].generic)); }
    break;

  case 131:
#line 301 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_DICT_EXPR, (yyvsp[(2) - (3)].generic)); }
    break;

  case 132:
#line 302 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_DICT_EXPR, list_create(0)); }
    break;

  case 133:
#line 307 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append(createPair((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)), (yyval.generic)); }
    break;

  case 134:
#line 308 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append(createPair((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic)), (yyval.generic)); }
    break;

  case 135:
#line 309 "./parse/parser.y"
    { list_append(createPair((yyvsp[(3) - (5)].generic), (yyvsp[(5) - (5)].generic)), (yyvsp[(1) - (5)].generic)); (yyval.generic) = (yyvsp[(1) - (5)].generic); }
    break;

  case 136:
#line 314 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_LIST_EXPR, list_create(0)); }
    break;

  case 137:
#line 315 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_LIST_EXPR, (yyvsp[(2) - (3)].generic)); }
    break;

  case 138:
#line 319 "./parse/parser.y"
    { (yyval.generic) = createSlice((yyvsp[(2) - (7)].generic), (yyvsp[(4) - (7)].generic), (yyvsp[(6) - (7)].generic)); }
    break;

  case 139:
#line 320 "./parse/parser.y"
    { (yyval.generic) = createSlice((yyvsp[(2) - (5)].generic), (yyvsp[(4) - (5)].generic), NULL); }
    break;

  case 140:
#line 324 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 141:
#line 325 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 142:
#line 329 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 143:
#line 330 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 144:
#line 334 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 145:
#line 335 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 146:
#line 336 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 147:
#line 340 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 148:
#line 341 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 149:
#line 345 "./parse/parser.y"
    { (yyval.generic) = createPair(createData(TYPE_STR, (yyvsp[(1) - (3)].generic)), (yyvsp[(3) - (3)].generic)); }
    break;

  case 150:
#line 349 "./parse/parser.y"
    { f_depth++; }
    break;

  case 151:
#line 349 "./parse/parser.y"
    { (yyval.generic) = createFunction((yyvsp[(2) - (7)].generic), (yyvsp[(4) - (7)].generic), (yyvsp[(7) - (7)].generic), 0, &f_depth); }
    break;

  case 152:
#line 350 "./parse/parser.y"
    { f_depth++; }
    break;

  case 153:
#line 350 "./parse/parser.y"
    { (yyval.generic) = createFunction((yyvsp[(3) - (8)].generic), (yyvsp[(5) - (8)].generic), (yyvsp[(8) - (8)].generic), 1, &f_depth); }
    break;

  case 154:
#line 354 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 155:
#line 355 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 156:
#line 356 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 157:
#line 357 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 158:
#line 361 "./parse/parser.y"
    { (yyval.generic) = createParam((yyvsp[(1) - (1)].generic), NULL, 0); }
    break;

  case 159:
#line 362 "./parse/parser.y"
    { (yyval.generic) = createParam((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), 0); }
    break;

  case 160:
#line 363 "./parse/parser.y"
    { ast_free((yyvsp[(1) - (2)].generic)); (yyval.generic) = createParam((yyvsp[(2) - (2)].generic), NULL, 1); }
    break;

  case 161:
#line 368 "./parse/parser.y"
    { l_depth++; }
    break;

  case 162:
#line 368 "./parse/parser.y"
    { (yyval.generic) = createWhile((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic), &l_depth); }
    break;

  case 163:
#line 372 "./parse/parser.y"
    { (yyval.generic) = createFlow((yyvsp[(2) - (5)].generic), (yyvsp[(3) - (5)].generic), (yyvsp[(4) - (5)].generic), (yyvsp[(5) - (5)].generic)); }
    break;

  case 164:
#line 376 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 165:
#line 377 "./parse/parser.y"
    { (yyval.generic) = createFlow((yyvsp[(2) - (4)].generic), (yyvsp[(3) - (4)].generic), (yyvsp[(4) - (4)].generic), NULL); }
    break;

  case 166:
#line 381 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 167:
#line 382 "./parse/parser.y"
    { (yyval.generic) = createFlow(NULL, (yyvsp[(2) - (2)].generic), NULL, NULL); }
    break;

  case 168:
#line 386 "./parse/parser.y"
    { l_depth++; }
    break;

  case 169:
#line 386 "./parse/parser.y"
    { ast_free((yyvsp[(3) - (6)].generic)); (yyval.generic) = createFor((yyvsp[(2) - (6)].generic), (yyvsp[(4) - (6)].generic), (yyvsp[(6) - (6)].generic), &l_depth); }
    break;

  case 170:
#line 387 "./parse/parser.y"
    { l_depth++; }
    break;

  case 171:
#line 387 "./parse/parser.y"
    { ast_free((yyvsp[(4) - (8)].generic)); (yyval.generic) = createFor((yyvsp[(3) - (8)].generic), (yyvsp[(5) - (8)].generic), (yyvsp[(8) - (8)].generic), &l_depth);}
    break;

  case 172:
#line 392 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic), NULL); }
    break;

  case 173:
#line 393 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic), NULL); }
    break;

  case 174:
#line 394 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (3)].generic), (yyvsp[(2) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 175:
#line 398 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;

  case 176:
#line 402 "./parse/parser.y"
    { (yyval.generic) = createCatch((yyvsp[(3) - (5)].generic), NULL, (yyvsp[(5) - (5)].generic)); }
    break;

  case 177:
#line 403 "./parse/parser.y"
    { (yyval.generic) = createCatch(NULL, NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 178:
#line 404 "./parse/parser.y"
    { (yyval.generic) = createCatch((yyvsp[(3) - (7)].generic), (yyvsp[(5) - (7)].generic), (yyvsp[(7) - (7)].generic)); }
    break;

  case 179:
#line 409 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;


/* Line 1267 of yacc.c.  */
#line 2789 "y.tab.c"
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


#line 412 "./parse/parser.y"


int main(int argc, char **argv)                        { return run(argc, argv); }
