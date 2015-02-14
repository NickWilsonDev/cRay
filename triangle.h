#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "structs.h"

obj_t *triangle_init(FILE *in, int objtype, float refraction_index);

int triangle_dump(FILE *out, obj_t *obj);

double triangle_hits(double *base, double *dir, obj_t *obj);

#endif
