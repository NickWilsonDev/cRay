#ifndef PLANE_H
#define PLANE_H

#include "structs.h"

obj_t *plane_init(FILE *in, int objtype, float refraction_index);

int plane_dump(FILE *out, obj_t *obj);

double hits_plane(double *base, double *dir, obj_t *obj);

void getamb_plane(obj_t *obj, double *intensity);

void getdiff_plane(obj_t *obj, double *intensity);

void getspec_plane(obj_t *obj, double *intensity);

void getloc_plane(obj_t *obj, double *location);

void getnorm_plane(obj_t *obj, double *normal);
#endif
