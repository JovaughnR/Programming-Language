#ifndef METHOD_H
#define METHOD_H

#include "type.h"

//=========================================================
//  I/O Functions
//=========================================================

// Print values to stdout
Data *builtin_print(List *args, Dict *kwargs);

// Read line from stdin
Data *builtin_input(const char *prompt);

//=========================================================
//  Type Checking and Conversion
//=========================================================

// Get type name as string
Data *builtin_type(Data *obj);

// Convert to integer
Data *builtin_int(Data *obj);

// Convert to float
Data *builtin_float(Data *obj);

// Convert to string
Data *builtin_str(Data *obj);

// Convert to boolean
Data *builtin_bool(Data *obj);

//=========================================================
//  Collection Functions
//=========================================================

// Get length of collection
Data *builtin_len(Data *obj);

// Append item to list

// Remove and return item at index

// Insert item at index

// Get dictionary keys

// Get dictionary values

//=========================================================
//  Mathematical Functions
//=========================================================

// Absolute value
Data *builtin_abs(Data *obj);

// Minimum value
// Maximum value

// Power function
Data *builtin_pow(Data *base, Data *exp);

// Round to precision
Data *builtin_round(Data *obj, int ndigits);

// Split string into list
Data *builtin_split(Data *str_data, const char *sep);

// Join list into string
Data *builtin_join(Data *list_data, const char *sep);

//=========================================================
//  Utility Functions
//=========================================================

// Return reversed copy of list

Data *builtin_sorted(Data *list_data);

Data *builtin_list(Data *data);
Data *builtin_range(Data *arg1, Data *arg2, Data *arg3);
Data *builtin_set(Data *data);
Data *builtin_dict(Data *data);

#endif // METHOD_H