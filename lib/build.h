//=========================================================
//  COMPLETE BUILTIN REGISTRY SYSTEM
//=========================================================

#ifndef BUILTIN_H
#define BUILTIN_H

#include "./lib/type.h"

// Macros
#define ARG(i) arg_data(args, i)
#define ARG_INT(i, def) arg_int(args, i, def)
#define ARG_STR(i, def) arg_str(args, i, def)
#define UNUSED(x) (void)(x)

//=========================================================
//  Builtin Type Enumerations
//=========================================================

typedef enum
{
   // Core builtin functions
   BUILTIN_PRINT,
   BUILTIN_INPUT,
   BUILTIN_TYPE,
   BUILTIN_INT,
   BUILTIN_FLOAT,
   BUILTIN_STR,
   BUILTIN_BOOL,
   BUILTIN_LEN,
   BUILTIN_ABS,
   BUILTIN_MIN,
   BUILTIN_MAX,
   BUILTIN_SUM,
   BUILTIN_POW,
   BUILTIN_ROUND,
   BUILTIN_SORTED,
   BUILTIN_REVERSED,
   BUILTIN_RANGE,
   BUILTIN_LIST,
   BUILTIN_DICT,
   BUILTIN_SET,
   BUILTIN_TUPLE,
   BUILTIN_ENUMERATE,
   BUILTIN_ZIP,
   BUILTIN_MAP,
   BUILTIN_FILTER,
   BUILTIN_ALL,
   BUILTIN_ANY,
   BUILTIN_CHR,
   BUILTIN_ORD,
   BUILTIN_HEX,
   BUILTIN_BIN,
   BUILTIN_OCT,
   BUILTIN_HASH,
   BUILTIN_ID,
   BUILTIN_ISINSTANCE,
   BUILTIN_ISSUBCLASS,
   BUILTIN_CALLABLE,
   BUILTIN_HASATTR,
   BUILTIN_GETATTR,
   BUILTIN_SETATTR,
   BUILTIN_DELATTR,
   BUILTIN_DIR,
   BUILTIN_VARS,
   BUILTIN_LOCALS,
   BUILTIN_GLOBALS,
   BUILTIN_EVAL,
   BUILTIN_EXEC,
   BUILTIN_COMPILE,
   BUILTIN_OPEN,
   BUILTIN_HELP,

   // List methods (modify in place)
   LIST_APPEND,
   LIST_EXTEND,
   LIST_INSERT,
   LIST_REMOVE,
   LIST_POP,
   LIST_CLEAR,
   LIST_INDEX,
   LIST_COUNT,
   LIST_REVERSE,
   LIST_COPY,

   // List methods (return new value)
   LIST_SLICE,
   LIST_GET,

   // Dict methods
   DICT_GET,
   DICT_KEYS,
   DICT_VALUES,
   DICT_ITEMS,
   DICT_POP,
   DICT_POPITEM,
   DICT_CLEAR,
   DICT_UPDATE,
   DICT_SETDEFAULT,
   DICT_COPY,
   DICT_FROMKEYS,

   // Set methods (modify in place)
   SET_ADD,
   SET_REMOVE,
   SET_DISCARD,
   SET_CLEAR,
   SET_UPDATE,
   SET_INTERSECTION_UPDATE,
   SET_DIFFERENCE_UPDATE,
   SET_SYMMETRIC_DIFFERENCE_UPDATE,

   // Set methods (return new set)
   SET_UNION,
   SET_INTERSECTION,
   SET_DIFFERENCE,
   SET_SYMMETRIC_DIFFERENCE,
   SET_COPY,
   SET_ISSUBSET,
   SET_ISSUPERSET,
   SET_ISDISJOINT,

   // Set methods (query)
   SET_HAS,

   // String methods (return new string)
   STR_UPPER,
   STR_LOWER,
   STR_CAPITALIZE,
   STR_TITLE,
   STR_SWAPCASE,
   STR_STRIP,
   STR_LSTRIP,
   STR_RSTRIP,
   STR_REPLACE,
   STR_SPLIT,
   STR_RSPLIT,
   STR_JOIN,
   STR_SPLITLINES,
   STR_PARTITION,
   STR_RPARTITION,
   STR_CENTER,
   STR_LJUST,
   STR_RJUST,
   STR_ZFILL,
   STR_EXPANDTABS,
   STR_TRANSLATE,
   STR_SLICE,
   STR_REVERSE,
   STR_REPEAT,

   // String methods (query)
   STR_STARTSWITH,
   STR_ENDSWITH,
   STR_FIND,
   STR_RFIND,
   STR_INDEX,
   STR_RINDEX,
   STR_COUNT,
   STR_ISALPHA,
   STR_ISDIGIT,
   STR_ISALNUM,
   STR_ISSPACE,
   STR_ISUPPER,
   STR_ISLOWER,
   STR_ISTITLE,
   STR_ISDECIMAL,
   STR_ISNUMERIC,
   STR_ISIDENTIFIER,
   STR_ISPRINTABLE,
   STR_ISASCII,

   // Integer/Float methods
   NUM_BIT_LENGTH,
   NUM_BIT_COUNT,
   NUM_TO_BYTES,
   NUM_FROM_BYTES,
   NUM_AS_INTEGER_RATIO,
   NUM_IS_INTEGER,
   NUM_HEX,

   // Range methods
   RANGE_START,
   RANGE_STOP,
   RANGE_STEP,
   RANGE_INDEX,
   RANGE_COUNT_METHOD

} BuiltinType;

//=========================================================
//  Builtin Information Structure
//=========================================================

typedef struct
{
   BuiltinType type;
   const char *name;
   int min_args;
   int max_args; // -1 for variadic
} BuiltinInfo;

//=========================================================
//  Builtin Registry Structure
//=========================================================

typedef struct
{
   const BuiltinInfo *items;
   size_t count;
} BuiltinRegistry;

//=========================================================
//  Complete Registry Declarations
//=========================================================

// Core builtins
const BuiltinInfo BUILTIN_REGISTRY[] = {
    {BUILTIN_PRINT, "print", 0, -1},
    {BUILTIN_INPUT, "input", 0, 1},
    {BUILTIN_TYPE, "type", 1, 1},
    {BUILTIN_INT, "int", 1, 2},
    {BUILTIN_FLOAT, "float", 1, 1},
    {BUILTIN_STR, "str", 1, 1},
    {BUILTIN_BOOL, "bool", 1, 1},
    {BUILTIN_LIST, "list", 0, 1},
    {BUILTIN_DICT, "dict", 0, 1},
    {BUILTIN_SET, "set", 0, 1},
    {BUILTIN_RANGE, "range", 1, 3},
    {BUILTIN_LEN, "len", 1, 1},
    {BUILTIN_ABS, "abs", 1, 1},
    {BUILTIN_MIN, "min", 1, -1},
    {BUILTIN_MAX, "max", 1, -1},
    {BUILTIN_SUM, "sum", 1, 2},
    {BUILTIN_POW, "pow", 2, 3},
    {BUILTIN_ROUND, "round", 1, 2},
    {BUILTIN_SORTED, "sorted", 1, 2},
    {BUILTIN_REVERSED, "reversed", 1, 1},
};

// Type-specific method registries
// List Methods
const BuiltinInfo LIST_REGISTRY[] = {
    {LIST_APPEND, "append", 1, 1},
    {LIST_EXTEND, "extend", 1, 1},
    {LIST_INSERT, "insert", 2, 2},
    {LIST_REMOVE, "remove", 1, 1},
    {LIST_POP, "pop", 0, 1},
    {LIST_CLEAR, "clear", 0, 0},
    {LIST_INDEX, "index", 1, 3},
    {LIST_REVERSE, "reverse", 0, 0},
    {LIST_COPY, "copy", 0, 0},
};

// Dict Methods
const BuiltinInfo DICT_REGISTRY[] = {
    {DICT_GET, "get", 1, 2},
    {DICT_KEYS, "keys", 0, 0},
    {DICT_VALUES, "values", 0, 0},
    {DICT_ITEMS, "items", 0, 0},
    {DICT_POP, "pop", 1, 2},
    {DICT_POPITEM, "popitem", 0, 0},
    {DICT_CLEAR, "clear", 0, 0},
    {DICT_UPDATE, "update", 1, 1},
    {DICT_SETDEFAULT, "setdefault", 1, 2},
    {DICT_COPY, "copy", 0, 0},
};

// Set Methods
const BuiltinInfo SET_REGISTRY[] = {
    {SET_ADD, "add", 1, 1},
    {SET_REMOVE, "remove", 1, 1},
    {SET_DISCARD, "discard", 1, 1},
    {SET_CLEAR, "clear", 0, 0},
    {SET_UNION, "union", 1, -1},
    {SET_INTERSECTION, "intersection", 1, -1},
    {SET_DIFFERENCE, "difference", 1, -1},
    {SET_SYMMETRIC_DIFFERENCE, "symmetric_difference", 1, 1},
    {SET_COPY, "copy", 0, 0},
    {SET_HAS, "has", 1, 1},
};

// String Methods
const BuiltinInfo STR_REGISTRY[] = {
    {STR_UPPER, "upper", 0, 0},
    {STR_LOWER, "lower", 0, 0},
    {STR_CAPITALIZE, "capitalize", 0, 0},
    {STR_TITLE, "title", 0, 0},
    {STR_STRIP, "strip", 0, 1},
    {STR_LSTRIP, "lstrip", 0, 1},
    {STR_RSTRIP, "rstrip", 0, 1},
    {STR_REPLACE, "replace", 2, 3},
    {STR_SPLIT, "split", 0, 2},
    {STR_JOIN, "join", 1, 1},
    {STR_STARTSWITH, "startswith", 1, 3},
    {STR_ENDSWITH, "endswith", 1, 3},
    {STR_FIND, "find", 1, 3},
    {STR_INDEX, "index", 1, 3},
    {STR_ISALPHA, "isalpha", 0, 0},
    {STR_ISDIGIT, "isdigit", 0, 0},
    {STR_ISALNUM, "isalnum", 0, 0},
    {STR_ISSPACE, "isspace", 0, 0},
    {STR_ISUPPER, "isupper", 0, 0},
    {STR_ISLOWER, "islower", 0, 0},
};

// Int/Bool Methods
const BuiltinInfo INT_REGISTRY[] = {
    {NUM_BIT_LENGTH, "bit_length", 0, 0},
    {NUM_BIT_COUNT, "bit_count", 0, 0},
};

// Float Methods
const BuiltinInfo FLOAT_REGISTRY[] = {
    {NUM_IS_INTEGER, "is_integer", 0, 0},
    {NUM_HEX, "hex", 0, 0},
};

// Range Methods
const BuiltinInfo RANGE_REGISTRY[] = {
    {RANGE_COUNT_METHOD, "count", 1, 1},
    {RANGE_INDEX, "index", 1, 1},
};

//=========================================================
//  Function Declarations
//=========================================================

// Initialization
void initializeBuiltins(Runtime *rt);
void initializeTypeMethods(Runtime *rt);

// Method lookup
Data *getBuiltinMethod(DataType type, Data *methodName, Runtime *rt);

// Builtin dispatchers
Data *dispatchBuiltin(BuiltinType type, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchListMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchDictMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchSetMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchStrMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchNumMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchRangeMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchNoneMethod(BuiltinType type, Data *object, List *args, Dict *kwargs, Runtime *rt);
#endif // BUILTIN_H