#ifndef OBJECT_H
#define OBJECT_H

#include "structs.h"

obj_t *object_init(FILE *in, int objtype, float refraction_index);

int object_dump(FILE *out, obj_t *obj);

#endif
