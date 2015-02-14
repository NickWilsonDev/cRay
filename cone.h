#ifndef CONE_H
#define CONE_H

#include <stdio.h>
#include "structs.h"
#include "ray.h"
#include "object.h"
#include "material.h"


obj_t *cone_init(FILE *in, int objtype, float refraction_index);

int cone_dump(FILE *out, obj_t *obj);

double hits_cone(double *base, double *dir, obj_t *obj);

void getamb_cone(obj_t *obj, double *intensity);

void getdiff_cone(obj_t *obj, double *diffuse);

void getspec_cone(obj_t *obj, double *intensity);

void getnorm_cone(obj_t *obj, double *norm_vector);

#endif
