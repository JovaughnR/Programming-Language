#ifndef IO_H
#define IO_H

#include "build.h"

#include "../../core/lib/type.h"
#include "../../core/lib/list.h"
#include "../../core/lib/dict.h"

extern const ModuleFuncInfo IO_MODULE[];
extern const int IO_MODULE_COUNT;

typedef enum
{
   IO_OPEN,
   IO_CLOSE,
   IO_READ,
   IO_READLINE,
   IO_READLINES,
   IO_WRITE,
   IO_WRITELINE,
   IO_WRITELINES,
   IO_SEEK,
   IO_TELL,
   IO_FLUSH,
   IO_EXISTS,
} IoType;

void initializeIoModule(Runtime *mod_rt);
Data *dispatchIoBuiltin(IoType type, List *args, Dict *kwargs, Runtime *rt);

#endif