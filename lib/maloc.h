/*
 * maloc.c - Memory allocation and object creation module
 * Provides factory functions for creating and initializing all AST nodes,
 * data structures, and runtime environment objects for the custom programming language.
 *
 * This module handles:
 * - Data object creation with type-specific initialization
 * - Abstract Syntax Tree (AST) node creation and composition
 * - Runtime environment and method dictionary setup
 * - Function, loop, and control flow structure creation
 * - Import and assignment structure initialization
 */

#ifndef ALLOC_H
#define ALLOC_H

#include "type.h"

// macros
#define __LEN__ 30

/**
 * data_create - Create a Data object with type-specific initialization
 * @param type The DataType enum value
 * @param value Pointer to the value to store
 *
 * Allocates and initializes a Data structure based on type:
 * - Primitive types (int, bool, float) are deep-copied
 * - Strings are duplicated using strdup()
 * - Reference types (list, dict, function, etc.) are wrapped in Object pointers
 * - Operators and indexed accesses are handled specially
 *
 * Returns: Pointer to new Data structure, or NULL on allocation failure
 */
Data *createData(DataType type, void *value);

/**
 * indexed_create - Create an indexed access structure (for array/dict access)
 * @param var The variable being indexed
 * @param value The index expression
 *
 * Creates a structure representing array/dictionary indexing operations.
 * Example: arr[index] or dict[key]
 *
 * Returns: Pointer to new Indexed structure, or NULL on allocation failure
 */
Indexed *createIndexed(ASTnode *object, void *value);

/**
 * env_create - Create a new execution environment/scope
 * @parent: Parent environment (for scope chaining), or NULL for global scope
 *
 * Allocates a new environment with local variable dictionary and reference tracking.
 * Creates separate dictionaries for global and nonlocal variable declarations.
 * Increments parent's reference count if parent is not NULL.
 *
 * Returns: Pointer to new Environment, or NULL on allocation failure
 */
Environment *env_create(Environment *parent);

/**
 * runtime_create - Create and initialize the runtime context
 *
 * Sets up the complete runtime environment including:
 * - Global execution environment
 * - Method dictionaries for all types
 * - Module system (initialized to NULL)
 *
 * Returns: Pointer to new Runtime structure, or NULL on allocation failure
 */
Runtime *createRuntime(void);

/**
 * attribute_create - Create an attribute access structure
 * @param object The object being accessed
 * @param attribute The attribute name (string)
 *
 * Creates a structure representing attribute access operations.
 * Example: obj.attribute
 *
 * Returns: Pointer to new Attribute structure, or NULL on allocation failure
 */
Attribute *createAttribute(ASTnode *object, char *attribute);

/**
 * ast_create - Create a basic AST node containing data
 * @param value: The Data object to store in this node
 *
 * Creates a leaf node in the abstract syntax tree.
 * Left and right child pointers are initialized to NULL.
 *
 * Returns: Pointer to new ASTnode, or NULL on allocation failure
 */
ASTnode *createASTnode(Data *value);

/**
 * ast_create_op - Create an AST node for an operator
 * @param op: The Operator enum value
 *
 * Convenience function that creates a Data object from an operator
 * and wraps it in an AST node.
 *
 * Returns: Pointer to new ASTnode containing operator, or NULL on failure
 */
ASTnode *createASTop(Operator op);

/**
 * ast_create_expr - Build an expression tree by connecting AST nodes
 * @param root: The operator node (root of expression)
 * @param left: The left operand (subtree)
 * @param right: The right operand (subtree)
 *
 * Connects a root operator node to its left and right operands,
 * creating a binary expression tree.
 *
 * Returns: Pointer to root node (same as input root)
 */
ASTnode *createASTexpr(ASTnode *root, ASTnode *left, ASTnode *right);

/**
 * pair_create - Create a key-value pair for hash tables
 * @param key: The key (typically a string)
 * @param value: The value associated with the key
 *
 * Creates a linked-list node for hash table collision handling.
 * The next pointer is initialized to NULL.
 *
 * Returns: Pointer to new Pair, or NULL on allocation failure
 */
struct Pair *createPair(void *key, void *value);

/**
 * params_create - Create a function parameter descriptor
 * @param name: The parameter name
 * @param defaultValue: The default value expression, or NULL if required parameter
 *
 * Creates metadata for a function parameter, including whether it has a default value.
 * Default values are stored as AST nodes and evaluated at function definition time.
 *
 * Returns: Pointer to new ParamInfo, or NULL on allocation failure
 */
ParamInfo *createParams(char *name, ASTnode *defaultValue);

/**
 * function_create - Create a Function object (NOTE: name appears to be "function_free" but implements creation)
 * @param name: The function name
 * @param params: List of ParamInfo structures
 * @param body: List of Statements forming the function body
 *
 * Creates a function definition structure.
 * The environment (env) and locals will be set later during function definition.
 *
 * Returns: Pointer to new Function, or NULL on allocation failure
 */
Function *createFunction(char *name, struct List *params, struct List *body);

/**
 * invoked_create - Create a function invocation/call structure
 * @param postfix: The function expression (can be a variable, attribute, or other postfix expression)
 * @param args: List of positional arguments
 * @param kwargs: List of keyword arguments (named parameters)
 *
 * Creates a structure representing a function call.
 * The postfix expression is evaluated at runtime to determine what's being called.
 *
 * Returns: Pointer to new Invoked structure, or NULL on allocation failure
 */
Invoked *createInvoked(ASTnode *postfix, struct List *args, struct List *kwargs);

/**
 * range_create - Create a range object for iteration
 * @param start: Starting value (inclusive)
 * @param stop: Ending value (exclusive)
 * @param step: Step size (must be non-zero)
 *
 * Creates a range structure used for for-loop iteration.
 * Step cannot be zero; an error is thrown if it is.
 *
 * Returns: Pointer to new Range, or NULL on allocation failure (or if step is 0)
 */
Range *createRange(int start, int stop, int step);

/**
 * for_create - Create a for-loop structure
 * @param iterator: The loop variable name
 * @param iterable: Expression yielding the sequence to iterate over
 * @param body: List of Statements forming the loop body
 *
 * Creates a for-loop definition.
 * The iterator variable is created in the loop's local scope.
 *
 * Returns: Pointer to new ForLoop, or NULL on allocation failure
 */
ForLoop *createFor(char *iterator, ASTnode *iterable, struct List *body);

/**
 * statment_create - Create a statement wrapper
 * @param type: The StatementType enum value (e.g., STMT_EXPRESSION, STMT_RETURN, etc.)
 * @param data: The statement-specific data
 * @param lineno: Source code line number for error reporting
 *
 * Wraps various statement types in a uniform structure.
 * Statements are not executed at creation; they're stored for later evaluation.
 *
 * Returns: Pointer to new Statement, or NULL on allocation failure
 */
Statement *createStatement(StatementType type, void *data, int lineno);

/**
 * flow_create - Create a conditional flow control structure
 * @param cond: The condition expression (NULL for else blocks)
 * @param body: List of Statements in the main block
 * @param elif: Pointer to next Flow structure for elif chain (NULL if none)
 * @param else_blck: Pointer to else block Flow structure (NULL if none)
 *
 * Creates if/elif/else control flow structures.
 * Condition is NULL for else blocks.
 * elif chains are created by linking Flow structures.
 *
 * Returns: Pointer to new Flow, or NULL on allocation failure
 */
Flow *createFlow(ASTnode *cond, struct List *body, Flow *elif, Flow *else_blck);

/**
 * while_create - Create a while-loop structure
 * @param condition: Expression that must remain truthy to continue looping
 * @param body: List of Statements forming the loop body
 *
 * Creates a while-loop definition with a condition and body.
 * The condition is re-evaluated before each iteration.
 *
 * Returns: Pointer to new WhileLoop, or NULL on allocation failure
 */
WhileLoop *createWhile(ASTnode *condition, struct List *body);

/**
 * import_create - Create an import statement structure
 * @param module: Data object containing module name
 * @param items: List of specific items to import (NULL if importing whole module)
 * @param alias: Alternative name for the module/items (NULL if no alias)
 * @param all: Boolean flag indicating "import *" (import everything)
 *
 * Creates an import statement for loading modules and their contents.
 * Supports: import module, from module import item, import module as alias, etc.
 *
 * Returns: Pointer to new Import, or NULL on allocation failure
 */

Import *createImport(Data *module, struct List *items, Data *alias, int all);
/**
 * assignment_create - Create an assignment statement structure
 * @param vars: List of variables being assigned to
 * @param values: List of value expressions being assigned
 * @param op: Optional operator for compound assignment (+=, -=, etc.), or NULL for simple assignment
 *
 * Creates an assignment structure supporting:
 * - Simple assignment: x = 5
 * - Multiple assignment: a, b = 1, 2
 * - Compound assignment: x += 10
 *
 * Returns: Pointer to new Assignment, or NULL on allocation failure
 */
Assignment *createAssignment(struct List *vars, struct List *values, ASTnode *op);

ASTnode *createReturn(ASTnode *astNode, int funcDepth);

void *createJump(int loopDepth, int jump);

Class *createClass(Data *var, struct List *statements, struct List *parents);

Instance *createInstance(Class *class);

Slice *createSlice(ASTnode *start, ASTnode *stop, ASTnode *step);

struct Catch *createCatch(char *errorName, char *alias, struct List *statements);

struct Exception *createException(struct List *try, struct Catch *error, struct List *finally);
#endif
