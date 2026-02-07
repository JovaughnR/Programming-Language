// error.h
#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Error type enumeration
typedef enum
{
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
   ERROR_ASSERTION
} ErrorType;

/**
 * @brief Prints an error message and exits the program
 *
 * @param type The type of error
 * @param format Printf-style format string
 * @param ... Variable arguments for the format string
 *
 * Usage examples:
 *   throw_error(ERROR_NAME, "name '%s' is not defined", varName);
 *   throw_error(ERROR_TYPE, "unsupported operand type(s) for %s: '%s' and '%s'",
 *               op, type1, type2);
 *   throw_error(ERROR_INDEX, "list index out of range");
 */
void throw_error(ErrorType type, const char *format, ...);

/**
 * @brief Prints a warning message without exiting
 *
 * @param format Printf-style format string
 * @param ... Variable arguments for the format string
 */
void throw_warning(const char *format, ...);

#endif // ERROR_H