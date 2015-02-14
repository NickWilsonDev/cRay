#ifndef FPLANE_H
#define FPLANE_H

#include "structs.h"

obj_t *fplane_init(FILE *in, int objtype, float refraction_index);

int fplane_dump(FILE *out, obj_t *obj);

double fplane_hits(double *base, double *dir, obj_t *obj);

#endif
