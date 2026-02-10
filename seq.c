#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./lib/list.h"
#include "./lib/daloc.h"
#include "./lib/maloc.h"
#include "./lib/utils.h"

#define LIST_SUCCESS 0
#define LIST_ERROR -1

List *list_create(int size)
{
   List *list = (List *)malloc(sizeof(List));
   if (!list)
      return NULL;

   list->items = malloc(size * sizeof(void *));
   if (!list->items)
   {
      free(list);
      return NULL;
   }

   for (int i = 0; i < size; i++)
      list->items[i] = NULL;

   list->end = -1;
   list->length = 0;
   list->capacity = size;
   return list;
}

/**
 * Resizes the list to accommodate more elements.
 *
 * @param list Pointer to the list to resize.
 * @return LIST_SUCCESS if resizing is successful, LIST_ERROR if memory allocation fails.
 */
static int list_resize(List *list)
{
   int newSize = 2 * list->capacity; // Double the list's capacity.
   void **newList = realloc(list->items, newSize * sizeof(void *));
   if (!newList)
      return LIST_ERROR;

   // Initialize the newly added space.
   for (int i = list->capacity; i < newSize; i++)
      newList[i] = NULL;

   list->items = (void **)newList;
   list->capacity = newSize;
   return LIST_SUCCESS;
}

int list_append(void *value, List *list)
{
   // Resize the list if it is more than 70% full.
   if ((float)list->length / list->capacity > 0.7)
      if (list_resize(list) != LIST_SUCCESS)
         return LIST_ERROR;

   list->end = (list->end + 1) % list->capacity;
   list->items[list->end] = value;
   list->length++;
   return LIST_SUCCESS;
}

void *list_top(List *list)
{
   if (!list || !list->length)
      return NULL;
   return list->items[list->end % list->capacity];
}

void *list_pop(List *list)
{
   void *value = top(list);
   if (value)
   {
      list->items[list->end % list->capacity] = NULL;
      list->end = (list->end - 1) % list->capacity;
      list->length--;
   }
   return value;
}

int list_insert(List *list, int index, void *value)
{
   if (!list || index < 0)
      return LIST_ERROR;

   if (index >= list->length)
      return append(value, list);

   // Resize if needed
   if ((float)list->length / list->capacity > 0.7)
   {
      if (resizeList(list) != LIST_SUCCESS)
         return LIST_ERROR;
   }

   // Shift elements right
   for (int i = list->length; i > index; i--)
      list->items[i] = list->items[i - 1];

   list->items[index] = value;
   list->length++;
   list->end = list->length - 1;
   return LIST_SUCCESS;
}

int list_remove(void *value, List *list, void (*freeItem)(void *))
{
   if (!list)
      return LIST_ERROR;

   int index = -1;

   for (int i = 0; i < list->length; i++)
   {
      if (list->items[i] == value)
      {
         index = i;
         break;
      }
   }

   if (index == -1)
      return LIST_ERROR;

   // Shift left
   freeItem(list->items[index]);
   for (int i = index; i < list->length - 1; i++)
      list->items[i] = list->items[i + 1];

   list->length--;
   list->end = list->length - 1;
   return LIST_SUCCESS;
}

void *list_get(int index, List *list)
{
   if (!list || index < 0 || index >= list->length)
   {
      fprintf(stderr, "List index out of range\n");
      exit(1);
   }
   return list->items[index];
}

int list_set(int index, void *value, List *list)
{
   if (!list || index < 0 || index >= list->length)
      return LIST_ERROR;

   freeData(list->items[index]); // free existing value
   list->items[index] = value;
   return LIST_SUCCESS;
}

int list_length(List *list)
{
   return list ? list->length : 0;
}

void list_clear(List *list)
{
   if (!list)
      return;

   for (int i = 0; i < list->length; i++)
   {
      freeData(list->items[i]);
      list->items[i] = NULL;
   }

   list->length = 0;
   list->end = -1;
}

int list_index(void *value, List *list, Compare cmp)
{
   if (!list)
      return -1;

   for (int i = 0; i < list->length; i++)
   {
      if (cmp(list->items[i], value))
         return i;
   }
   return -1;
}

int list_has(void *value, List *list, Compare cmp)
{
   return list_index(value, list, cmp) != -1;
}

List *list_concat(List *a, List *b)
{
   if (!a || !b)
      return NULL;

   List *result = createList(RUNTIME_SIZE);
   for (int i = 0; i < a->length; i++)
      append(cloneData(a->items[i]), result);

   for (int i = 0; i < b->length; i++)
      append(cloneData(b->items[i]), result);

   return result;
}

List *list_repeat(int times, List *list)
{
   if (!list || times <= 0)
      return createList(RUNTIME_SIZE);

   List *result = createList(RUNTIME_SIZE);
   for (int t = 0; t < times; t++)
      for (int i = 0; i < list->length; i++)
         append(cloneData(list->items[i]), result);

   return result;
}

int lists_equal(List *a, List *b, Compare cmp)
{
   if (a->length != b->length)
      return 0;
   for (int i = 0; i < a->length; i++)
      if (cmp(a->items[i], b->items[i]) != 0)
         return 0;
   return 1;
}

int list_not_equal(List *a, List *b, Compare cmp)
{
   return !list_equal(a, b, cmp);
}

int list_less(List *a, List *b, Compare cmp)
{
   int min_len = (a->length < b->length) ? a->length : b->length;
   for (int i = 0; i < min_len; i++)
   {
      int res = cmp(a->items[i], b->items[i]);
      if (res < 0)
         return 1;
      else if (res > 0)
         return 0;
   }
   return a->length < b->length;
}

int list_less_equal(List *a, List *b, Compare cmp)
{
   return list_less(a, b, cmp) || list_equal(a, b, cmp);
}

int list_greater(List *a, List *b, Compare cmp)
{
   return !list_less_equal(a, b, cmp);
}

int list_greater_equal(List *a, List *b, Compare cmp)
{
   return !list_less(a, b, cmp);
}

List *list_slice(int start, int stop, List *list)
{
   if (!list)
      return NULL;

   int len = list->length;

   // Handle negative indices (Python-style: -1 is last element)
   if (start < 0)
      start = len + start;
   if (stop < 0)
      stop = len + stop;

   // Clamp to valid range [0, len]
   if (start < 0)
      start = 0;
   if (start > len)
      start = len;
   if (stop < 0)
      stop = 0;
   if (stop > len)
      stop = len;

   // Return empty list when invalid range
   if (start >= stop)
      return createList(RUNTIME_SIZE);

   List *slice = createList(RUNTIME_SIZE);
   if (!slice)
      return NULL;

   for (int i = start; i < stop; i++)
   {
      Data *cloned = cloneData(list_get(list, i));
      if (!cloned)
      {
         // Cleanup on failure
         list_clear(slice);
         free(slice->items);
         free(slice);
         return NULL;
      }
      if (append(cloned, slice) != LIST_SUCCESS)
      {
         freeData(cloned);
         list_clear(slice);
         free(slice->items);
         free(slice);
         return NULL;
      }
   }

   return slice;
}

/**
 * Comparator function for qsort that compares numeric Data objects.
 *
 * @param a Pointer to first Data* pointer
 * @param b Pointer to second Data* pointer
 * @return Negative if a < b, 0 if equal, positive if a > b
 */
static int compare_numeric_data(const void *a, const void *b)
{
   Data *data_a = *(Data **)a;
   Data *data_b = *(Data **)b;

   // Handle NULL cases
   if (!data_a || !data_b)
      return 0;

   // Only compare if both are numeric
   if (!isNumeric(data_a->type) || !isNumeric(data_b->type))
      return 0;

   double val_a = valueToFloat(data_a);
   double val_b = valueToFloat(data_b);

   if (val_a < val_b)
      return -1;
   if (val_a > val_b)
      return 1;
   return 0;
}

List *list_sorted(List *list)
{
   if (!list || list->length == 0)
      return createList(RUNTIME_SIZE);

   // Create a new list to hold the sorted result
   List *result = createList(RUNTIME_SIZE);
   if (!result)
      return NULL;

   // Copy all elements from original list (cloning the data)
   for (int i = 0; i < list->length; i++)
   {
      Data *cloned = cloneData(list->items[i]);
      if (!cloned)
      {
         // Cleanup on failure
         list_clear(result);
         free(result->items);
         free(result);
         return NULL;
      }
      if (append(cloned, result) != LIST_SUCCESS)
      {
         freeData(cloned);
         list_clear(result);
         free(result->items);
         free(result);
         return NULL;
      }
   }

   // Sort the items array in-place using qsort
   qsort(result->items, result->length, sizeof(void *), compare_numeric_data);
   return result;
}

void list_extend(List *src, List *other)
{
   if (!src || !other)
      return;

   for (int i = 0; i < other->length; i++)
   {
      Data *cloned = cloneData(other->items[i]);
      if (!cloned)
         return;
      if (list_append(cloned, src) != LIST_SUCCESS)
      {
         freeData(cloned);
         return;
      }
   }
}

void list_reverse(List *src)
{
   if (!src)
      return;

   int midpoint = src->length / 2;
   int stop = src->end;

   for (int start = 0; start < midpoint; start++)
   {
      if (start == stop)
         break;

      void *tmp = src->items[start];
      src->items[start] = src->items[stop];
      src->items[stop] = tmp;
      stop--;
   }
}

List *list_clone(List *original)
{
   if (!original)
      return NULL;

   List *newList = createList(original->capacity);

   for (int i = 0; i < original->length; i++)
   {
      Data *item = original->items[i];
      list_append(cloneData(item), newList);
   }

   return newList;
}

void list_free(List *list, void (*freeItem)(void *))
{
   if (!list)
      return;

   if (freeItem)
      for (int i = 0; i <= list->end; i++)
      {
         if (list->items[i])
            freeItem(list->items[i]);
      }

   free(list->items);
   free(list);

   /************************************************
    *  caller responsible implementing own freePair
    ************************************************/
}

// Example usage in main:
// int main(void)
// {
//    int x = 9, y = 10, p = 3, z = 27, r = 4, j = 7;
//    double d = 1.9, e = 5.5, f = 2.3;
//    int true = 1;

//    List *list = createList(PARSE_SIZE);
//    append(createData(TYPE_INT, &p), list);
//    append(createData(TYPE_INT, &j), list);
//    append(createData(TYPE_FLOAT, &d), list); // Add doubles
//    append(createData(TYPE_INT, &z), list);
//    append(createData(TYPE_INT, &x), list);
//    append(createData(TYPE_FLOAT, &e), list);
//    append(createData(TYPE_INT, &y), list);
//    append(createData(TYPE_FLOAT, &f), list);
//    append(createData(TYPE_INT, &r), list);
//    append(createData(TYPE_BOOL, &true), list);

//    printf("Original: %s\n", listTostring(list));

//    // Get a sorted copy (original unchanged)
//    List *sorted_list = list_sorted(list);
//    printf("Sorted:   %s\n", listTostring(sorted_list));
//    printf("Original: %s\n", listTostring(list)); // Still unsorted

//    printf("Sliced:   %s\n", listTostring(list_slice(sorted_list, 2, 5)));
//    // Clean up BOTH lists
//    list_clear(sorted_list);
//    free(sorted_list->items);
//    free(sorted_list);

//    list_clear(list);
//    free(list->items);
//    free(list);

//    return 0;
// }