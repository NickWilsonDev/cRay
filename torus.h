#ifndef TORUS_H
#define TORUS_H

#include "structs.h"
#include "ray.h"
#include "object.h"
#include "material.h"

obj_t *torus_init(FILE *in, int objtype, float refraction_index);

int torus_dump(FILE *out, obj_t *obj);

double hits_torus(double *base, double *dir, obj_t *obj);

void getamb_torus(obj_t *obj, double *intensity);

void getdiff_torus(obj_t *obj, double *intensity);

void getspec_torus(obj_t *obj, double *intensity);

void getnorm_torus(obj_t *obj, double *norm_vec);

void getbump_torus(obj_t *obj, double *norm_vec);
#endif
