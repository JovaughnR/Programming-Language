#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "./lib/list.h"

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
   if (!value)
      return LIST_SUCCESS;

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
   void *value = list_top(list);
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
      return list_append(value, list);

   // Resize if needed
   if ((float)list->length / list->capacity > 0.7)
   {
      if (list_resize(list) != LIST_SUCCESS)
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

int list_remove(void *value, List *list, void (*item_free)(const void *))
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
   item_free(list->items[index]);
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
   return cloneData(list->items[index]);
}

int list_set(int index, void *value, List *list, void (*item_free)(const void *))
{
   if (!list || index < 0 || index >= list->length)
      return LIST_ERROR;

   item_free(list->items[index]); // free existing value
   list->items[index] = value;
   return LIST_SUCCESS;
}

int list_length(List *list)
{
   return list ? list->length : 0;
}

void list_clear(List *list, void (*item_free)(const void *))
{
   if (!list)
      return;

   for (int i = 0; i < list->length; i++)
   {
      item_free(list->items[i]);
      list->items[i] = NULL;
   }

   list->length = 0;
   list->end = -1;
}

int list_index(void *value, List *list)
{
   if (!list)
      return -1;

   for (int i = 0; i < list->length; i++)
   {
      if (datacmp(list->items[i], value))
         return i;
   }
   return -1;
}

int list_has(void *value, List *list)
{
   return list_index(value, list) != -1;
}

List *list_concat(List *a, List *b)
{
   if (!a || !b)
      return NULL;

   List *result = list_create(__size__);
   for (int i = 0; i < a->length; i++)
      list_append(cloneData(a->items[i]), result);

   for (int i = 0; i < b->length; i++)
      list_append(cloneData(b->items[i]), result);

   return result;
}

List *list_repeat(int times, List *list)
{
   if (!list || times <= 0)
      return list_create(__size__);

   List *result = list_create(__size__);
   for (int t = 0; t < times; t++)
      for (int i = 0; i < list->length; i++)
         list_append(cloneData(list->items[i]), result);

   return result;
}

int list_equal(List *a, List *b)
{
   if (a->length != b->length)
      return 0;
   for (int i = 0; i < a->length; i++)
      if (datacmp(a->items[i], b->items[i]) != 1)
         return 0;
   return 1;
}

int list_not_equal(List *a, List *b)
{
   return !list_equal(a, b);
}

int list_less(List *a, List *b)
{
   int min_len = (a->length < b->length) ? a->length : b->length;
   for (int i = 0; i < min_len; i++)
   {
      int res = compare_numeric_data(a->items[i], b->items[i]);
      if (res < 0)
         return 1;
      else if (res > 0)
         return 0;
   }
   return a->length < b->length;
}

int list_less_equal(List *a, List *b)
{
   return list_less(a, b) || list_equal(a, b);
}

int list_greater(List *a, List *b)
{
   return !list_less_equal(a, b);
}

int list_greater_equal(List *a, List *b)
{
   return !list_less(a, b);
}

List *list_slice(int start, int end, int step, List *list)
{
   if (!list)
      return NULL;

   int len = list->length;

   if (step == 0)
      return NULL;

   // Resolve sentinels
   if (step > 0)
   {
      if (start == INT_MIN)
         start = 0;
      if (end == INT_MAX)
         end = len;
   }
   else
   {
      if (start == INT_MAX)
         start = len - 1;
      if (end == INT_MIN)
         end = -1; // one before index 0
   }

   // Normalize negative indices — but never touch the -1 sentinel for negative steps
   if (start < 0 && !(step < 0 && start == -1))
      start += len;
   if (end < 0 && !(step < 0 && end == -1))
      end += len;

   // Clamp
   if (step > 0)
   {
      start = start < 0 ? 0 : (start > len ? len : start);
      end = end < 0 ? 0 : (end > len ? len : end);
   }
   else
   {
      start = start >= len ? len - 1 : (start < 0 ? 0 : start);
      end = end >= len ? len - 1 : (end < -1 ? -1 : end);
   }

   // Count iterations
   int newLen = 0;
   if (step > 0)
   {
      if (start < end)
         newLen = 1 + (end - 1 - start) / step;
      else
         newLen = 1 + (start - 1 - end) / (-step);
   }

   List *result = list_create(newLen);

   int j = 0;
   if (step > 0)
      for (int i = start; i < end; i += step)
         result->items[j++] = cloneData(list->items[i]);

   else
      for (int i = start; i > end; i += step)
         result->items[j++] = cloneData(list->items[i]);

   return result;
}

void list_extend(List *src, List *other)
{
   if (!src || !other)
      return;

   for (int i = 0; i < other->length; i++)
   {
      void *cloned = cloneData(other->items[i]);
      if (!cloned)
         continue;
      ;
      list_append(cloned, src);
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

   List *newList = list_create(original->capacity);

   for (int i = 0; i < original->length; i++)
   {
      void *item = original->items[i];
      list_append(cloneData(item), newList);
   }

   return newList;
}

void list_free(List *list, void (*item_free)(const void *))
{
   if (!list)
      return;

   if (item_free)
      for (int i = 0; i <= list->end; i++)
      {
         if (list->items[i])
            item_free(list->items[i]);
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

//    List *list = list_create(PARSE_SIZE);
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