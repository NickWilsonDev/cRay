#ifndef SPHERE_H
#define SPHERE_H

#include <stdio.h>
#include "structs.h"
#include "ray.h"
#include <ctype.h>
#include "object.h"
#include "material.h"

obj_t *sphere_init(FILE *in, int objtype, float refraction_index);

int sphere_dump(FILE *out, obj_t *obj);

double hits_sphere(double *base, double *dir, obj_t *obj);

void getamb_sphere(obj_t *obj, double *intensity);

void getdiff_sphere(obj_t *obj, double *intensity);

void getspec_sphere(obj_t *obj, double *intensity);

void getloc_sphere(obj_t *obj, double *location);

void getnorm_sphere(obj_t *obj, double *norm_vec);

void getbump_sphere(obj_t *obj, double *norm_vec);
#endif
