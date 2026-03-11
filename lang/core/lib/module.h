#ifndef MODULE_H
#define MODULE_H

#include "type.h"

void register_native(ModuleRegistry *reg, char *name, void (*init)(Runtime *));
void executeImport(Import *import, Runtime *rt);
#endif // MODULE_H