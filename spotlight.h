#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "structs.h"

obj_t *spotlight_init(FILE *in, int objtype, float refraction_index);

int spot_visible(obj_t *lobj, double *hitloc);

#endif
