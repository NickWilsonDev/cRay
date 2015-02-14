#ifndef CYLANDER_H
#define CYLANDER_H

#include <stdio.h>
#include "structs.h"
#include "ray.h"
#include <ctype.h>
#include "object.h"
#include "material.h"


obj_t *cylander_init(FILE *in, int objtype, float refraction_index);

int cylander_dump(FILE *out, obj_t *obj);

double hits_cylander(double *base, double *dir, obj_t *obj);

void getamb_cylander(obj_t *obj, double *intensity);

void getdiff_cylander(obj_t *obj, double *diffuse);

void getspec_cylander(obj_t *obj, double *intensity);

void getloc_cylander(obj_t *obj, double *location);

void getnorm_cylander(obj_t *obj, double *norm_vector);

#endif
