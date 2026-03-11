#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "./lib/thread.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"
#include "../core/lib/format.h"

const ModuleFuncInfo THREAD_MODULE[] = {
    {MODULE_THREAD, THREAD_CREATE, "__thread_create__", 2, 3},
    {MODULE_THREAD, THREAD_JOIN, "__thread_join__", 1, 1},
    {MODULE_THREAD, THREAD_DETACH, "__thread_detach__", 1, 1},
    {MODULE_THREAD, THREAD_SLEEP, "__thread_sleep__", 1, 1},
    {MODULE_THREAD, THREAD_ID, "__thread_id__", 0, 0},
    {MODULE_THREAD, MUTEX_CREATE, "__mutex_create__", 0, 0},
    {MODULE_THREAD, MUTEX_LOCK, "__mutex_lock__", 1, 1},
    {MODULE_THREAD, MUTEX_UNLOCK, "__mutex_unlock__", 1, 1},
    {MODULE_THREAD, MUTEX_DESTROY, "__mutex_destroy__", 1, 1},
    {MODULE_THREAD, COND_CREATE, "__cond_create__", 0, 0},
    {MODULE_THREAD, COND_WAIT, "__cond_wait__", 2, 2},
    {MODULE_THREAD, COND_SIGNAL, "__cond_signal__", 1, 1},
    {MODULE_THREAD, COND_BROADCAST, "__cond_broadcast__", 1, 1},
    {MODULE_THREAD, COND_DESTROY, "__cond_destroy__", 1, 1},
};

const int THREAD_MODULE_COUNT = sizeof(THREAD_MODULE) / sizeof(ModuleFuncInfo);

// At top of thread.c — global interpreter lock
static pthread_mutex_t g_interp_lock = PTHREAD_MUTEX_INITIALIZER;

void interp_lock(void)
{
   pthread_mutex_lock(&g_interp_lock);
}

void interp_unlock(void)
{
   pthread_mutex_unlock(&g_interp_lock);
}

#define MAX_THREADS 256

// Package passed to the thread worker
typedef struct
{
   Data *func;
   Data *args_data;
   Runtime *rt;
   Data *result;
} ThreadPackage;

typedef struct
{
   pthread_t tid;
   ThreadPackage *pkg;
} ThreadEntry;

static ThreadEntry g_threads[MAX_THREADS];
static int g_thread_count = 0;
static pthread_mutex_t g_registry_mutex = PTHREAD_MUTEX_INITIALIZER;

//=========================================================
//  Helpers
//=========================================================

static void registry_add(pthread_t tid, ThreadPackage *pkg)
{
   pthread_mutex_lock(&g_registry_mutex);
   if (g_thread_count < MAX_THREADS)
   {
      g_threads[g_thread_count].tid = tid;
      g_threads[g_thread_count].pkg = pkg;
      g_thread_count++;
   }
   pthread_mutex_unlock(&g_registry_mutex);
}

static ThreadPackage *registry_get(pthread_t tid)
{
   pthread_mutex_lock(&g_registry_mutex);
   for (int i = 0; i < g_thread_count; i++)
   {
      if (pthread_equal(g_threads[i].tid, tid))
      {
         ThreadPackage *pkg = g_threads[i].pkg;
         pthread_mutex_unlock(&g_registry_mutex);
         return pkg;
      }
   }
   pthread_mutex_unlock(&g_registry_mutex);
   return NULL;
}

static void registry_remove(pthread_t tid)
{
   pthread_mutex_lock(&g_registry_mutex);
   for (int i = 0; i < g_thread_count; i++)
   {
      if (pthread_equal(g_threads[i].tid, tid))
      {
         g_threads[i] = g_threads[--g_thread_count];
         break;
      }
   }
   pthread_mutex_unlock(&g_registry_mutex);
}

//=========================================================
//  Thread Worker
//=========================================================

static void *thread_worker(void *arg)
{
   ThreadPackage *pkg = (ThreadPackage *)arg;

   if (!pkg || !pkg->func)
   {
      if (pkg)
         pkg->result = createData(TYPE_NONE, NULL);
      return pkg;
   }

   List *args = NULL;
   if (pkg->args_data && pkg->args_data->type == TYPE_LIST)
      args = LIST_PTR(pkg->args_data);
   else
      args = list_create(__len__);

   Env *thread_env = env_create(NULL);
   Runtime *thread_rt = malloc(sizeof(Runtime));
   memcpy(thread_rt, pkg->rt, sizeof(Runtime));
   thread_rt->env = thread_env;

   // Clear any existing error before running
   clear_error();

   interp_lock();
   pkg->result = executeFunction(pkg->func, args, thread_rt);
   interp_unlock();

   // If an error occurred inside the thread, capture it
   if (g_thrown.error)
   {
      fprintf(stderr, "ThreadError in thread %lu: %s\n",
              (unsigned long)pthread_self(), g_thrown.message);
      clear_error();
      if (!pkg->result)
         pkg->result = createData(TYPE_NONE, NULL);
   }
   env_free(thread_env);
   free(thread_rt);
   return pkg;
}

static pthread_t *get_thread(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE)
   {
      throw_error(ERROR_RUNTIME,
                  "%s(): thread handle is none", funcname);
      return NULL;
   }
   if (d->type != TYPE_THREAD || !d->ref || !d->ref->object)
   {
      throw_error(ERROR_RUNTIME,
                  "%s(): invalid or already-joined thread handle", funcname);
      return NULL;
   }
   return (pthread_t *)d->ref->object;
}

static pthread_mutex_t *get_mutex(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE)
   {
      throw_error(ERROR_RUNTIME,
                  "%s(): mutex is none — are you accessing a variable from outside the thread?",
                  funcname);
      return NULL;
   }
   if (d->type != TYPE_MUTEX || !d->ref || !d->ref->object)
   {
      throw_error(ERROR_RUNTIME,
                  "%s(): invalid or destroyed mutex", funcname);
      return NULL;
   }
   return (pthread_mutex_t *)d->ref->object;
}

static pthread_cond_t *get_cond(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE)
   {
      throw_error(ERROR_RUNTIME,
                  "%s(): condition variable is none — are you accessing a variable from outside the thread?",
                  funcname);
      return NULL;
   }
   if (d->type != TYPE_COND || !d->ref || !d->ref->object)
   {
      throw_error(ERROR_RUNTIME,
                  "%s(): invalid or destroyed condition variable", funcname);
      return NULL;
   }
   return (pthread_cond_t *)d->ref->object;
}

//=========================================================
//  Implementations
//=========================================================

// create(func, args=[], name="") -> thread
// Creates and starts a new thread running func with args.
// Returns a thread handle for use with join() or detach().
// Example: t = thread.create(my_func, [1, 2, 3])
// Example: t = thread.create(worker, [data])
Data *__thread_create__(Data *func, Data *args, Data *name, Runtime *rt)
{
   if (!func || (func->type != TYPE_FUNCTION && func->type != TYPE_LAMBDA))
   {
      throw_error(ERROR_TYPE,
                  "thread.create() first argument must be a function, got '%s'",
                  func ? getDataType(func->type) : "none");
      return createData(TYPE_NONE, NULL);
   }

   ThreadPackage *pkg = malloc(sizeof(ThreadPackage));
   pkg->func = cloneData(func);
   if (args && args->type == TYPE_LIST)
      pkg->args_data = cloneData(args);
   else
      pkg->args_data = createData(TYPE_LIST, list_create(__len__));

   pkg->rt = rt;
   pkg->result = NULL;

   pthread_t *tid = malloc(sizeof(pthread_t));
   int rc = pthread_create(tid, NULL, thread_worker, pkg);
   if (rc != 0)
   {
      free(tid);
      data_free(pkg->func);
      data_free(pkg->args_data);
      free(pkg);
      throw_error(ERROR_RUNTIME, "thread.create(): failed to create thread");
      return createData(TYPE_NONE, NULL);
   }

   // Store pkg in global registry keyed by tid
   registry_add(*tid, pkg);
   return createData(TYPE_THREAD, tid);
}
// join(thread) -> any
// Waits for a thread to finish and returns its result.
// Blocks until the thread completes.
// Example: result = thread.join(t)
Data *__thread_join__(Data *thread_data)
{
   pthread_t *tid = get_thread(thread_data, "thread.join");
   if (!tid)
      return createData(TYPE_NONE, NULL);

   // Check if already joined
   ThreadPackage *pkg = registry_get(*tid);
   if (!pkg)
   {
      throw_error(ERROR_RUNTIME, "thread.join(): thread has already been joined");
      return createData(TYPE_NONE, NULL);
   }

   void *retval;
   int rc = pthread_join(*tid, &retval);
   if (rc != 0)
   {
      throw_error(ERROR_RUNTIME, "thread.join(): failed to join thread");
      return createData(TYPE_NONE, NULL);
   }

   registry_remove(*tid);

   Data *result = createData(TYPE_NONE, NULL);
   if (pkg && pkg->result)
   {
      data_free(result);
      result = cloneData(pkg->result);
      data_free(pkg->result);
      data_free(pkg->func);
      data_free(pkg->args_data);
      free(pkg);
   }

   return result;
}

// detach(thread) -> none
// Detaches a thread so it runs independently.
// A detached thread cannot be joined.
// Example: thread.detach(t)
Data *__thread_detach__(Data *thread_data)
{
   pthread_t *tid = get_thread(thread_data, "thread.detach");
   if (!tid)
      return createData(TYPE_NONE, NULL);

   pthread_detach(*tid);
   return createData(TYPE_NONE, NULL);
}

// sleep(ms) -> none
// Sleeps the current thread for ms milliseconds.
// Example: thread.sleep(1000)  # sleep 1 second
// Example: thread.sleep(500)   # sleep 500ms
Data *__thread_sleep__(Data *ms)
{
   if (!ms || ms->type != TYPE_INT)
   {
      throw_error(ERROR_TYPE, "thread.sleep() argument must be an integer (milliseconds)");
      return createData(TYPE_NONE, NULL);
   }

   int millis = *(int *)ms->atom;
   usleep((useconds_t)(millis * 1000));
   return createData(TYPE_NONE, NULL);
}

// id() -> int
// Returns the current thread's ID.
// Example: print(thread.id())
Data *__thread_id__(void)
{
   unsigned long tid = (unsigned long)pthread_self();
   int id = (int)(tid % 1000000); // truncate to readable int
   return createData(TYPE_INT, &id);
}

// mutex_create() -> mutex
// Creates a new mutex for thread synchronization.
// Example: m = thread.mutex_create()
Data *__mutex_create__(void)
{
   pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
   pthread_mutex_init(mutex, NULL);

   Data *result = createData(TYPE_MUTEX, mutex);
   return result;
}

// mutex_lock(mutex) -> none
// Acquires a mutex lock. Blocks if already locked.
// Example: thread.mutex_lock(m)
Data *__mutex_lock__(Data *mutex_data)
{
   pthread_mutex_t *mutex = get_mutex(mutex_data, "thread.mutex_lock");
   if (!mutex)
      return createData(TYPE_NONE, NULL);

   pthread_mutex_lock(mutex);
   return createData(TYPE_NONE, NULL);
}

// mutex_unlock(mutex) -> none
// Releases a mutex lock.
// Example: thread.mutex_unlock(m)
Data *__mutex_unlock__(Data *mutex_data)
{
   pthread_mutex_t *mutex = get_mutex(mutex_data, "thread.mutex_unlock");
   if (!mutex)
      return createData(TYPE_NONE, NULL);

   pthread_mutex_unlock(mutex);
   return createData(TYPE_NONE, NULL);
}

// mutex_destroy(mutex) -> none
// Destroys a mutex and releases its resources.
// Example: thread.mutex_destroy(m)
Data *__mutex_destroy__(Data *mutex_data)
{
   pthread_mutex_t *mutex = get_mutex(mutex_data, "thread.mutex_destroy");
   if (!mutex)
      return createData(TYPE_NONE, NULL);

   pthread_mutex_destroy(mutex);
   mutex_data->ref->object = NULL;
   return createData(TYPE_NONE, NULL);
}

// cond_create() -> cond
// Creates a condition variable for thread coordination.
// Example: c = thread.cond_create()
Data *__cond_create__(void)
{
   pthread_cond_t *cond = malloc(sizeof(pthread_cond_t));
   pthread_cond_init(cond, NULL);

   Data *result = createData(TYPE_COND, cond);
   return result;
}

// cond_wait(cond, mutex) -> none
// Waits on a condition variable, atomically releasing the mutex.
// The mutex must be locked before calling cond_wait.
// Example: thread.cond_wait(c, m)
Data *__cond_wait__(Data *cond_data, Data *mutex_data)
{
   pthread_cond_t *cond = get_cond(cond_data, "thread.cond_wait");
   pthread_mutex_t *mutex = get_mutex(mutex_data, "thread.cond_wait");
   if (!cond || !mutex)
      return createData(TYPE_NONE, NULL);

   pthread_cond_wait(cond, mutex);
   return createData(TYPE_NONE, NULL);
}

// cond_signal(cond) -> none
// Wakes up one thread waiting on the condition variable.
// Example: thread.cond_signal(c)
Data *__cond_signal__(Data *cond_data)
{
   pthread_cond_t *cond = get_cond(cond_data, "thread.cond_signal");
   if (!cond)
      return createData(TYPE_NONE, NULL);

   pthread_cond_signal(cond);
   return createData(TYPE_NONE, NULL);
}

// cond_broadcast(cond) -> none
// Wakes up ALL threads waiting on the condition variable.
// Example: thread.cond_broadcast(c)
Data *__cond_broadcast__(Data *cond_data)
{
   pthread_cond_t *cond = get_cond(cond_data, "thread.cond_broadcast");
   if (!cond)
      return createData(TYPE_NONE, NULL);

   pthread_cond_broadcast(cond);
   return createData(TYPE_NONE, NULL);
}

// cond_destroy(cond) -> none
// Destroys a condition variable and releases its resources.
// Example: thread.cond_destroy(c)
Data *__cond_destroy__(Data *cond_data)
{
   pthread_cond_t *cond = get_cond(cond_data, "thread.cond_destroy");
   if (!cond)
      return createData(TYPE_NONE, NULL);

   pthread_cond_destroy(cond);
   cond_data->ref->object = NULL;
   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeThreadModule(Runtime *mod_rt)
{
   for (int i = 0; i < THREAD_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)THREAD_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&THREAD_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchThreadBuiltin(ThreadType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);

   switch (type)
   {
   case THREAD_CREATE:
      return __thread_create__(ARG(0), ARG(1), ARG(2), rt);
   case THREAD_JOIN:
      return __thread_join__(ARG(0));
   case THREAD_DETACH:
      return __thread_detach__(ARG(0));
   case THREAD_SLEEP:
      return __thread_sleep__(ARG(0));
   case THREAD_ID:
      return __thread_id__();
   case MUTEX_CREATE:
      return __mutex_create__();
   case MUTEX_LOCK:
      return __mutex_lock__(ARG(0));
   case MUTEX_UNLOCK:
      return __mutex_unlock__(ARG(0));
   case MUTEX_DESTROY:
      return __mutex_destroy__(ARG(0));
   case COND_CREATE:
      return __cond_create__();
   case COND_WAIT:
      return __cond_wait__(ARG(0), ARG(1));
   case COND_SIGNAL:
      return __cond_signal__(ARG(0));
   case COND_BROADCAST:
      return __cond_broadcast__(ARG(0));
   case COND_DESTROY:
      return __cond_destroy__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown thread builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}