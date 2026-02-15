#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#define TOMBSTONE (void *)1 // Define a special value for marking deleted slots in the hash table.

#define __len__ 8 // Initialize to a small amount for parsing
#define __size__ 8

#define CONSTRUCTOR_NAME "__struct__"

#define IS_LIST(d) ((d) && (d)->type == TYPE_LIST)
#define IS_DICT(d) ((d) && (d)->type == TYPE_DICT)
#define IS_SET(d) ((d) && (d)->type == TYPE_SET)
#define IS_FUNCTION(d) ((d) && (d)->type == TYPE_FUNCTION)
#define IS_CLASS(d) ((d) && (d)->type == TYPE_CLASS)
#define IS_INSTANCE(d) ((d) && (d)->type == TYPE_INSTANCE)

#define LIST_PTR(d) (IS_LIST(d) ? (List *)(d)->ref->object : NULL)
#define DICT_PTR(d) (IS_DICT(d) ? (Dict *)(d)->ref->object : NULL)
#define SET_PTR(d) (IS_SET(d) ? (Set *)(d)->ref->object : NULL)
#define FUNCTION_PTR(d) (IS_FUNCTION(d) ? (Function *)(d)->ref->object : NULL)
#define CLASS_PTR(d) (IS_CLASS(d) ? (Class *)(d)->ref->object : NULL)
#define INSTANCE_PTR(d) (IS_INSTANCE(d) ? (Instance *)(d)->obj->object : NULL)

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

   // Helper Types
   TYPE_INVOKED,
   TYPE_OPERATOR,
   TYPE_LOOKUP,
   TYPE_ATTRIBUTE,
   TYPE_INDEX,
   TYPE_BUILTIN,
   TYPE_VAR,
   TYPE_SLICE,
   TYPE_RUNTIME
} DataType;

typedef enum
{
   PLUS,
   MINUS,
   DIVIDE,
   MULTIPLY,
   MODULO,
   FLOOR,
   EXPONENT,
   XOR,
   BIT_OR,
   BIT_AND,
   BIT_NOT,
   LOGICAL_AND,
   LOGICAL_OR,
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
   struct Class *class;
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

typedef struct Function
{
   Data *name;
   struct List *params;
   struct List *body;
   struct Environment *env;
   struct Dict *locals;
} Function;

typedef struct Environment
{
   struct Dict *vars;
   struct Environment *parent;
   int ref;
   struct Dict *global_vars;
   struct Dict *nonlocal_vars;
   struct Dict *builtins;
} Environment;

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
   void *modules; // List of imported module environments
   struct Environment *env;
   struct Method *methods;

   struct Arena *tempArena; // NEW: Temporary allocations
   struct Arena *exprArena; // NEW: Expression evaluation
   int arenaDepth;          // NEW: Track nested scopes
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
   FLOW_TAKEN
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
   struct ASTnode *defaultValue;
   int hasDefault;
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
   STMT_ASSIGNMENT,
   STMT_COMP_ASSIGN,
   STMT_FLOW,
   STMT_WHILE,
   STMT_FOR,
   STMT_RETURN,
   STMT_BREAK,
   STMT_CONTINUE,
   STMT_FUNCTION,
   STMT_EXPRESSION,
   STMT_CLASS,
   STMT_GLOBAL,
   STMT_NONLOCAL,
   STMT_IMPORT,
   STMT_EXCEPTION
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

#endif // CORES_TYPE_H