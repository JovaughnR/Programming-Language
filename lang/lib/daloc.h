#ifndef DALLOC_H
#define DALLOC_H

#include "type.h"

/* Function prototypes */
void pair_free(const void *pair);

void data_free(const void *d);

void methods_free(Method *methods);

void ast_free(const void *ast);

void statement_free(const void *statement);

void env_free(Environment *env);

void runtime_free(Runtime *rt);
#endif // DALLOC_H