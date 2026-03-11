#ifndef CSV_H
#define CSV_H

#include "build.h"

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

extern const ModuleFuncInfo CSV_MODULE[];
extern const int CSV_MODULE_COUNT;

typedef enum
{
    CSV_PARSE,
    CSV_STRINGIFY,
    CSV_READ,
    CSV_WRITE,
} CsvType;

void initializeCsvModule(Runtime *mod_rt);
Data *dispatchCsvBuiltin(CsvType type, List *args, Dict *kwargs, Runtime *rt);

#endif