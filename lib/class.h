#ifndef CLASS_H
#define CLASS_H

#include "type.h"

Instance *extract_attribute_name(ASTnode *attrAst);
Instance *lookup_in_mro(Class *cls, Instance *name);
void defineClass(Class *cls, Runtime *rt);
Instance *get_instance_attr(Instance *inst, Instance *name);
// Helper function to compute MRO
List *computeMRO(Class *cls, Runtime *parentRt);
void setInstanceAttribute(Instance *instance, Instance *attrName, Data *value);

#endif // CLASS_H