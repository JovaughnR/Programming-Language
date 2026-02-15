#ifndef SET_H
#define SET_H
#include "type.h"
// Function prototypes

typedef int (*Compare)(const void *, const void *);
typedef void *(*Clone)(const void *);

typedef struct Set
{
   int size;
   int capacity;
   void **bucket;
} Set;

/**
 * Function: createSet
 * Allocates and initializes a new Set.
 *
 * Returns:
 * - A pointer to the newly created Set.
 */
Set *set_create();
void set_add(void *value, Set *set);
void set_remove(void *value, Set *set);
Set *set_union(Set *A, Set *B);
Set *set_intersection(Set *A, Set *B);
Set *set_difference(Set *A, Set *B);
Set *set_symdiff(Set *A, Set *B);
int set_equal(Set *a, Set *b);
void set_clear(Set *set);
int set_has(void *value, Set *set);
int set_len(Set *set);
Set *set_clone(Set *original);
void set_free(Set *set);
#endif // SET_H,