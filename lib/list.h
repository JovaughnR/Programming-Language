#ifndef LIST_H
#define LIST_H
#include "type.h"

#define LIST_SUCCESS 0
#define LIST_ERROR -1

typedef struct List
{
   void **items;
   int length;
   int end;
   int capacity;
} List;

/**
 * Creates a new list with an initial capacity of 10.
 *
 * @return A pointer to the newly created list, or NULL if memory allocation fails.
 */
List *list_create(int size);
/**
 * Pushes a new value onto the list.
 *
 * @param value The object pointer to push onto the list. The list takes ownership of the pointer.
 * @param list Pointer to the list.
 * @return LIST_SUCCESS if the value is successfully pushed, LIST_ERROR if resizing fails.
 */
int list_append(void *value, List *list);

/**
 * Retrieves the value at the top of the list without removing it.
 *
 * @param list Pointer to the list.
 * @return A pointer to the object at the top of the list, or NULL if the list is empty.
 */
void *list_top(List *list);

/**
 * Pops the top value from the list, removing it.
 *
 * @param list Pointer to the list.
 * @return A pointer to the object at the top of the list, or NULL if the list is empty.
 */
void *list_pop(List *list);
/**
 * Inserts a value at a specific index in the list (like Python list.insert()).
 * Elements from the index onward are shifted right.
 *
 * @param list Pointer to the list.
 * @param index Position to insert the new value (0 = front).
 *              If index > list->length, the value is appended.
 * @param value Pointer to the object to insert.
 *
 * @return LIST_SUCCESS on success, LIST_ERROR if memory resizing fails or list is NULL.
 */
int list_insert(List *list, int index, void *value);

/**
 * Removes the first occurrence of an element in the list (like Python list.remove()).
 *
 * @param list Pointer to the list.
 * @param value Pointer to the value to remove (pointer comparison).
 *
 * @return LIST_SUCCESS if removed, LIST_ERROR if not found or list is NULL.
 */
int list_remove(void *value, List *list);
/**
 * Retrieves the value at a given index (like Python list[index]).
 *
 * @param list Pointer to the list.
 * @param index Index to retrieve (0-based).
 *
 * @return Pointer to the value, or NULL if index is out of range.
 */
void *list_get(int index, List *list);
/**
 * Replaces the value at a given index (like Python list[index] = value).
 *
 * @param list Pointer to the list.
 * @param index Index to replace.
 * @param value New value to assign.
 *
 * @return LIST_SUCCESS if replaced, LIST_ERROR if index is invalid or list is NULL.
 */
int list_set(int index, void *value, List *list);
/**
 * Returns the number of elements in the list (like Python len(list)).
 *
 * @param list Pointer to the list.
 *
 * @return Number of elements in the list, or 0 if list is NULL.
 */
int list_length(List *list);
/**
 * Clears all elements from the list (like Python list.clear()).
 *
 * @param list Pointer to the list.
 *
 * Frees all allocated items but does not deallocate the list itself.
 */
void list_clear(List *list);
/**
 * Returns the index of a value in the list (like Python list.index()).
 *
 * @param list Pointer to the list.
 * @param value Value to search for (pointer comparison).
 *
 * @return Index of the element, or -1 if not found.
 */
int list_index(void *value, List *list);
/**
 * Checks whether a value exists in the List .
 *
 * @param list Pointer to the list.
 * @param value Value to search for.
 *
 * @return 1 if found, 0 otherwise.
 */
int list_contains(void *value, List *list);

List *list_concat(List *a, List *b);

/**
 * Repeats a list multiple times (like Python list * n).
 *
 * @param list Pointer to the list.
 * @param times Number of repetitions.
 * @return Pointer to a new list containing the repeated elements.
 *         Caller must free the returned list.
 */
List *list_repeat(int times, List *list);

/**
 * Compares two lists for equality (like Python list1 == list2).
 *
 * @param a Pointer to the first list.
 * @param b Pointer to the second list.
 * @param cmp Comparator function to compare elements. Should return 0 if equal.
 * @return 1 if equal, 0 otherwise.
 */
int lists_equal(List *a, List *b, int (*cmp)(const void *, const void *));
/**
 * Compares two lists for inequality (like Python list1 != list2).
 *
 * @param a Pointer to the first list.
 * @param b Pointer to the second list.
 * @param cmp Comparator function to compare elements. Should return 0 if equal.
 * @return 1 if not equal, 0 if equal.
 */
int list_not_equal(List *a, List *b, int (*cmp)(const void *, const void *));
/**
 * Lexicographically compares two lists (like Python list1 < list2).
 *
 * @param a Pointer to the first list.
 * @param b Pointer to the second list.
 * @param cmp Comparator function for elements.
 * @return 1 if a < b, 0 otherwise.
 */
int list_less(List *a, List *b, int (*cmp)(const void *, const void *));
/**
 * Lexicographically compares two lists (like Python list1 <= list2).
 *
 * @param a Pointer to the first list.
 * @param b Pointer to the second list.
 * @param cmp Comparator function for elements.
 * @return 1 if a <= b, 0 otherwise.
 */
int list_less_equal(List *a, List *b, int (*cmp)(const void *, const void *));

/**
 * Lexicographically compares two lists (like Python list1 > list2).
 *
 * @param a Pointer to the first list.
 * @param b Pointer to the second list.
 * @param cmp Comparator function for elements.
 * @return 1 if a > b, 0 otherwise.
 */
int list_greater(List *a, List *b, int (*cmp)(const void *, const void *));
/**
 * Lexicographically compares two lists (like Python list1 >= list2).
 *
 * @param a Pointer to the first list.
 * @param b Pointer to the second list.
 * @param cmp Comparator function for elements.
 * @return 1 if a >= b, 0 otherwise.
 */
int list_greater_equal(List *a, List *b, int (*cmp)(const void *, const void *));

List *list_sorted(List *list);

List *list_extend(List *list, List *other);

List *list_clone(List *original);
#endif