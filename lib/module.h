#ifndef MODULE_H
#define MODULE_H

#include "type.h"

// int file_exists(const char *filename);
// char *get_module_path(const char *module_name);
// Runtime *load_module(const char *filepath, Runtime *parent_rt);
void executeImport(Import *import, Runtime *rt);

#endif // MODULE_H