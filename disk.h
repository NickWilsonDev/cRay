#ifndef DISK_H
#define DISK_H

#include "structs.h"
#include "ray.h"
#include "object.h"
#include "material.h"

obj_t *disk_init(FILE *in, int objtype, float refraction_index);

int disk_dump(FILE *out, obj_t *obj);

double hits_disk(double *base, double *dir, obj_t *obj);

void getamb_disk(obj_t *obj, double *intensity);

void getdiff_disk(obj_t *obj, double *intensity);

void getspec_disk(obj_t *obj, double *intensity);
#endif
