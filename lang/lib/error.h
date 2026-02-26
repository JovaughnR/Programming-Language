#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <setjmp.h>
#include "type.h"

// ANSI color codes
#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD "\033[1m"

// Error type enumeration
typedef enum
{
   ERROR_NONE,
   ERROR_SYNTAX,
   ERROR_NAME,
   ERROR_TYPE,
   ERROR_VALUE,
   ERROR_ATTRIBUTE,
   ERROR_KEY,
   ERROR_INDEX,
   ERROR_MEMORY,
   ERROR_ZERO_DIVISION,
   ERROR_RUNTIME,
   ERROR_IMPORT,
   ERROR_IO,
   ERROR_ASSERTION,
   ERROR_ALL
} ErrorType;

// Represents a single catch clause:
//   except ValueError as e:
//       <statements>
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

// Thrown error state — set by throw_error, read by the interpreter
typedef struct ThrownError
{
   ErrorType type;
   char message[1024]; // formatted message
   int active;         // 1 if in try block
   int error;          // 1 if an error occured
   int lineno;         // line where error occured
} ThrownError;

// Global thrown error state — checked by the interpreter after each statement
extern ThrownError g_thrown;
extern int repl_mode;

typedef struct ErrorEntry
{
   char *name;
   ErrorType type;
} ErrorEntry;

ErrorType get_error_type(const char *name);

void throw_error(ErrorType type, const char *format, ...);
void throw_warning(const char *format, ...);

// Called by the interpreter to handle an Exception AST node at runtime
// Returns 1 if the error was handled, 0 if it should propagate
Status handleException(Exception *except, void **returns, Runtime *rt);

// Clear the thrown error state (called after a successful catch)
void clear_error(void);

#endif // ERROR_H