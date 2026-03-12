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
#line 274 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 287 "y.tab.c"

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
#define YYFINAL  82
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1270

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  181
/* YYNRULES -- Number of states.  */
#define YYNSTATES  335

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   327

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      75,    80,     2,     2,    78,     2,    73,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    76,    77,
       2,    79,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    74,     2,    81,     2,     2,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72
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
     395,   399,   403,   406,   410,   415,   421,   424,   428,   434,
     439,   447,   453,   454,   456,   457,   459,   461,   464,   468,
     470,   474,   478,   479,   487,   488,   497,   498,   500,   503,
     507,   509,   513,   516,   517,   522,   528,   529,   534,   535,
     538,   539,   546,   547,   556,   559,   562,   566,   569,   575,
     578,   586
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      83,     0,    -1,    -1,    85,    -1,    24,    85,    25,    -1,
      24,    25,    -1,    24,     1,    25,    -1,     8,    84,    -1,
      76,    87,    -1,    86,    -1,    85,    86,    -1,    87,    -1,
      88,    -1,     8,    -1,     1,     8,    -1,     1,    77,    -1,
     105,    77,    -1,   108,    77,    -1,    21,    30,    77,    -1,
      22,    30,    77,    -1,     9,   108,    77,    -1,     9,    77,
      -1,    16,    77,    -1,    17,    77,    -1,    10,   108,    77,
      -1,    -1,    89,   126,    -1,    -1,    90,   104,    -1,    -1,
      91,   133,    -1,    -1,    92,   131,    -1,    -1,    93,   136,
      -1,    -1,    94,    97,    -1,    -1,    95,   139,    -1,    -1,
      96,   101,    -1,     3,    98,    77,    -1,     3,    98,     4,
      30,    77,    -1,    26,    98,     3,    99,    77,    -1,    26,
      98,     3,    38,    77,    -1,    30,    -1,    98,    73,    30,
      -1,   100,    -1,    99,    78,   100,    -1,    30,    -1,   100,
      73,    30,    -1,    23,    30,    24,   102,    25,    -1,   103,
      -1,   102,    78,   103,    -1,    30,    -1,    30,    79,    32,
      -1,     8,   103,    -1,    27,    30,    84,    -1,    27,    30,
      75,   114,    80,    84,    -1,   106,    79,   123,    -1,   106,
     107,   123,    -1,   112,    -1,   106,    78,   112,    -1,    63,
      -1,    46,    -1,    47,    -1,    49,    -1,    54,    -1,    53,
      -1,    50,    -1,    55,    -1,    72,    -1,    56,    -1,    58,
      -1,    59,    -1,   109,    -1,   110,    -1,   110,     6,   110,
      76,   109,    -1,    29,   110,    -1,   111,    -1,     5,   129,
      76,   110,    -1,   112,    -1,    75,   108,    80,    -1,   111,
      70,   111,    -1,   111,    62,   111,    -1,   111,    44,   111,
      -1,   111,    45,   111,    -1,   111,    43,   111,    -1,   111,
      65,   111,    -1,   111,    66,   111,    -1,   111,    67,   111,
      -1,   111,    68,   111,    -1,   111,    51,   111,    -1,   111,
      69,   111,    -1,   111,    34,   111,    -1,   111,    35,   111,
      -1,   111,    36,   111,    -1,   111,    37,   111,    -1,   111,
      38,   111,    -1,   111,    39,   111,    -1,   111,    40,   111,
      -1,   111,    41,   111,    -1,   111,    42,   111,    -1,   111,
      52,   111,    -1,    64,   111,    -1,    48,   111,    -1,    37,
     111,    -1,    36,   111,    -1,   113,    -1,   112,    75,   114,
      80,    -1,   112,    75,   114,    78,   124,    80,    -1,   112,
      73,    30,    -1,   112,    74,   108,    81,    -1,   112,   120,
      -1,    32,    -1,    33,    -1,    31,    -1,    71,    -1,   115,
      -1,   119,    -1,   117,    -1,   116,    -1,    30,    -1,    57,
      -1,    -1,   108,    -1,   114,    78,   108,    -1,     8,   108,
      -1,    60,    -1,    61,    -1,    24,   123,    25,    -1,    24,
     118,    25,    -1,    24,    25,    -1,   108,    76,   108,    -1,
       8,   108,    76,   108,    -1,   118,    78,   108,    76,   108,
      -1,    74,    81,    -1,    74,   123,    81,    -1,    74,    30,
     136,   108,    81,    -1,    74,    30,   136,    81,    -1,    74,
     121,    76,   122,    76,   122,    81,    -1,    74,   121,    76,
     122,    81,    -1,    -1,   108,    -1,    -1,   108,    -1,   108,
      -1,     8,   108,    -1,   123,    78,   108,    -1,   125,    -1,
     124,    78,   125,    -1,    30,    79,   108,    -1,    -1,     7,
      30,    75,   129,    80,   127,    84,    -1,    -1,    28,     7,
      30,    75,   129,    80,   128,    84,    -1,    -1,   130,    -1,
       8,   130,    -1,   129,    78,   130,    -1,    30,    -1,    30,
      79,   108,    -1,    38,    30,    -1,    -1,    14,   108,   132,
      84,    -1,    18,   108,    84,   134,   135,    -1,    -1,    19,
     108,    84,   134,    -1,    -1,    20,    84,    -1,    -1,    15,
      30,    52,   108,   137,    84,    -1,    -1,    15,    75,    30,
      52,   108,    80,   138,    84,    -1,   140,   141,    -1,   140,
     142,    -1,   140,   141,   142,    -1,    11,    84,    -1,    12,
      75,    30,    80,    84,    -1,    12,    84,    -1,    12,    75,
      30,     4,    30,    80,    84,    -1,    13,    84,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    93,    93,    94,    99,   100,   101,   102,   103,   106,
     107,   110,   111,   112,   113,   114,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   130,   130,   131,   131,   132,
     132,   133,   133,   134,   134,   135,   135,   136,   136,   137,
     137,   141,   142,   143,   144,   148,   149,   153,   154,   158,
     159,   163,   167,   168,   172,   173,   174,   179,   180,   184,
     185,   189,   190,   194,   194,   194,   194,   195,   195,   195,
     196,   196,   196,   197,   197,   202,   206,   207,   208,   212,
     213,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   244,   248,   249,
     250,   251,   252,   253,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   270,   271,   272,   273,   277,   278,
     282,   286,   287,   292,   293,   294,   299,   300,   301,   302,
     306,   307,   311,   312,   316,   317,   321,   322,   323,   327,
     328,   332,   336,   336,   337,   337,   341,   342,   343,   344,
     348,   349,   350,   355,   355,   359,   363,   364,   368,   369,
     373,   373,   374,   374,   379,   380,   381,   385,   389,   390,
     391,   396
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
  "ENUM", "LBRACE", "RBRACE", "FROM", "CLASS", "ASYNC", "AWAIT", "IDENT",
  "STRING", "INTEGER", "DECIMAL", "LSHIFT", "RSHIFT", "PLUS", "MINUS",
  "STAR", "DIVIDE", "MODULO", "FLOOR", "EXPONENT", "BITAND", "BITOR",
  "BITXOR", "MINUS_EQ", "DIV_EQ", "BITNOT", "MUL_EQ", "MOD_EQ", "LE", "IN",
  "EXP_EQ", "FLDIV_EQ", "R_SHIFT_EQ", "XOR_EQ", "NONE", "AND_EQ", "OR_EQ",
  "TRUE", "FALSE", "LAND", "PLUS_EQ", "INVERT", "EQ", "NE", "LT", "GT",
  "GE", "LOR", "FSTRING", "L_SHIFT_EQ", "'.'", "'['", "'('", "':'", "';'",
  "','", "'='", "')'", "']'", "$accept", "program", "block", "stmt_list",
  "statement", "simple_stmt", "compound_stmt", "@1", "@2", "@3", "@4",
  "@5", "@6", "@7", "@8", "import", "module", "import_items",
  "import_name", "enum", "enumerations", "enumeration", "class",
  "assignment", "left_side", "equals", "value", "ternary", "lambda",
  "binop", "postfix", "term", "args", "boolean", "set", "dict",
  "dict_items", "list", "sliced_items", "slice_start", "slice_end",
  "values", "kwargs", "kwarg", "func", "@9", "@10", "params", "param_item",
  "while_stmt", "@11", "if_stmt", "elif_chain", "else_block", "for_stmt",
  "@12", "@13", "exception", "try_block", "catch_block", "final_block", 0
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
     325,   326,   327,    46,    91,    40,    58,    59,    44,    61,
      41,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    82,    83,    83,    84,    84,    84,    84,    84,    85,
      85,    86,    86,    86,    86,    86,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    89,    88,    90,    88,    91,
      88,    92,    88,    93,    88,    94,    88,    95,    88,    96,
      88,    97,    97,    97,    97,    98,    98,    99,    99,   100,
     100,   101,   102,   102,   103,   103,   103,   104,   104,   105,
     105,   106,   106,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   108,   109,   109,   109,   110,
     110,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   112,   112,
     112,   112,   112,   112,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   114,   114,   114,   114,   115,   115,
     116,   117,   117,   118,   118,   118,   119,   119,   119,   119,
     120,   120,   121,   121,   122,   122,   123,   123,   123,   124,
     124,   125,   127,   126,   128,   126,   129,   129,   129,   129,
     130,   130,   130,   132,   131,   133,   134,   134,   135,   135,
     137,   136,   138,   136,   139,   139,   139,   140,   141,   141,
     141,   142
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
       3,     3,     2,     3,     4,     5,     2,     3,     5,     4,
       7,     5,     0,     1,     0,     1,     1,     2,     3,     1,
       3,     3,     0,     7,     0,     8,     0,     1,     2,     3,
       1,     3,     2,     0,     4,     5,     0,     4,     0,     2,
       0,     6,     0,     8,     2,     2,     3,     2,     5,     2,
       7,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   156,    13,     0,     0,     0,     0,     0,     0,
       0,     0,   122,   116,   114,   115,     0,     0,     0,   123,
     128,   129,     0,   117,     0,     0,     0,     0,     9,    11,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    79,    81,   108,   118,   121,   120,
     119,    14,    15,     0,   160,     0,     0,   157,    21,     0,
      81,     0,    22,    23,     0,     0,     0,   132,   146,     0,
       0,    78,   107,   106,   105,   104,     0,   122,   136,   146,
       0,     0,     1,    10,     0,     0,    26,     0,    28,     0,
      30,     0,    32,     0,    34,     0,     0,    36,     0,    38,
       0,     0,    40,    16,    64,    65,    66,    69,    68,    67,
      70,    72,    73,    74,    63,    71,     0,     0,     0,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,   124,   113,   158,     0,   162,     0,
       0,    20,    24,    18,    19,   147,     0,   131,     0,   130,
       0,   147,     0,   137,    82,     0,     0,     0,     0,   163,
       0,     0,    45,     0,     0,     0,     0,     0,   177,     0,
       0,   174,   175,     0,    62,    59,    60,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    87,    85,    86,
      92,   103,    84,    88,    89,    90,    91,    93,    83,   111,
     143,     0,     0,   125,     0,   161,    80,   159,     0,   133,
       0,   148,   139,     0,   156,     0,   124,    57,   166,     0,
       0,     0,     0,     0,    41,     0,     7,     0,     5,     0,
       8,     0,   179,   181,   176,     0,     0,   112,   144,   127,
       0,   109,   134,     0,   138,     0,   156,     0,     0,   168,
     164,   170,     0,     0,    46,    49,     0,     0,    47,     6,
       4,     0,     0,    54,     0,    52,    77,   145,     0,   122,
     126,     0,   149,   135,   152,     0,     0,     0,     0,     0,
     165,     0,     0,    42,    44,    43,     0,     0,     0,     0,
      56,     0,    51,     0,   144,   141,     0,     0,   110,     0,
     154,    58,   166,   169,   171,   172,    48,    50,     0,   178,
      55,    53,     0,   151,     0,   150,   153,     0,   167,     0,
       0,   140,   155,   173,   180
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,   178,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    97,   173,   267,   268,   102,
     274,   275,    88,    39,    40,   118,    41,    42,    43,    44,
      60,    46,   214,    47,    48,    49,    69,    50,   145,   211,
     278,    70,   281,   282,    86,   309,   327,    56,    57,    92,
     229,    90,   259,   290,    94,   291,   329,    99,   100,   181,
     182
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -254
static const yytype_int16 yypact[] =
{
     296,    14,    33,  -254,   570,   825,   -44,   -32,    19,    30,
     681,   919,  -254,  -254,  -254,  -254,   938,   938,   938,  -254,
    -254,  -254,   938,  -254,    22,   825,    65,   372,  -254,  -254,
    -254,    16,    78,    74,   102,   112,    35,   126,   115,    73,
    1191,    76,  -254,   135,   984,  1157,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,    -2,    86,   125,    39,  -254,  -254,    80,
      37,    91,  -254,  -254,    92,    94,   825,  -254,    96,   -11,
     -10,  -254,  -254,  -254,  -254,  -254,   825,   112,  -254,  -254,
      28,    90,  -254,  -254,   143,   167,  -254,   145,  -254,   825,
    -254,   825,  -254,    -6,  -254,   146,   146,  -254,    24,  -254,
      61,   147,  -254,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,   973,   729,   729,  -254,
     919,   938,   938,   938,   938,   938,   938,   938,   938,   938,
     938,   938,   938,   938,   938,   938,   938,   938,   938,   938,
     938,   938,   171,   825,   777,  -254,  -254,   825,  -254,   919,
      -2,  -254,  -254,  -254,  -254,   127,   825,  -254,   825,  -254,
     825,  -254,   199,  -254,  -254,   130,   172,    23,    24,  -254,
     154,   177,  -254,     7,    15,    24,   447,   633,  -254,    32,
      24,   196,  -254,   186,    37,   133,   133,   136,    93,    93,
     121,   121,   174,   174,   174,   174,   174,  1114,  1076,  1095,
    1133,  1133,  1057,  1133,  1133,  1133,  1133,  1133,  1021,  -254,
     132,   141,   825,  -254,    42,  -254,  -254,  -254,   825,  -254,
     142,  -254,  -254,   138,    33,   149,   777,  -254,   201,    24,
     825,   169,   195,   197,  -254,     5,  -254,     1,  -254,   522,
    -254,   203,  -254,  -254,  -254,    34,   825,  -254,   825,  -254,
     873,  -254,  -254,   825,  -254,    43,    33,    64,   825,   214,
    -254,  -254,   825,   160,  -254,  -254,   161,    17,   166,  -254,
    -254,     9,    34,   163,    12,  -254,  -254,  -254,   -42,   164,
    -254,    67,  -254,  -254,  -254,    71,   825,    24,    24,    24,
    -254,    24,   165,  -254,  -254,  -254,   210,   218,   220,    24,
    -254,   219,  -254,    34,   825,  -254,   825,   222,  -254,    24,
    -254,  -254,   201,  -254,  -254,  -254,   166,  -254,   173,  -254,
    -254,  -254,   176,  -254,   164,  -254,  -254,    24,  -254,    24,
      24,  -254,  -254,  -254,  -254
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -254,  -254,  -163,    79,   -24,    84,  -254,  -254,  -254,  -254,
    -254,  -254,  -254,  -254,  -254,  -254,   168,  -254,   -34,  -254,
    -254,  -253,  -254,  -254,  -254,  -254,    -4,    20,    -1,    59,
       2,  -254,    41,  -254,  -254,  -254,  -254,  -254,  -254,  -254,
     -39,   -16,  -254,   -38,  -254,  -254,  -254,  -199,   -46,  -254,
    -254,  -254,   -41,  -254,   191,  -254,  -254,  -254,  -254,  -254,
      95
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -62
static const yytype_int16 yytable[] =
{
      59,    61,    45,    83,   227,   228,    68,   146,    80,    51,
      71,   232,   236,   298,   157,   159,   242,   243,   235,   300,
      79,    81,    51,    84,   170,   255,   269,     2,    54,    45,
      76,   175,   175,    62,   304,   265,    55,   302,    95,   305,
     175,    53,   272,   266,    85,    63,    10,   176,   176,    64,
     321,    11,    77,    13,    14,    15,   176,   285,    16,    17,
      65,    96,   155,    54,   273,    82,   260,   158,   160,   171,
      18,    55,   161,   179,   180,    72,    73,    74,    52,    19,
     233,    75,    20,    21,   234,   168,    22,   169,   233,   299,
     303,    52,    89,    23,   295,   296,    24,    25,   226,   177,
     177,   185,   186,    78,   217,    87,   160,   241,   177,   163,
     142,   143,   144,    79,    79,   149,    91,   150,   184,   187,
     250,   150,   251,   284,   311,   312,   313,    93,   314,   123,
     124,   125,   126,   127,   128,   129,   319,    98,   101,   210,
     213,   120,   286,   215,   287,   307,   326,   308,   216,   150,
     103,   310,   219,   119,   220,   148,   221,   151,   223,   125,
     126,   127,   128,   129,   332,   147,   333,   334,   152,   153,
     164,   154,   156,   165,   166,   167,   172,   183,    45,    45,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   225,   218,     2,   224,   230,   231,   249,   180,
     245,   160,   246,   247,   252,    83,   129,   248,   253,   254,
     258,   262,   213,    10,   256,   263,   261,   264,    11,    12,
      13,    14,    15,   271,   289,    16,    17,   293,   294,   297,
     265,    45,   301,   306,   277,   315,   280,    18,   317,   283,
     318,   320,   324,   330,   288,   239,    19,   331,   292,    20,
      21,   240,   316,    22,   174,   322,   276,   257,   162,   325,
      23,   328,     0,    24,    25,     0,   244,     0,     0,     0,
     222,     0,   280,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    -2,     1,     0,   -35,
     277,     2,   323,   -25,     3,     4,     5,   -37,     0,     0,
     -31,   -33,     6,     7,   -29,     0,     0,     8,     9,   -39,
      10,     0,   -35,   -27,   -25,    11,    12,    13,    14,    15,
       0,     0,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,    19,     0,     0,    20,    21,     0,     0,
      22,     0,     0,     0,     0,     0,     0,    23,     0,     0,
      24,    25,    -3,     1,     0,   -35,     0,     2,     0,   -25,
       3,     4,     5,   -37,     0,     0,   -31,   -33,     6,     7,
     -29,     0,     0,     8,     9,   -39,    10,     0,   -35,   -27,
     -25,    11,    12,    13,    14,    15,     0,     0,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       0,     0,    20,    21,     0,     0,    22,     0,     0,     0,
       0,     0,     0,    23,     0,     0,    24,    25,   237,     0,
     -35,     0,     2,     0,   -25,     3,     4,     5,   -37,     0,
       0,   -31,   -33,     6,     7,   -29,     0,     0,     8,     9,
     -39,    10,   238,   -35,   -27,   -25,    11,    12,    13,    14,
      15,     0,     0,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,     0,     0,     0,     0,
       0,     0,     0,     0,    19,     0,     0,    20,    21,     0,
       0,    22,     0,     0,     0,     0,     0,     0,    23,     0,
       0,    24,    25,     1,     0,   -35,     0,     2,     0,   -25,
       3,     4,     5,   -37,     0,     0,   -31,   -33,     6,     7,
     -29,     0,     0,     8,     9,   -39,    10,   270,   -35,   -27,
     -25,    11,    12,    13,    14,    15,     0,     0,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,     0,     0,     0,     0,     2,     0,     0,     0,    19,
       0,     0,    20,    21,     0,     0,    22,     0,     0,     0,
       0,     0,     0,    23,    10,     0,    24,    25,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,    19,     0,     0,
      20,    21,     0,     0,    22,     0,     0,     0,     2,     0,
       0,    23,     4,     5,    24,    25,     0,    58,     0,     6,
       7,     0,     0,     0,     8,     9,     0,    10,     0,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,     0,     0,     0,     2,     0,     0,    66,
      19,     0,     0,    20,    21,     0,     0,    22,     0,     0,
       0,     0,     0,     0,    23,    10,    67,    24,    25,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     0,     0,     2,     0,     0,    76,    19,     0,
       0,    20,    21,     0,     0,    22,     0,     0,     0,     0,
       0,     0,    23,    10,     0,    24,    25,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,     0,     2,     0,     0,   212,    19,     0,     0,    20,
      21,     0,     0,    22,     0,     0,     0,     0,     0,     0,
      23,    10,     0,    24,    25,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,     0,     0,     0,     0,
       2,     0,     0,     0,    19,     0,     0,    20,    21,     0,
       0,    22,     0,     0,     0,     0,     0,     0,    23,    10,
       0,    24,    25,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,     0,     0,     0,     0,     2,     0,
       0,     0,    19,     0,     0,    20,    21,     0,     0,    22,
       0,     0,     0,     0,     0,     0,    23,    10,     0,    24,
      25,     0,    11,   279,    13,    14,    15,     0,     0,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,     0,     2,     0,     0,     0,     0,     0,
      19,     0,     0,    20,    21,     0,     0,    22,     0,     0,
       0,     0,     0,    10,    23,     0,     0,    24,    25,    12,
      13,    14,    15,     0,     0,    16,    17,     0,     0,     0,
       0,     0,    10,     0,     0,     0,     0,    18,    12,    13,
      14,    15,     0,     0,    16,    17,    19,     0,     0,    20,
      21,     0,     0,    22,     0,     0,    18,     0,     0,     0,
      23,     0,     0,    24,    25,    19,     0,    10,    20,    21,
       0,     0,    22,    12,    13,    14,    15,     0,     0,    23,
       0,     0,    24,    25,     0,     0,     0,     0,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
      19,     0,     0,    20,    21,   133,   134,     0,     0,     0,
       0,     0,     0,     0,    23,     0,   135,    24,     0,   136,
     137,   138,   139,   140,   141,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,     0,     0,     0,
       0,     0,   133,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,     0,     0,   136,   137,   138,   139,
     140,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,     0,     0,     0,     0,     0,   133,   134,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
       0,   132,   136,   137,   138,   139,   140,   133,   134,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,     0,
       0,   136,   137,   138,   139,   140,   133,   134,   121,   122,
     123,   124,   125,   126,   127,   128,   129,     0,     0,     0,
     136,   137,   138,   139,   140,   133,   134,   121,   122,   123,
     124,   125,   126,   127,   128,   129,     0,     0,     0,   136,
     137,   138,   139,   140,   -62,   -62,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -62,   -62,
     -62,   -62,   -62,   -61,   -61,     0,   -61,   -61,     0,     0,
     -61,   -61,   -61,   -61,     0,   -61,   -61,     0,     0,     0,
     -61,     0,     0,     0,     0,     0,     0,     0,     0,   -61,
     142,   143,   144,     0,     0,   -61,   -61,   104,   105,     0,
     106,   107,     0,     0,   108,   109,   110,   111,     0,   112,
     113,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,   115,     0,     0,     0,     0,     0,   116,
     117
};

static const yytype_int16 yycheck[] =
{
       4,     5,     0,    27,   167,   168,    10,    53,    24,     8,
      11,     4,   175,     4,    25,    25,   179,   180,     3,   272,
      24,    25,     8,     7,    30,   224,    25,     5,    30,    27,
       8,     8,     8,    77,    76,    30,    38,    25,     3,    81,
       8,     8,     8,    38,    28,    77,    24,    24,    24,    30,
     303,    29,    30,    31,    32,    33,    24,   256,    36,    37,
      30,    26,    66,    30,    30,     0,   229,    78,    78,    75,
      48,    38,    76,    12,    13,    16,    17,    18,    77,    57,
      73,    22,    60,    61,    77,    89,    64,    91,    73,    80,
      78,    77,    18,    71,    77,    78,    74,    75,    75,    76,
      76,   117,   118,    81,   150,    27,    78,    75,    76,    81,
      73,    74,    75,   117,   118,    76,    14,    78,   116,   120,
      78,    78,    80,    80,   287,   288,   289,    15,   291,    36,
      37,    38,    39,    40,    41,    42,   299,    11,    23,   143,
     144,     6,    78,   147,    80,    78,   309,    80,   149,    78,
      77,    80,   156,    77,   158,    30,   160,    77,   162,    38,
      39,    40,    41,    42,   327,    79,   329,   330,    77,    77,
      80,    77,    76,    30,     7,    30,    30,    30,   176,   177,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,    30,    30,    76,     5,    75,    52,    30,   212,    13,
      24,    78,    76,    81,   218,   239,    42,    76,    76,    81,
      19,    52,   226,    24,    75,    30,   230,    30,    29,    30,
      31,    32,    33,    30,    20,    36,    37,    77,    77,    73,
      30,   239,    79,    79,   248,    80,   250,    48,    30,   253,
      30,    32,    30,    80,   258,   176,    57,    81,   262,    60,
      61,   177,   296,    64,    96,   304,   246,   226,    77,   307,
      71,   312,    -1,    74,    75,    -1,   181,    -1,    -1,    -1,
      81,    -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,
     304,     5,   306,     7,     8,     9,    10,    11,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    -1,    21,    22,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      74,    75,     0,     1,    -1,     3,    -1,     5,    -1,     7,
       8,     9,    10,    11,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    21,    22,    23,    24,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    74,    75,     1,    -1,
       3,    -1,     5,    -1,     7,     8,     9,    10,    11,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    74,    75,     1,    -1,     3,    -1,     5,    -1,     7,
       8,     9,    10,    11,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,     5,    -1,    -1,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    24,    -1,    74,    75,    -1,    29,
      30,    31,    32,    33,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    -1,    -1,     5,    -1,
      -1,    71,     9,    10,    74,    75,    -1,    77,    -1,    16,
      17,    -1,    -1,    -1,    21,    22,    -1,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,     5,    -1,    -1,     8,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    24,    25,    74,    75,    -1,
      29,    30,    31,    32,    33,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,     5,    -1,    -1,     8,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    24,    -1,    74,    75,    -1,    29,    30,
      31,    32,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,     5,    -1,    -1,     8,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    24,    -1,    74,    75,    -1,    29,    30,    31,    32,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
       5,    -1,    -1,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    24,
      -1,    74,    75,    -1,    29,    30,    31,    32,    33,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,     5,    -1,
      -1,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    24,    -1,    74,
      75,    -1,    29,    30,    31,    32,    33,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    24,    71,    -1,    -1,    74,    75,    30,
      31,    32,    33,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    48,    30,    31,
      32,    33,    -1,    -1,    36,    37,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    -1,    48,    -1,    -1,    -1,
      71,    -1,    -1,    74,    75,    57,    -1,    24,    60,    61,
      -1,    -1,    64,    30,    31,    32,    33,    -1,    -1,    71,
      -1,    -1,    74,    75,    -1,    -1,    -1,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      57,    -1,    -1,    60,    61,    51,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    62,    74,    -1,    65,
      66,    67,    68,    69,    70,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    -1,    -1,
      -1,    -1,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    65,    66,    67,    68,
      69,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    -1,    -1,    -1,    -1,    51,    52,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    45,    65,    66,    67,    68,    69,    51,    52,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      -1,    65,    66,    67,    68,    69,    51,    52,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    51,    52,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    -1,    -1,    -1,    65,
      66,    67,    68,    69,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,
      67,    68,    69,    46,    47,    -1,    49,    50,    -1,    -1,
      53,    54,    55,    56,    -1,    58,    59,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    -1,    -1,    78,    79,    46,    47,    -1,
      49,    50,    -1,    -1,    53,    54,    55,    56,    -1,    58,
      59,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    78,
      79
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     5,     8,     9,    10,    16,    17,    21,    22,
      24,    29,    30,    31,    32,    33,    36,    37,    48,    57,
      60,    61,    64,    71,    74,    75,    83,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,   105,
     106,   108,   109,   110,   111,   112,   113,   115,   116,   117,
     119,     8,    77,     8,    30,    38,   129,   130,    77,   108,
     112,   108,    77,    77,    30,    30,     8,    25,   108,   118,
     123,   110,   111,   111,   111,   111,     8,    30,    81,   108,
     123,   108,     0,    86,     7,    28,   126,    27,   104,    18,
     133,    14,   131,    15,   136,     3,    26,    97,    11,   139,
     140,    23,   101,    77,    46,    47,    49,    50,    53,    54,
      55,    56,    58,    59,    63,    72,    78,    79,   107,    77,
       6,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    51,    52,    62,    65,    66,    67,    68,
      69,    70,    73,    74,    75,   120,   130,    79,    30,    76,
      78,    77,    77,    77,    77,   108,    76,    25,    78,    25,
      78,   108,   136,    81,    80,    30,     7,    30,   108,   108,
      30,    75,    30,    98,    98,     8,    24,    76,    84,    12,
      13,   141,   142,    30,   112,   123,   123,   110,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,    30,
     108,   121,     8,   108,   114,   108,   110,   130,    76,   108,
     108,   108,    81,   108,    75,    30,    75,    84,    84,   132,
      52,    30,     4,    73,    77,     3,    84,     1,    25,    85,
      87,    75,    84,    84,   142,    24,    76,    81,    76,   108,
      78,    80,   108,    76,    81,   129,    75,   114,    19,   134,
      84,   108,    52,    30,    30,    30,    38,    99,   100,    25,
      25,    30,     8,    30,   102,   103,   109,   108,   122,    30,
     108,   124,   125,   108,    80,   129,    78,    80,   108,    20,
     135,   137,   108,    77,    77,    77,    78,    73,     4,    80,
     103,    79,    25,    78,    76,    81,    79,    78,    80,   127,
      80,    84,    84,    84,    84,    80,   100,    30,    30,    84,
      32,   103,   122,   108,    30,   125,    84,   128,   134,   138,
      80,    81,    84,    84,    84
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
#line 93 "./parse/parser.y"
    { global_statements = list_create(0); }
    break;

  case 3:
#line 94 "./parse/parser.y"
    { global_statements = (yyvsp[(1) - (1)].generic); }
    break;

  case 4:
#line 99 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (3)].generic); }
    break;

  case 5:
#line 100 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 6:
#line 101 "./parse/parser.y"
    { (yyval.generic) = list_create(0); yyerrok; }
    break;

  case 7:
#line 102 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;

  case 8:
#line 103 "./parse/parser.y"
    { (yyval.generic) = list_create(1); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 9:
#line 106 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 10:
#line 107 "./parse/parser.y"
    { list_append((yyvsp[(2) - (2)].generic), (yyvsp[(1) - (2)].generic)); (yyval.generic) = (yyvsp[(1) - (2)].generic); }
    break;

  case 11:
#line 110 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 12:
#line 111 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 13:
#line 112 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 14:
#line 113 "./parse/parser.y"
    { (yyval.generic) = NULL; yyerrok; }
    break;

  case 15:
#line 114 "./parse/parser.y"
    { (yyval.generic) = NULL; yyerrok; }
    break;

  case 16:
#line 118 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_ASMT, (yyvsp[(1) - (2)].generic), yylineno-1); }
    break;

  case 17:
#line 119 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_EXPR, (yyvsp[(1) - (2)].generic), yylineno-1); }
    break;

  case 18:
#line 120 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_GLOBAL, (yyvsp[(2) - (3)].generic), yylineno-1); }
    break;

  case 19:
#line 121 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_NONLOCAL, (yyvsp[(2) - (3)].generic), yylineno-1); }
    break;

  case 20:
#line 122 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn((yyvsp[(2) - (3)].generic), f_depth), yylineno-1); }
    break;

  case 21:
#line 123 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_RETURN, createReturn(NULL, f_depth), yylineno-1); }
    break;

  case 22:
#line 124 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_CONTINUE, createJump(l_depth, 0), yylineno-1); }
    break;

  case 23:
#line 125 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_BREAK, createJump(l_depth, 1), yylineno-1); }
    break;

  case 24:
#line 126 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_THROW, (yyvsp[(2) - (3)].generic), yylineno-1); }
    break;

  case 25:
#line 130 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 26:
#line 130 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FUNC, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 27:
#line 131 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 28:
#line 131 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_CLASS, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 29:
#line 132 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 30:
#line 132 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FLOW, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 31:
#line 133 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 32:
#line 133 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_WHILE, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 33:
#line 134 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 34:
#line 134 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_FOR, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 35:
#line 135 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 36:
#line 135 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_IMPORT, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 37:
#line 136 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 38:
#line 136 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_EXCEPTION, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 39:
#line 137 "./parse/parser.y"
    { (yyval.generic) = (void*)(long)yylineno; }
    break;

  case 40:
#line 137 "./parse/parser.y"
    { (yyval.generic) = createStatement(STMT_ENUM, (yyvsp[(2) - (2)].generic), (int)(long)(yyvsp[(1) - (2)].generic)); }
    break;

  case 41:
#line 141 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (3)].generic), NULL, NULL, 0); }
    break;

  case 42:
#line 142 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (5)].generic), NULL, (yyvsp[(4) - (5)].generic), 0); }
    break;

  case 43:
#line 143 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (5)].generic), (yyvsp[(4) - (5)].generic), NULL, 0); }
    break;

  case 44:
#line 144 "./parse/parser.y"
    { (yyval.generic) = createImport((yyvsp[(2) - (5)].generic), NULL, NULL, 1); ast_free((yyvsp[(4) - (5)].generic)); }
    break;

  case 45:
#line 148 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 46:
#line 149 "./parse/parser.y"
    { (yyval.generic) = str_concat(str_concat((yyvsp[(1) - (3)].generic), "."), (yyvsp[(3) - (3)].generic)); }
    break;

  case 47:
#line 153 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 48:
#line 154 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 49:
#line 158 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 50:
#line 159 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 51:
#line 163 "./parse/parser.y"
    { (yyval.generic) = createEnum((yyvsp[(2) - (5)].generic), (yyvsp[(4) - (5)].generic)); }
    break;

  case 52:
#line 167 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__);  list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 53:
#line 168 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 54:
#line 172 "./parse/parser.y"
    { (yyval.generic) = createEnumItem((yyvsp[(1) - (1)].generic), NULL); }
    break;

  case 55:
#line 173 "./parse/parser.y"
    { (yyval.generic) = createEnumItem((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 56:
#line 174 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;

  case 57:
#line 179 "./parse/parser.y"
    { (yyval.generic) = createClass((yyvsp[(2) - (3)].generic), (yyvsp[(3) - (3)].generic), NULL); }
    break;

  case 58:
#line 180 "./parse/parser.y"
    { (yyval.generic) = createClass((yyvsp[(2) - (6)].generic), (yyvsp[(6) - (6)].generic), (yyvsp[(4) - (6)].generic)); }
    break;

  case 59:
#line 184 "./parse/parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), NULL); }
    break;

  case 60:
#line 185 "./parse/parser.y"
    { (yyval.generic) = createAssignment((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), (yyvsp[(2) - (3)].generic)); }
    break;

  case 61:
#line 189 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 62:
#line 190 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 75:
#line 202 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 76:
#line 206 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 77:
#line 207 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_TERNARY, createTernary((yyvsp[(1) - (5)].generic), (yyvsp[(3) - (5)].generic), (yyvsp[(5) - (5)].generic)))); }
    break;

  case 78:
#line 208 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_AWAIT, createAwait((yyvsp[(2) - (2)].generic)))); }
    break;

  case 79:
#line 212 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 80:
#line 213 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createLambda((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic), yylineno)); }
    break;

  case 82:
#line 218 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (3)].generic); }
    break;

  case 83:
#line 219 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 84:
#line 220 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 85:
#line 221 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 86:
#line 222 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 87:
#line 223 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 88:
#line 224 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 89:
#line 225 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 90:
#line 226 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 91:
#line 227 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 92:
#line 228 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 93:
#line 229 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 94:
#line 230 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 95:
#line 231 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 96:
#line 232 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 97:
#line 233 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 98:
#line 234 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 99:
#line 235 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 100:
#line 236 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 101:
#line 237 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 102:
#line 238 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 103:
#line 239 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(2) - (3)].generic), (yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 104:
#line 240 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 105:
#line 241 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 106:
#line 243 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), createASTnode(createData(TYPE_INT, &(int){0})), (yyvsp[(2) - (2)].generic)); }
    break;

  case 107:
#line 245 "./parse/parser.y"
    { (yyval.generic) = createASTexpr((yyvsp[(1) - (2)].generic), createASTnode(createData(TYPE_INT, &(int){0})), (yyvsp[(2) - (2)].generic)); }
    break;

  case 108:
#line 248 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 109:
#line 249 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic), NULL))); }
    break;

  case 110:
#line 250 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INVOKED, createInvoked((yyvsp[(1) - (6)].generic), (yyvsp[(3) - (6)].generic), (yyvsp[(5) - (6)].generic)))); }
    break;

  case 111:
#line 251 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_ATTRIBUTE, createAttribute((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)))); }
    break;

  case 112:
#line 252 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_INDEX, createIndexed((yyvsp[(1) - (4)].generic), (yyvsp[(3) - (4)].generic)))); }
    break;

  case 113:
#line 253 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_SLICE, createIndexed((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic)))); }
    break;

  case 114:
#line 257 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 115:
#line 258 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 116:
#line 259 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 117:
#line 260 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 118:
#line 261 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 119:
#line 262 "./parse/parser.y"
    { (yyval.generic) = createASTnode((yyvsp[(1) - (1)].generic)); }
    break;

  case 120:
#line 263 "./parse/parser.y"
    { (yyval.generic) = createASTnode((yyvsp[(1) - (1)].generic)); }
    break;

  case 121:
#line 264 "./parse/parser.y"
    { (yyval.generic) = createASTnode((yyvsp[(1) - (1)].generic)); }
    break;

  case 122:
#line 265 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_LOOKUP, (yyvsp[(1) - (1)].generic))); }
    break;

  case 123:
#line 266 "./parse/parser.y"
    { (yyval.generic) = createASTnode(createData(TYPE_NONE, NULL)); }
    break;

  case 124:
#line 270 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 125:
#line 271 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 126:
#line 272 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 127:
#line 273 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 128:
#line 277 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 129:
#line 278 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 130:
#line 282 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_SET_EXPR, (yyvsp[(2) - (3)].generic)); }
    break;

  case 131:
#line 286 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_DICT_EXPR, (yyvsp[(2) - (3)].generic)); }
    break;

  case 132:
#line 287 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_DICT_EXPR, list_create(0)); }
    break;

  case 133:
#line 292 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append(createPair((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic)), (yyval.generic)); }
    break;

  case 134:
#line 293 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append(createPair((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic)), (yyval.generic)); }
    break;

  case 135:
#line 294 "./parse/parser.y"
    { list_append(createPair((yyvsp[(3) - (5)].generic), (yyvsp[(5) - (5)].generic)), (yyvsp[(1) - (5)].generic)); (yyval.generic) = (yyvsp[(1) - (5)].generic); }
    break;

  case 136:
#line 299 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_LIST_EXPR, list_create(0)); }
    break;

  case 137:
#line 300 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_LIST_EXPR, (yyvsp[(2) - (3)].generic)); }
    break;

  case 138:
#line 301 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_GENERATOR, createGenerator((yyvsp[(2) - (5)].generic), (yyvsp[(3) - (5)].generic), (yyvsp[(4) - (5)].generic))); }
    break;

  case 139:
#line 302 "./parse/parser.y"
    { (yyval.generic) = createData(TYPE_GENERATOR, createGenerator((yyvsp[(2) - (4)].generic), (yyvsp[(3) - (4)].generic), NULL)); }
    break;

  case 140:
#line 306 "./parse/parser.y"
    { (yyval.generic) = createSlice((yyvsp[(2) - (7)].generic), (yyvsp[(4) - (7)].generic), (yyvsp[(6) - (7)].generic)); }
    break;

  case 141:
#line 307 "./parse/parser.y"
    { (yyval.generic) = createSlice((yyvsp[(2) - (5)].generic), (yyvsp[(4) - (5)].generic), NULL); }
    break;

  case 142:
#line 311 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 143:
#line 312 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 144:
#line 316 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 145:
#line 317 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(1) - (1)].generic); }
    break;

  case 146:
#line 321 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 147:
#line 322 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 148:
#line 323 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 149:
#line 327 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 150:
#line 328 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 151:
#line 332 "./parse/parser.y"
    { (yyval.generic) = createPair(createData(TYPE_STR, (yyvsp[(1) - (3)].generic)), (yyvsp[(3) - (3)].generic)); }
    break;

  case 152:
#line 336 "./parse/parser.y"
    { f_depth++; }
    break;

  case 153:
#line 336 "./parse/parser.y"
    { (yyval.generic) = createFunction((yyvsp[(2) - (7)].generic), (yyvsp[(4) - (7)].generic), (yyvsp[(7) - (7)].generic), 0, &f_depth); }
    break;

  case 154:
#line 337 "./parse/parser.y"
    { f_depth++; }
    break;

  case 155:
#line 337 "./parse/parser.y"
    { (yyval.generic) = createFunction((yyvsp[(3) - (8)].generic), (yyvsp[(5) - (8)].generic), (yyvsp[(8) - (8)].generic), 1, &f_depth); }
    break;

  case 156:
#line 341 "./parse/parser.y"
    { (yyval.generic) = list_create(0); }
    break;

  case 157:
#line 342 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(1) - (1)].generic), (yyval.generic)); }
    break;

  case 158:
#line 343 "./parse/parser.y"
    { (yyval.generic) = list_create(__len__); list_append((yyvsp[(2) - (2)].generic), (yyval.generic)); }
    break;

  case 159:
#line 344 "./parse/parser.y"
    { list_append((yyvsp[(3) - (3)].generic), (yyvsp[(1) - (3)].generic)); (yyval.generic) = (yyvsp[(1) - (3)].generic); }
    break;

  case 160:
#line 348 "./parse/parser.y"
    { (yyval.generic) = createParam((yyvsp[(1) - (1)].generic), NULL, 0); }
    break;

  case 161:
#line 349 "./parse/parser.y"
    { (yyval.generic) = createParam((yyvsp[(1) - (3)].generic), (yyvsp[(3) - (3)].generic), 0); }
    break;

  case 162:
#line 350 "./parse/parser.y"
    { ast_free((yyvsp[(1) - (2)].generic)); (yyval.generic) = createParam((yyvsp[(2) - (2)].generic), NULL, 1); }
    break;

  case 163:
#line 355 "./parse/parser.y"
    { l_depth++; }
    break;

  case 164:
#line 355 "./parse/parser.y"
    { (yyval.generic) = createWhile((yyvsp[(2) - (4)].generic), (yyvsp[(4) - (4)].generic), &l_depth); }
    break;

  case 165:
#line 359 "./parse/parser.y"
    { (yyval.generic) = createFlow((yyvsp[(2) - (5)].generic), (yyvsp[(3) - (5)].generic), (yyvsp[(4) - (5)].generic), (yyvsp[(5) - (5)].generic)); }
    break;

  case 166:
#line 363 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 167:
#line 364 "./parse/parser.y"
    { (yyval.generic) = createFlow((yyvsp[(2) - (4)].generic), (yyvsp[(3) - (4)].generic), (yyvsp[(4) - (4)].generic), NULL); }
    break;

  case 168:
#line 368 "./parse/parser.y"
    { (yyval.generic) = NULL; }
    break;

  case 169:
#line 369 "./parse/parser.y"
    { (yyval.generic) = createFlow(NULL, (yyvsp[(2) - (2)].generic), NULL, NULL); }
    break;

  case 170:
#line 373 "./parse/parser.y"
    { l_depth++; }
    break;

  case 171:
#line 373 "./parse/parser.y"
    { ast_free((yyvsp[(3) - (6)].generic)); (yyval.generic) = createFor((yyvsp[(2) - (6)].generic), (yyvsp[(4) - (6)].generic), (yyvsp[(6) - (6)].generic), &l_depth); }
    break;

  case 172:
#line 374 "./parse/parser.y"
    { l_depth++; }
    break;

  case 173:
#line 374 "./parse/parser.y"
    { ast_free((yyvsp[(4) - (8)].generic)); (yyval.generic) = createFor((yyvsp[(3) - (8)].generic), (yyvsp[(5) - (8)].generic), (yyvsp[(8) - (8)].generic), &l_depth);}
    break;

  case 174:
#line 379 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic), NULL); }
    break;

  case 175:
#line 380 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (2)].generic), (yyvsp[(2) - (2)].generic), NULL); }
    break;

  case 176:
#line 381 "./parse/parser.y"
    { (yyval.generic) = createException((yyvsp[(1) - (3)].generic), (yyvsp[(2) - (3)].generic), (yyvsp[(3) - (3)].generic)); }
    break;

  case 177:
#line 385 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;

  case 178:
#line 389 "./parse/parser.y"
    { (yyval.generic) = createCatch((yyvsp[(3) - (5)].generic), NULL, (yyvsp[(5) - (5)].generic)); }
    break;

  case 179:
#line 390 "./parse/parser.y"
    { (yyval.generic) = createCatch(NULL, NULL, (yyvsp[(2) - (2)].generic)); }
    break;

  case 180:
#line 391 "./parse/parser.y"
    { (yyval.generic) = createCatch((yyvsp[(3) - (7)].generic), (yyvsp[(5) - (7)].generic), (yyvsp[(7) - (7)].generic)); }
    break;

  case 181:
#line 396 "./parse/parser.y"
    { (yyval.generic) = (yyvsp[(2) - (2)].generic); }
    break;


/* Line 1267 of yacc.c.  */
#line 2829 "y.tab.c"
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


#line 399 "./parse/parser.y"


int main(int argc, char **argv)                        { return run(argc, argv); }
