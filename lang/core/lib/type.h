#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <pthread.h>

#define TOMBSTONE (void *)1 // Define a special value for marking deleted slots in the hash table.

#define __len__ 8 // Initialize to a small amount for parsing
#define __size__ 8

#define CONSTRUCTOR_NAME "__struct__"

#define IS_LIST(d) ((d) && (d)->type == TYPE_LIST)
#define IS_DICT(d) ((d) && (d)->type == TYPE_DICT)
#define IS_SET(d) ((d) && (d)->type == TYPE_SET)
#define IS_FUNCTION(d) ((d) && (d)->type == TYPE_FUNCTION || (d)->type == TYPE_LAMBDA)
#define IS_CLASS(d) ((d) && (d)->type == TYPE_CLASS)
#define IS_INSTANCE(d) ((d) && (d)->type == TYPE_INSTANCE)

#define LIST_PTR(d) (IS_LIST(d) ? (List *)(d)->ref->object : NULL)
#define DICT_PTR(d) (IS_DICT(d) ? (Dict *)(d)->ref->object : NULL)
#define SET_PTR(d) (IS_SET(d) ? (Set *)(d)->ref->object : NULL)
#define FUNC_PTR(d) (IS_FUNCTION(d) ? (Function *)(d)->ref->object : NULL)
#define CLASS_PTR(d) (IS_CLASS(d) ? (Class *)(d)->ref->object : NULL)
#define INST_PTR(d) (IS_INSTANCE(d) ? (Instance *)(d)->ref->object : NULL)
#define INST_CLASS(inst) (CLASS_PTR((inst)->class))

typedef enum
{
   TYPE_INT,
   TYPE_FLOAT,
   TYPE_BOOL,
   TYPE_STR,
   TYPE_FUNCTION,
   TYPE_INSTANCE,
   TYPE_LIST,
   TYPE_DICT,
   TYPE_RANGE,
   TYPE_SET,
   TYPE_CLASS,
   TYPE_NONE,
   TYPE_FILE,
   TYPE_DB,
   TYPE_STMT,
   TYPE_THREAD,
   TYPE_MUTEX,
   TYPE_COND,
   TYPE_AWAIT,
   TYPE_GENERATOR,

   // Helper Types
   TYPE_FUTURE,
   TYPE_HTTP_SERVER,
   TYPE_INVOKED,
   TYPE_OPERATOR,
   TYPE_LOOKUP,
   TYPE_ATTRIBUTE,
   TYPE_INDEX,
   TYPE_ENUM,
   TYPE_BUILTIN,
   TYPE_MODULE,
   TYPE_FSTRING,
   TYPE_VAR,
   TYPE_SLICE,
   TYPE_LAMBDA,
   TYPE_TERNARY,
   TYPE_RUNTIME,
   TYPE_LIST_EXPR,
   TYPE_DICT_EXPR,
   TYPE_SET_EXPR,
} DataType;

typedef enum
{
   ADD,
   SUB,
   DIV,
   MUL,
   MOD,
   FLDIV,
   EXP,
   XOR,
   BIT_OR,
   BIT_AND,
   BIT_NOT,
   AND,
   OR,
   EQUAL,
   NEQ,
   LEQ,
   GEQ,
   LST,
   GRT,
   NOT,
   INTO,
   RIGHT_SHIFT,
   LEFT_SHIFT
} Operator;

/**
 * @struct Data
 * @brief A boxed runtime value that stores any language object.
 *
 * @param type  The data type of value stored.
 * @param value   Pointer to the actual data (ASTNode*, char*, Function*, etc).
 */
typedef struct Data
{
   DataType type;
   union
   {
      int *atom;
      double *real;
      char *str;
      void *any;

      struct Range *range;
      struct Object *ref;
   };
} Data;

typedef struct Object
{
   void *object;   // Pointer to the actual object (List*, Dict*, Set*, Function*)
   int references; // Number of references
   int isEvaluated;
} Object;

typedef struct Range
{
   int start, stop, step;
   struct Dict *methods;
} Range;

typedef struct Indexed
{
   struct ASTnode *object;
   struct ASTnode *value;
} Indexed;

typedef struct Invoked
{
   struct ASTnode *postfix;
   struct List *args;
   struct List *kwargs;
} Invoked;

typedef struct Attribute
{
   struct ASTnode *object; // The object (left side of the dot)
   struct Data *attrib;    // The attribute name (right side of the dot)
} Attribute;

typedef struct Instance
{
   struct Data *class;
   struct Dict *attributes;
} Instance;

typedef struct Slice
{
   struct ASTnode *start;
   struct ASTnode *stop;
   struct ASTnode *step;
} Slice;

typedef struct ASTnode
{
   Data *data;
   struct ASTnode *left;
   struct ASTnode *right;
} ASTnode;

typedef struct Ternary
{
   struct ASTnode *condition;
   struct ASTnode *trueExpr;
   struct ASTnode *falseExpr;
} Ternary;

typedef struct Future
{
   int id;
   Data *result;
   int done;
   int error;
   char error_msg[1024];
   pthread_mutex_t lock;
   pthread_cond_t ready;
} Future;

typedef struct Function
{
   Data *name;
   struct List *params;
   struct List *body;
   struct Env *env;
   struct Dict *locals;
   int isAsync;
   struct Chunk *chunk;
   struct VMUpvalue **upvalues;
   int upvalue_count;
} Function;

typedef struct Await
{
   struct ASTnode *expr;
} Await;

typedef struct Env
{
   struct Dict *vars;
   struct Env *parent;
   int ref;
   struct Dict *global_vars;
   struct Dict *nonlocal_vars;
} Env;

typedef struct Method
{
   struct Dict *builtin; // GLOBAL BUILTIN METHODS
   struct Dict *list;    // LIST ATTRIBUTES
   struct Dict *set;     // SET ATTRIBUTES
   struct Dict *dict;    // DICT ATTRIBUTES
   struct Dict *atom;    // INT ATTRIBUTES
   struct Dict *real;    // FLOAT ATTRIBUTES
   struct Dict *str;     // STRING ATTRIBUTES
   struct Dict *range;
   struct Dict *none;
} Method;

typedef struct Runtime
{
   struct Env *env;
   struct Method *methods;
   struct ModuleRegistry *registry; // ← only on root runtime
   struct Runtime *parent;          // ← NEW: link to parent runtime

   struct Arena *tempArena;
   struct Arena *exprArena;
   int arenaDepth;
} Runtime;

typedef struct Class
{
   Data *name;              // Class name
   struct List *parents;    // List of parent classes
   struct List *statements; // List of methods
   struct List *mro;
   Runtime *rt;
   int isInitialize;
} Class;

typedef enum
{
   FLOW_NORMAL,   // Continue normal execution
   FLOW_RETURN,   // Return from function
   FLOW_BREAK,    // Break from loop
   FLOW_CONTINUE, // Continue to next iteration
   FLOW_TAKEN,
   FLOW_EXCEPTION
} Status;

typedef struct Flow
{
   struct ASTnode *condition;
   struct List *body;

   struct Flow *elif_chain;
   struct Flow *else_block;
} Flow;

typedef struct WhileLoop
{
   struct ASTnode *condition;
   struct List *body;
} WhileLoop;

typedef struct ForLoop
{
   Data *iterator;
   struct ASTnode *iterable;
   struct List *body;
} ForLoop;

typedef struct ParamInfo
{
   Data *name;
   ASTnode *defaultValue;
   int hasDefault;
   int isVariadic;
} ParamInfo;

typedef struct Import
{
   Data *module;       // Module name (e.g., "math" or "os.path")
   Data *alias;        // Alias name (for "import X as Y")
   struct List *items; // Specific items to import (NULL for "import module")
   int importAll;      // 1 for "from X import *", 0 otherwise
} Import;

typedef enum
{
   STMT_ASMT,
   STMT_FLOW,
   STMT_WHILE,
   STMT_FOR,
   STMT_RETURN,
   STMT_BREAK,
   STMT_CONTINUE,
   STMT_FUNC,
   STMT_EXPR,
   STMT_CLASS,
   STMT_GLOBAL,
   STMT_NONLOCAL,
   STMT_IMPORT,
   STMT_EXCEPTION,
   STMT_ENUM,
   STMT_ASYNC,
   STMT_THROW
} StatementType;

typedef struct Statement
{
   StatementType type;
   void *data;
   int lineno;
} Statement;

typedef struct
{
   struct List *vars;   // (x, x[2] x.size)
   struct List *values; // List of values to assign
   void *value;         // Right-hand side expression (can be a list of values)
   ASTnode *op;         // Operator for compound assignments (NULL if simple)
} Assignment;

typedef struct ModuleEntry
{
   const char *name;
   struct Runtime *rt; // NULL until first imported
   int isNative;
   void (*initializer)(struct Runtime *); // native init fn
} ModuleEntry;

typedef struct ModuleRegistry
{
   ModuleEntry *entries;
   int count;
   int capacity;
} ModuleRegistry;

typedef struct EnumItem
{
   char *name;
   int hasValue;
   struct ASTnode *value; // NULL if no explicit value
} EnumItem;

typedef struct Enum
{
   Data *name;
   struct List *items; // List of EnumItem*
} Enum;

typedef struct Catch
{
   struct Data *errorName;  // e.g. "ValueError" — NULL means catch-all
   struct Data *alias;      // e.g. "e"          — NULL if no 'as' clause
   struct List *statements; // body of the catch block
} Catch;

// Represents a full try/except/finally block
typedef struct Exception
{
   struct List *tried; // body of the try block
   struct Catch *catched;
   struct List *finally; // body of finally — NULL if absent
} Exception;

typedef struct Generator
{
   char *ident;
   struct ForLoop *loop;
   ASTnode *cond;
} Generator;

#endif // CORES_TYPE_H