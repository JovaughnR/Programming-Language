#ifndef ASYNC_H
#define ASYNC_H

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

// Initialize the thread pool (call once at startup)
void async_init(int pool_size);

// Shutdown the thread pool (call at exit)
void async_shutdown(void);

// Submit an async function call — returns a Future Data*
Data *async_submit(Data *func_data, List *args, Runtime *rt);

// Block until future is resolved — returns result
Data *async_await(Data *future_data, Runtime *rt);

// Execute a TYPE_AWAIT node
Data *executeAwait(Await *aw, Runtime *rt);

extern Data *executeFunction(Data *func_data, List *args, Runtime *rt);

#endif