#ifndef DB_H
#define DB_H

#include "build.h"

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

extern const ModuleFuncInfo DB_MODULE[];
extern const int DB_MODULE_COUNT;

typedef enum
{
   DB_OPEN,
   DB_CLOSE,
   DB_EXEC,
   DB_QUERY,
   DB_PREPARE,
   DB_BIND,
   DB_STEP,
   DB_FINALIZE,
   DB_LASTID,
   DB_CHANGES,
   DB_BEGIN,
   DB_COMMIT,
   DB_ROLLBACK,
   DB_TABLES,
   DB_SCHEMA,
} DbType;

void initializeDbModule(Runtime *mod_rt);
Data *dispatchDbBuiltin(DbType type, List *args, Dict *kwargs, Runtime *rt);

#endif