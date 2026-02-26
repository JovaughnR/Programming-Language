#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lib/dict.h"

Dict *dict_create(int size)
{
   Dict *dict = (Dict *)malloc(sizeof(Dict));
   if (!dict)
      return NULL;

   dict->size = size; // Set the size of the dict
   dict->capacity = 0;
   dict->buckets = (Pair **)calloc(size, sizeof(Pair *));
   if (!dict->buckets)
   {
      free(dict);
      return NULL;
   }
   return dict;
}

Pair *pair_create(void *key, void *value)
{
   Pair *pair = (Pair *)malloc(sizeof(Pair));
   if (!pair)
      return NULL;

   pair->key = key;
   pair->value = value;
   pair->next = NULL;
   return pair;
}

/**
 * Resizes the dict by doubling its capacity.
 * It rehashes all existing key-value pairs and reassigns them to the new buckets.
 *
 * @param size the new size
 * @param dict The dict to resize
 */
static void dict_resize(int size, Dict *dict)
{
   Pair **newBucket;
   newBucket = calloc(size, sizeof(Pair *));

   for (int i = 0; i < dict->size; i++)
   {
      Pair *pair = dict->buckets[i];
      while (pair != NULL)
      {
         Pair *next = pair->next;
         int index = hash(pair->key) % size;
         pair->next = newBucket[index];
         newBucket[index] = pair;
         pair = next;
      }
   }

   free(dict->buckets);
   dict->buckets = (Pair **)newBucket;
   dict->size = size;
}

int dict_insert(void *key, void *value, Dict *dict)
{
   if (!key || !dict)
      return 0;

   if ((double)dict->capacity / dict->size > 0.7)
      dict_resize(2 * dict->size, dict);

   int index = hash(key) % dict->size;
   Pair *pair = dict->buckets[index];

   while (pair != NULL)
   {
      if (datacmp(pair->key, key))
      {
         data_free(pair->value);
         pair->value = value;
         data_free(key);
         return 1;
      }
      pair = pair->next;
   }

   Pair *newPair = pair_create(key, value);
   newPair->next = dict->buckets[index];
   dict->buckets[index] = newPair;
   dict->capacity++;
   return 1;
}

void *dict_get(void *key, Dict *dict)
{
   if (!key || !dict)
      return NULL;

   int index = hash(key) % dict->size; // Compute the hash index for the key
   if (index < 0)
      return NULL;

   Pair *pair = dict->buckets[index]; // Get the linked list at this bucket

   while (pair != NULL) // Traverse the linked list at this bucket
   {
      if (datacmp(pair->key, key)) // If the key matches
         return pair->value;       // Return the associated value
      pair = pair->next;           // Move to the next pair in the list
   }

   return NULL; // If key is not found, return NULL
}

void dict_erase(void *key, Dict *dict)
{
   int index = hash(key) % dict->size; // Compute the hash index for the key
   Pair *pair = dict->buckets[index];  // Get the linked list at this bucket

   if (pair == NULL) // If the bucket is empty, do nothing
      return;

   Pair *prev = NULL;   // Previous pair pointer for traversal
   while (pair != NULL) // Traverse the linked list at this bucket
   {
      if (datacmp(pair->key, key)) // If the key matches
      {
         if (prev == NULL)                     // If the pair to remove is the first pair in the list
            dict->buckets[index] = pair->next; // Update the bucket to the next pair
         else
            prev->next = pair->next; // Otherwise, update the previous pair's next pointer

         data_free(pair->key); // Free key in clearDict or eraseDict
         data_free(pair->value);
         free(pair); // Free the memory of the pair
         dict->capacity--;
         return;
      }

      prev = pair; // Move to the next pair in the list
      pair = pair->next;
   }
}

/**
 * Checks if the dict contains a specific key.
 *
 * @param key The key to check for existence in the dict
 * @param dict The dict to search
 * @return 1 if the key exists, 0 if the key does not exist
 */
int dict_has(void *key, Dict *dict)
{
   if (!key || !dict)
      return 0;

   unsigned long h = hash(key);
   int index = h % dict->size;

   Pair *pair = dict->buckets[index];
   while (pair != NULL) // Traverse the linked list at this bucket
   {
      if (datacmp(pair->key, key)) // If the key matches
         return 1;                 // Return 1 to indicate the key exists
      pair = pair->next;           // Move to the next pair in the list
   }
   return 0; // Return 0 if the key does not exist
}

/**
 * Clears all the elements from the dict by freeing the memory allocated for all key-value pairs.
 *
 * @param dict The dict to clear
 */
void dict_clear(Dict *dict)
{
   if (!dict)
      return;

   for (int i = 0; i < dict->size; i++)
   {
      Pair *pair = dict->buckets[i];
      while (pair != NULL)
      {
         Pair *temp = pair;
         pair = pair->next;
         pair_free(temp);
      }

      dict->buckets[i] = NULL; // Reset the bucket to NULL
   }
}

int dict_len(Dict *dict)
{
   return dict->capacity;
}

int dict_keys(Dict *dict, void **items)
{
   int index = 0;
   for (int i = 0; i < dict->size; i++)
   {
      Pair *pair = dict->buckets[i];
      while (pair)
      {
         items[index++] = cloneData(pair->key);
         pair = pair->next;
      }
   }
   return index;
}

int dict_values(Dict *dict, void **items)
{
   int index = 0;
   for (int i = 0; i < dict->size; i++)
   {
      Pair *pair = dict->buckets[i];
      while (pair)
      {
         items[index++] = cloneData(pair->value);
         pair = pair->next;
      }
   }
   return index;
}

void *dict_pop(void *key, Dict *dict)
{
   int index = hash(key) % dict->size;
   if (index == -1)
      return NULL;

   Pair *pair = dict->buckets[index];
   Pair *prev = NULL;

   while (pair)
   {
      if (datacmp(pair->key, key))
      {
         if (prev)
            prev->next = pair->next;
         else
            dict->buckets[index] = pair->next;

         void *val = pair->value;
         data_free(pair->key);
         free(pair);
         dict->capacity--;

         return val;
      }
      prev = pair;
      pair = pair->next;
   }

   return NULL;
}

void dict_update(Dict *dest, Dict *src)
{
   for (int i = 0; i < src->size; i++)
   {
      Pair *pair = src->buckets[i];
      while (pair)
      {
         void *key = cloneData(pair->key);
         void *value = cloneData(pair->value);

         dict_insert(key, value, dest);
         pair = pair->next;
      }
   }

   /************************************
    *  TODO: check on implementation
    ***********************************/
}

int dict_equal(Dict *a, Dict *b)
{
   if (a->capacity != b->capacity)
      return 0;

   for (int i = 0; i < a->size; i++)
   {
      Pair *a_pair = a->buckets[i];
      Pair *b_pair = b->buckets[i];

      while (a_pair && b_pair)
      {
         if (!datacmp(a_pair->key, b_pair->key))
            return 0;

         if (!datacmp(a_pair->value, b_pair->value))
            return 0;

         a_pair = a_pair->next;
         b_pair = b_pair->next;
      }
   }
   return 1;
}

Dict *dict_clone(Dict *original)
{
   if (!original)
      return NULL;

   Dict *copy = dict_create(original->size);
   if (!copy)
      return NULL;

   for (int i = 0; i < original->size; i++)
   {
      Pair *pair = original->buckets[i];
      while (pair)
      {
         void *key = cloneData(pair->key);
         void *val = cloneData(pair->value);

         dict_insert(key, val, copy);
         pair = pair->next;
      }
   }
   return copy;
}

int dict_setdefault(void *key, void *def_val, Dict *dict)
{
   return dict_insert(key, def_val, dict);
}

void dict_free(Dict *dict)
{
   if (!dict)
      return;

   for (int i = 0; i < dict->size; i++)
   {
      Pair *pair = dict->buckets[i];
      while (pair)
      {
         Pair *temp = pair;
         pair = pair->next;
         pair_free(temp);
      }
   }

   free(dict->buckets);
   free(dict);

   /*******************************
    * TODO: CHECK ON IMPLEMENTATION
    ********************************/
}