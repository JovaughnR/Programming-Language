//=========================================================
//  COMPLETE BUILTIN REGISTRY SYSTEM
//=========================================================

#ifndef BUILTIN_H
#define BUILTIN_H

#include "type.h"
#include <stdlib.h>

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

// Initialization
void initializeBuiltins(Runtime *rt);

// Builtin dispatchers
Data *dispatchBuiltin(BuiltinType type, List *args, Dict *kwargs, Runtime *rt);
Data *dispatchMethod(Data *object, Data *method, List *args, Dict *kwargs, Runtime *rt);

#endif // BUILTIN_H