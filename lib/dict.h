#ifndef DICT_H
#define DICT_H

typedef struct Pair
{
   void *key; // can be int, float, string
   void *value;
   struct Pair *next;
} Pair;

typedef struct Dict
{
   Pair **buckets; // Array of pointers to the bucket (linked lists of key-value pairs)
   int size;       // Number of buckets in the dict
   int capacity;   //  actual amount of pairs in the table
} Dict;

// Function Prototypes

Dict *dict_create(int size);
Pair *pair_create(void *key, void *value);

// These functions should be implemented by whosover includes the module
extern unsigned long hash(void *key);
extern int datacmp(const void *d1, const void *d2);
extern void *cloneData(const void *d);
extern void data_free(const void *data);
extern void pair_free(const void *pair);

/**
 * Inserts a new key-value pair into the hash dict.
 * If a key already exists, it appends the new pair at the end of the linked list at the corresponding bucket.
 *
 * @param key The key to insert (string, int, float)
 * @param value The value associated with the key (can be any type)
 * @param dict The dict where the key-value pair is inserted
 */
int dict_insert(void *key, void *value, Dict *dict);
/**
 * Retrieves the value associated with the given key from the dict.
 *
 * @param key The key whose associated value is to be retrieved
 * @param dict The dict to search
 * @return The value associated with the key, or NULL if the key is not found
 */
void *dict_get(void *key, Dict *dict);
/**
 * Removes a key-value pair from the dict.
 * If the key is found, it is removed from the bucket (linked list).
 *
 * @param key The key to remove
 * @param dict The dict from which the key-value pair is to be removed
 */

void dict_erase(void *key, Dict *dict);
int dict_has(void *key, Dict *dict);
void dict_clear(Dict *dict);
int dict_len(Dict *dict);
void dict_keys(Dict *dict, void **items);
void dict_values(Dict *dict, void **items);
void dict_items(Dict *dict, void **items);
void *dict_pop(void *key, Dict *dict);
void dict_update(Dict *dest, Dict *src);
int dict_equal(Dict *a, Dict *b);
Dict *dict_clone(Dict *original);
int dict_setdefault(void *key, void *def_val, Dict *dict);
void dict_free(Dict *dict);

#endif
