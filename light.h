#ifndef LIGHT_H
#define LIGHT_H

/*
 * light.h
 */
#include "structs.h"

obj_t *light_init(FILE *in, int objtype, float refraction_index);

int light_dump(FILE *out, obj_t *obj);

double hits_light(double *base, double *dir, obj_t *obj);

#endif
