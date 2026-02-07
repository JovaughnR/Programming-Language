#ifndef SET_H
#define SET_H
#include "type.h"
// Function prototypes

void set_add(void *value, Set *set);
void set_remove(void *value, Set *set);
Set *set_union(Set *A, Set *B);
Set *set_intersection(Set *A, Set *B);
Set *set_difference(Set *A, Set *B);
Set *set_symdiff(Set *A, Set *B);
int sets_equal(Set *a, Set *b, int (*cmp)(const void *, const void *));
void set_clear(Set *set);
int set_has(void *value, Set *set);
int set_len(Set *set);
Set *set_clone(Set *original);
#endif // SET_H