#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "./lib/async.h"
#include "./lib/thread.h"

#include "../core/lib/type.h"
#include "../core/lib/error.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/class.h"
#include "../core/lib/exec.h"

// Forward declare
extern pthread_mutex_t g_interp_lock;

//=========================================================
//  Thread Pool
//=========================================================

#define DEFAULT_POOL_SIZE 4
#define MAX_QUEUE_SIZE 256

typedef struct AsyncTask
{
   Data *func;
   List *args;
   Runtime *rt;
   Future *future;
} AsyncTask;

typedef struct
{
   AsyncTask *tasks[MAX_QUEUE_SIZE];
   int head;
   int tail;
   int count;
   pthread_mutex_t lock;
   pthread_cond_t has_work;
   pthread_cond_t has_space;
} TaskQueue;

typedef struct
{
   pthread_t *workers;
   int size;
   TaskQueue queue;
   int shutdown;
} ThreadPool;

static ThreadPool g_pool = {0};

static void queue_push(TaskQueue *q, AsyncTask *task)
{
   pthread_mutex_lock(&q->lock);
   while (q->count == MAX_QUEUE_SIZE)
      pthread_cond_wait(&q->has_space, &q->lock);
   q->tasks[q->tail] = task;
   q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
   q->count++;
   pthread_cond_signal(&q->has_work);
   pthread_mutex_unlock(&q->lock);
}

static AsyncTask *queue_pop(TaskQueue *q)
{
   pthread_mutex_lock(&q->lock);
   while (q->count == 0 && !g_pool.shutdown)
      pthread_cond_wait(&q->has_work, &q->lock);
   if (q->count == 0)
   {
      pthread_mutex_unlock(&q->lock);
      return NULL;
   }
   AsyncTask *task = q->tasks[q->head];
   q->head = (q->head + 1) % MAX_QUEUE_SIZE;
   q->count--;
   pthread_cond_signal(&q->has_space);
   pthread_mutex_unlock(&q->lock);
   return task;
}

//=========================================================
//  Worker
//=========================================================

static void *pool_worker(void *arg)
{
   (void)arg;

   while (1)
   {
      AsyncTask *task = queue_pop(&g_pool.queue);
      if (!task)
         break; // shutdown

      Future *f = task->future;

      // Clear error state for this task
      clear_error();

      // Run the function under the interpreter lock
      interp_lock();
      Data *result = executeFunction(task->func, task->args, task->rt);
      interp_unlock();

      // Store result in future
      pthread_mutex_lock(&f->lock);
      if (g_thrown.error)
      {
         f->error = 1;
         strncpy(f->error_msg, g_thrown.message, sizeof(f->error_msg) - 1);
         clear_error();
         f->result = createData(TYPE_NONE, NULL);
      }
      else
      {
         f->result = result ? result : createData(TYPE_NONE, NULL);
      }
      f->done = 1;
      pthread_cond_signal(&f->ready);
      pthread_mutex_unlock(&f->lock);

      // Cleanup task
      data_free(task->func);
      data_free(task->args ? createData(TYPE_LIST, task->args) : NULL);
      free(task);
   }

   return NULL;
}

//=========================================================
//  Public API
//=========================================================

void async_init(int pool_size)
{
   if (pool_size <= 0)
      pool_size = DEFAULT_POOL_SIZE;

   g_pool.size = pool_size;
   g_pool.shutdown = 0;
   g_pool.workers = malloc(sizeof(pthread_t) * pool_size);

   TaskQueue *q = &g_pool.queue;
   q->head = 0;
   q->tail = 0;
   q->count = 0;
   pthread_mutex_init(&q->lock, NULL);
   pthread_cond_init(&q->has_work, NULL);
   pthread_cond_init(&q->has_space, NULL);

   for (int i = 0; i < pool_size; i++)
      pthread_create(&g_pool.workers[i], NULL, pool_worker, NULL);
}

void async_shutdown(void)
{
   g_pool.shutdown = 1;

   // Wake all workers so they can exit
   pthread_mutex_lock(&g_pool.queue.lock);
   pthread_cond_broadcast(&g_pool.queue.has_work);
   pthread_mutex_unlock(&g_pool.queue.lock);

   for (int i = 0; i < g_pool.size; i++)
      pthread_join(g_pool.workers[i], NULL);

   free(g_pool.workers);
   pthread_mutex_destroy(&g_pool.queue.lock);
   pthread_cond_destroy(&g_pool.queue.has_work);
   pthread_cond_destroy(&g_pool.queue.has_space);
}

// Submit an async function call — returns a Future Data*
Data *async_submit(Data *func_data, List *args, Runtime *rt)
{
   if (!func_data || func_data->type != TYPE_FUNCTION)
   {
      throw_error(ERROR_TYPE,
                  "await: expression must be an async function call");
      return createData(TYPE_NONE, NULL);
   }

   Function *fn = FUNC_PTR(func_data);
   if (!fn->isAsync)
   {
      throw_error(ERROR_TYPE,
                  "await: '%s' is not an async function",
                  fn->name ? fn->name->str : "<anonymous>");
      return createData(TYPE_NONE, NULL);
   }

   // Create future
   Future *f = malloc(sizeof(Future));
   f->id = 0;
   f->result = NULL;
   f->done = 0;
   f->error = 0;
   memset(f->error_msg, 0, sizeof(f->error_msg));
   pthread_mutex_init(&f->lock, NULL);
   pthread_cond_init(&f->ready, NULL);

   // Build task
   AsyncTask *task = malloc(sizeof(AsyncTask));
   task->func = cloneData(func_data);
   task->args = args;
   task->rt = rt;
   task->future = f;

   // Wrap future in Data
   Data *future_data = createData(TYPE_FUTURE, f);
   // Submit to pool
   queue_push(&g_pool.queue, task);
   return future_data;
}

// Block until future resolves — returns result
Data *async_await(Data *future_data, Runtime *rt)
{
   (void)rt;

   if (!future_data || future_data->type != TYPE_FUTURE || !future_data->ref || !future_data->ref->object)
   {
      throw_error(ERROR_TYPE, "await: invalid future");
      return createData(TYPE_NONE, NULL);
   }

   Future *f = (Future *)future_data->ref->object;

   // Release interpreter lock while waiting so other threads can run
   interp_unlock();

   pthread_mutex_lock(&f->lock);
   while (!f->done)
      pthread_cond_wait(&f->ready, &f->lock);
   pthread_mutex_unlock(&f->lock);

   // Reacquire interpreter lock
   interp_lock();

   if (f->error)
   {
      throw_error(ERROR_RUNTIME, "AsyncError: %s", f->error_msg);
      return createData(TYPE_NONE, NULL);
   }

   return f->result ? cloneData(f->result) : createData(TYPE_NONE, NULL);
}

//=========================================================
//  executeAwait — called from exec.c for TYPE_AWAIT nodes
//=========================================================

Data *executeAwait(Await *aw, Runtime *rt)
{
   if (!aw || !aw->expr)
      return createData(TYPE_NONE, NULL);

   // The expr must be a function call (TYPE_INVOKED)
   // Execute it — if the function is async it returns a Future
   Data *result = executeAST(aw->expr, rt);

   if (!result || result->type != TYPE_FUTURE)
   {
      // Not a future — just return as-is (sync function called with await)
      return result ? result : createData(TYPE_NONE, NULL);
   }

   // It's a future — block until resolved
   Data *resolved = async_await(result, rt);
   data_free(result);
   return resolved;
}