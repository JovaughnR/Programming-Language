#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/set.h"

extern unsigned long hash(void *key);
extern int datacmp(const void *d1, const void *d2);
extern void *cloneData(const void *d);
extern void data_free(const void *data);

Set *set_create()
{
   Set *set = (Set *)malloc(sizeof(Set));
   set->capacity = 0;

   // Allocate memory for the hash table (bucket array).
   set->bucket = (void **)calloc(8, sizeof(void *));
   set->size = 8;
   return set;
}

/**
 * Function: resizeset
 * Resizes the hash table to a new size, rehashing all existing elements.
 *
 * Parameters:
 * - __size: The new size of the hash table.
 * - set: A pointer to the pointer of the Set to be resized.
 */
static void set_resize(Set *set, int new_size)
{
   // New bucket
   void **bucket;
   bucket = calloc(new_size, sizeof(void *));

   for (int i = 0; i < set->size; i++)
   {
      void *v = set->bucket[i];
      if (v && v != TOMBSTONE)
      {
         unsigned long h = hash(v);
         int idx = h % new_size;

         while (bucket[idx])
            idx = (idx + 1) % new_size;

         bucket[idx] = v;
      }
   }

   free(set->bucket);
   set->bucket = bucket;
   set->size = new_size;
}

void set_add(void *value, Set *set)
{
   if (!set)
      return;

   if ((float)set->capacity / set->size > 0.7)
      set_resize(set, set->size * 2);

   unsigned long h = hash(value);
   int idx = h % set->size;

   while (set->bucket[idx])
   {
      // already exists
      if (set->bucket[idx] != TOMBSTONE &&
          datacmp(value, set->bucket[idx]))
         return;

      idx = (idx + 1) % set->size;
   }

   set->bucket[idx] = value;
   set->capacity++;
}

/**
 * Function: set_remove
 * Removes a value from the set if it exists.
 *
 * Parameters:
 * - value: The value to remove.
 * - set: The Set from which the value will be removed.
 */
int set_remove(void *value, Set *set)
{
   int idx = hash(value) % set->size;
   int start = idx;

   while (set->bucket[idx])
   {
      if (set->bucket[idx] != TOMBSTONE &&
          datacmp(value, set->bucket[idx]))
      {
         set->bucket[idx] = TOMBSTONE;
         set->capacity--;
         return 1;
      }

      idx = (idx + 1) % set->size;
      if (idx == start)
         return 0;
   }

   return 0;
}

Set *set_union(Set *A, Set *B)
{
   Set *res = set_create();

   for (int i = 0; i < A->size; i++)
      if (A->bucket[i] && A->bucket[i] != TOMBSTONE)
         set_add(cloneData(A->bucket[i]), res);

   for (int i = 0; i < B->size; i++)
      if (B->bucket[i] && B->bucket[i] != TOMBSTONE)
         set_add(cloneData(B->bucket[i]), res);

   return res;
}

Set *set_intersection(Set *A, Set *B)
{
   Set *res = set_create();

   for (int i = 0; i < A->size; i++)
      if (A->bucket[i] && A->bucket[i] != TOMBSTONE)
         if (set_has(A->bucket[i], B))
            set_add(cloneData(A->bucket[i]), res);

   return res;
}

Set *set_difference(Set *A, Set *B)
{
   Set *res = set_create();

   for (int i = 0; i < A->size; i++)
      if (A->bucket[i] && A->bucket[i] != TOMBSTONE)
         if (!set_has(A->bucket[i], B))
            set_add(cloneData(A->bucket[i]), res); // Fixed argument order

   return res;
}

Set *set_symdiff(Set *A, Set *B)
{
   Set *res = set_create();

   // A - B
   for (int i = 0; i < A->size; i++)
      if (A->bucket[i] && A->bucket[i] != TOMBSTONE)
         if (!set_has(A->bucket[i], B))
            set_add(cloneData(A->bucket[i]), res); // Fixed argument order

   // B - A
   for (int i = 0; i < B->size; i++)
      if (B->bucket[i] && B->bucket[i] != TOMBSTONE)
         if (!set_has(B->bucket[i], A))
            set_add(cloneData(B->bucket[i]), res); // Fixed argument order

   return res;
}

int set_equal(Set *a, Set *b)
{
   if (a->capacity != b->capacity)
      return 0;

   for (int i = 0; i < a->size; i++)
   {
      if (!datacmp(a->bucket[i], b->bucket[i]))
         return 0;
   }

   return 1;
}

/**
 * Function: clearSet
 * Removes all elements from the set.
 *
 * Parameters:
 * - set: The Set to be cleared.
 */
void set_clear(Set *set)
{
   for (int i = 0; i < set->size; i++)
   {
      void *val = set->bucket[i];
      if (val && val != TOMBSTONE)
         data_free(set->bucket[i]);

      set->bucket[i] = NULL; // Clear the slot.
   }
   set->capacity = 0;
}

int set_has(void *value, Set *set)
{
   unsigned long h = hash(value);
   int idx = h % set->size;
   int start = idx;

   while (set->bucket[idx])
   {
      if (set->bucket[idx] != TOMBSTONE &&
          datacmp(value, set->bucket[idx]))
         return 1;

      idx = (idx + 1) % set->size;
      if (idx == start)
         break;
   }
   return 0;
}

int set_len(Set *set)
{
   return set->capacity;
}

Set *set_clone(Set *original)
{
   if (!original)
      return NULL;

   Set *copy = set_create();
   if (!copy)
      return NULL;

   for (int i = 0; i < original->size; i++)
   {
      Data *value = original->bucket[i];
      if (value && value != TOMBSTONE)
         set_add(cloneData(value), copy);
   }

   return copy;
}

void set_free(Set *set)
{
   if (!set)
      return;

   // Free all elements in the set
   for (int i = 0; i < set->size; i++)
   {
      void *elem = set->bucket[i];
      if (elem && elem != TOMBSTONE)
      {
         data_free(elem);
         set->bucket[i] = NULL; // Prevent double-free
      }
   }

   // Free the bucket array
   free(set->bucket);
   set->bucket = NULL;

   // Free the set structure itself
   free(set);

   /************************************************
    *  caller responsible implementing own freeItem
    ************************************************/
}

// int main(void)
// {
//    Set *set = set_create();
//    int x = 9;
//    double y = 9.01;
//    int true = 1;

//    Data *d1 = createData(TYPE_INT, &x);
//    Data *d2 = createData(TYPE_STR, "hello");
//    Data *d3 = createData(TYPE_FLOAT, &y);
//    Data *d4 = createData(TYPE_BOOL, &true);

//    set_add(d1, set);
//    set_add(d2, set);
//    set_add(d3, set);
//    set_add(d4, set);

//    set_remove(d4, set);
//    printf("%s\n", set_to_string(set));
//    printf("Set Has: %d\n", set_has(d4, set));
//    clearSet(set);
//    printf("%s\n", set_to_string(set));
//    return 0;
// }