#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#define TOMBSTONE (Instance *)1 // Define a special value for marking deleted slots in the hash table.

#define PARSE_SIZE 10     // Initialize to a small amount for parsing
#define RUNTIME_SIZE 1000 // Initialize to large amount for during runtime

typedef struct Dict Dict;
typedef struct List List;
typedef struct Set Set;

#define IS_LIST(inst) ((inst) && (inst)->type == TYPE_LIST)
#define IS_DICT(inst) ((inst) && (inst)->type == TYPE_DICT)
#define IS_SET(inst) ((inst) && (inst)->type == TYPE_SET)
#define IS_FUNCTION(inst) ((inst) && (inst)->type == TYPE_FUNCTION)
#define IS_CLASS(inst) ((inst) && (inst)->type == TYPE_CLASS)

#define LIST_PTR(inst) (IS_LIST(inst) ? (List *)(inst)->ref->object : NULL)
#define DICT_PTR(inst) (IS_DICT(inst) ? (Dict *)(inst)->ref->object : NULL)
#define SET_PTR(inst) (IS_SET(inst) ? (Set *)(inst)->ref->object : NULL)
#define FUNCTION_PTR(d) (IS_FUNCTION(d) ? (Function *)(d)->ref->object : NULL)
#define CLASS_PTR(inst) (IS_CLASS(inst) ? (Class *)(inst)->ref->object : NULL)

typedef enum
{
   TYPE_INT,
   TYPE_FLOAT,
   TYPE_BOOL,
   TYPE_STR,
   TYPE_FUNCTION,
   TYPE_LIST,
   TYPE_DICT,
   TYPE_RANGE,
   TYPE_SET,
   TYPE_CLASS,
   TYPE_NONE,
} DataType;

typedef enum
{
   TYPE_INSTANCE,
   TYPE_INVOKED,
   TYPE_OPERATOR,
   TYPE_LOOKUP,
   TYPE_ATTRIBUTE,
   TYPE_INDEX,
   TYPE_BUILTIN,
} HelperType;

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
   HelperType type;
   union
   {
      struct Operator *op;
      struct Invoked *invoked;
      struct Instance *instance;
      struct Indexed *index;
      struct Attribute *attribute;
      void *builtin;
      struct Data *var;
      void *any;
   };
} Data;

typedef struct Instance
{
   DataType type;
   union
   {
      int *integer;
      double *decimal;
      char *string;
      struct Reference *ref; // referene types (class, list, dict, set, function, class)
      struct Range *range;
      struct Instance *inst;
      void *none;
   };
   struct Dict *attributes;
} Instance;

typedef struct Set
{
   int size;
   int capacity;
   Instance **bucket;
} Set;

typedef struct Reference
{
   void *object;   // Pointer to the actual object (List*, Dict*, Set*, Function*)
   int references; // Number of references
   int isEvaluated;
} Reference;

typedef struct Range
{
   int start;
   int stop;
   int step;
} Range;

typedef struct Class
{
   char *name;              // Class name
   struct Runtime *rt;      // Class environment (contains methods and attributes)
   struct List *parents;    // List of parent classes
   struct List *statements; // List of methods
   struct List *mro;
   int isInitialize;
} Class;

typedef struct Indexed
{
   Data *variable;
   struct ASTNode *value;
} Indexed;

typedef struct Attribute
{
   struct ASTNode *object;    // The object (left side of the dot)
   struct ASTNode *attribute; // The attribute name (right side of the dot)
} Attribute;

typedef struct ASTnode
{
   Data *data;
   struct ASTnode *left;
   struct ASTnode *right;
} ASTnode;

typedef struct Function
{
   char *name;
   struct List *params;
   struct List *body;
   struct Env *env;
   struct Dict *locals;
} Function;

typedef enum
{
   BUILTIN_PRINT,
   BUILTIN_INPUT,
   BUILTIN_TYPE,
   BUILTIN_INT,
   BUILTIN_FLOAT,
   BUILTIN_STR,
   BUILTIN_BOOL,
   BUILTIN_LEN,

   BUILTIN_REMOVE,
   BUILTIN_KEYS,
   BUILTIN_VALUES,
   BUILTIN_ABS,
   BUILTIN_MIN,
   BUILTIN_MAX,
   BUILTIN_DICT,
   BUILTIN_SET,
   BUILTIN_LIST,
   BUILTIN_RANGE,
   BUILTIN_SUM,
   BUILTIN_POW,
   BUILTIN_ROUND,
   BUILTIN_UPPER,
   BUILTIN_LOWER,
   BUILTIN_SPLIT,
   BUILTIN_JOIN,
   BUILTIN_SORTED,
   BUILTIN_REVERSED,
   BUILTIN_CLEAR,
   BUILTIN_EXTEND,

   // LIST METHOD TYPES (MODIFIES LIST IN PLACE)
   LIST_REVERSE,
   LIST_EXTEND,
   LIST_SLICE,
   LIST_INDEX,
   LIST_CLEAR,
   LIST_GET,
   LIST_REMOVE,
   LIST_POP,
   LIST_INSERT

} BuiltinType;

typedef struct
{
   BuiltinType type;
   const char *name;
   int min_args;
   int max_args; // -1 for variadic
} BuiltinInfo;

typedef struct Environment
{
   Dict *vars;
   struct Env *parent;
   int ref;
   Dict *global_vars;
   Dict *nonlocal_vars;
   Dict *builtins;
} Environment;

typedef struct
{
   void *modules; // List of imported module environments
   struct Env *env;
   struct Dict *builtins;   // GLOBAL BUILTIN METHODS
   struct Dict *list_dir;   // LIST ATTRIBUTES
   struct Dict *set_dir;    // SET ATTRIBUTES
   struct Dict *dict_dir;   // DICT ATTRIBUTES
   struct Dict *int_dir;    // INT ATTRIBUTES
   struct Dict *float_dir;  // FLOAT ATTRIBUTES
   struct Dict *str_dir;    // STRING ATTRIBUTES
   struct Arena *tempArena; // NEW: Temporary allocations
   struct Arena *exprArena; // NEW: Expression evaluation
   int arenaDepth;          // NEW: Track nested scopes
} Runtime;

typedef Data *(*ExecFn)(ASTnode *, Runtime *);
// typedef Status (*ExecBody)(List *, void **, Runtime *);

const char *getDataType(DataType type);
char *dataTostring(Data *d);

#endif // CORES_TYPE_H