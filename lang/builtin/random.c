#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "./lib/random.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo RANDOM_MODULE[] = {
    {MODULE_RANDOM, RANDOM_RANDOM, "__random__", 0, 0},
    {MODULE_RANDOM, RANDOM_RANDINT, "__random_randint__", 2, 2},
    {MODULE_RANDOM, RANDOM_CHOICE, "__random_choice__", 1, 1},
    {MODULE_RANDOM, RANDOM_SHUFFLE, "__random_shuffle__", 1, 1},
    {MODULE_RANDOM, RANDOM_SEED, "__random_seed__", 1, 1},
    {MODULE_RANDOM, RANDOM_UNIFORM, "__random_uniform__", 2, 2},
    {MODULE_RANDOM, RANDOM_SAMPLE, "__random_sample__", 2, 2},
    {MODULE_RANDOM, RANDOM_RANDRANGE, "__random_randrange__", 1, 3},
};

const int RANDOM_MODULE_COUNT = sizeof(RANDOM_MODULE) / sizeof(ModuleFuncInfo);

// Seed on first use
static int seeded = 0;

static void ensure_seeded(void)
{
   if (!seeded)
   {
      srand((unsigned int)time(NULL));
      seeded = 1;
   }
}

//=========================================================
//  Helpers
//=========================================================

static double rand_double(void)
{
   return (double)rand() / ((double)RAND_MAX + 1.0);
}

static int get_int(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE || d->type != TYPE_INT)
   {
      throw_error(ERROR_TYPE, "%s() argument must be an integer", funcname);
      return 0;
   }
   return *(int *)d->atom;
}
//=========================================================
//  Implementations
//=========================================================

// random() -> float
// Returns a random float in the range [0.0, 1.0).
// Example: random.random() -> 0.472381
Data *__random__(void)
{
   ensure_seeded();
   double val = rand_double();
   return createData(TYPE_FLOAT, &val);
}

// randint(a, b) -> int
// Returns a random integer N such that a <= N <= b.
// Example: random.randint(1, 6) -> 4
Data *__random_randint__(Data *a, Data *b)
{
   ensure_seeded();
   int lo = get_int(a, "random.randint");
   int hi = get_int(b, "random.randint");

   if (lo > hi)
   {
      throw_error(ERROR_VALUE, "random.randint(): a must be <= b");
      return createData(TYPE_NONE, NULL);
   }

   int val = lo + rand() % (hi - lo + 1);
   return createData(TYPE_INT, &val);
}

// uniform(a, b) -> float
// Returns a random float N such that a <= N <= b.
// Example: random.uniform(1.0, 10.0) -> 7.342
Data *__random_uniform__(Data *a, Data *b)
{
   ensure_seeded();

   double lo = a->type == TYPE_FLOAT ? *(double *)a->real : (double)*(int *)a->atom;
   double hi = b->type == TYPE_FLOAT ? *(double *)b->real : (double)*(int *)b->atom;

   if (lo > hi)
   {
      throw_error(ERROR_VALUE, "random.uniform(): a must be <= b");
      return createData(TYPE_NONE, NULL);
   }

   double val = lo + rand_double() * (hi - lo);
   return createData(TYPE_FLOAT, &val);
}

// randrange(stop) | randrange(start, stop) | randrange(start, stop, step) -> int
// Returns a random integer from range(start, stop, step).
// Example: random.randrange(10)      -> 7
// Example: random.randrange(0, 10)   -> 3
// Example: random.randrange(0, 10, 2) -> 4  (even numbers only)
Data *__random_randrange__(Data *a, Data *b, Data *step)
{
   ensure_seeded();

   int start = 0, stop = 0, s = 1;

   if (!b || b->type == TYPE_NONE)
   {
      // randrange(stop)
      stop = get_int(a, "random.randrange");
   }
   else
   {
      start = get_int(a, "random.randrange");
      stop = get_int(b, "random.randrange");
      if (step && step->type == TYPE_INT)
         s = *(int *)step->atom;
   }

   if (s == 0)
   {
      throw_error(ERROR_VALUE, "random.randrange(): step cannot be zero");
      return createData(TYPE_NONE, NULL);
   }

   int n = (stop - start + s - (s > 0 ? 1 : -1)) / s;
   if (n <= 0)
   {
      throw_error(ERROR_VALUE, "random.randrange(): empty range");
      return createData(TYPE_NONE, NULL);
   }

   int val = start + s * (rand() % n);
   return createData(TYPE_INT, &val);
}

// choice(seq) -> any
// Returns a random element from a non-empty list.
// Example: random.choice([1, 2, 3, 4, 5]) -> 3
Data *__random_choice__(Data *seq)
{
   ensure_seeded();

   if (!seq || seq->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "random.choice() argument must be a list");
      return createData(TYPE_NONE, NULL);
   }

   List *list = LIST_PTR(seq);
   if (list->length == 0)
   {
      throw_error(ERROR_VALUE, "random.choice(): list is empty");
      return createData(TYPE_NONE, NULL);
   }

   int idx = rand() % list->length;
   return cloneData((Data *)list->items[idx]);
}

// shuffle(list) -> none
// Shuffles a list in place using Fisher-Yates algorithm.
// Example: random.shuffle(x)
Data *__random_shuffle__(Data *seq)
{
   ensure_seeded();

   if (!seq || seq->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "random.shuffle() argument must be a list");
      return createData(TYPE_NONE, NULL);
   }

   List *list = LIST_PTR(seq);
   for (int i = list->length - 1; i > 0; i--)
   {
      int j = rand() % (i + 1);
      void *tmp = list->items[i];
      list->items[i] = list->items[j];
      list->items[j] = tmp;
   }

   return createData(TYPE_NONE, NULL);
}

// sample(list, k) -> list
// Returns a new list of k unique random elements from the list.
// Example: random.sample([1, 2, 3, 4, 5], 3) -> [4, 1, 3]
Data *__random_sample__(Data *seq, Data *k)
{
   ensure_seeded();

   if (!seq || seq->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "random.sample() first argument must be a list");
      return createData(TYPE_NONE, NULL);
   }

   List *list = LIST_PTR(seq);
   int count = get_int(k, "random.sample");

   if (count < 0 || count > list->length)
   {
      throw_error(ERROR_VALUE, "random.sample(): k must be between 0 and len(list)");
      return createData(TYPE_NONE, NULL);
   }

   // Copy indices and do a partial Fisher-Yates shuffle
   int *indices = malloc(list->length * sizeof(int));
   for (int i = 0; i < list->length; i++)
      indices[i] = i;

   for (int i = 0; i < count; i++)
   {
      int j = i + rand() % (list->length - i);
      int tmp = indices[i];
      indices[i] = indices[j];
      indices[j] = tmp;
   }

   List *result = list_create(__size__);
   for (int i = 0; i < count; i++)
      list_append(cloneData((Data *)list->items[indices[i]]), result);

   free(indices);
   return createData(TYPE_LIST, result);
}

// seed(n) -> none
// Seeds the random number generator for reproducible results.
// Example: random.seed(42)
Data *__random_seed__(Data *n)
{
   int val = get_int(n, "random.seed");
   srand((unsigned int)val);
   seeded = 1;
   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeRandomModule(Runtime *mod_rt)
{
   for (int i = 0; i < RANDOM_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)RANDOM_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&RANDOM_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchRandomBuiltin(RandomType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case RANDOM_RANDOM:
      return __random__();
   case RANDOM_RANDINT:
      return __random_randint__(ARG(0), ARG(1));
   case RANDOM_UNIFORM:
      return __random_uniform__(ARG(0), ARG(1));
   case RANDOM_RANDRANGE:
      return __random_randrange__(ARG(0), ARG(1), ARG(2));
   case RANDOM_CHOICE:
      return __random_choice__(ARG(0));
   case RANDOM_SHUFFLE:
      return __random_shuffle__(ARG(0));
   case RANDOM_SAMPLE:
      return __random_sample__(ARG(0), ARG(1));
   case RANDOM_SEED:
      return __random_seed__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown random builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}