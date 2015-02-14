#ifndef DISPSPHERE_H
#define DISPSPHERE_H

#include <stdio.h>
#include "structs.h"
#include "ray.h"
#include <ctype.h>
#include "object.h"
#include "material.h"
#include "ppmread.h"

obj_t *dispsphere_init(FILE *in, int objtype, float refraction_index);

int dispsphere_dump(FILE *out, obj_t *obj);

double hits_dispsphere(double *base, double *dir, obj_t *obj);

PPMPixel get_pixel_from_map(dispsphere_t *sphere, double *hitloc);

void getamb_dispsphere(obj_t *obj, double *intensity);

void getdiff_dispsphere(obj_t *obj, double *intensity);

void getspec_dispsphere(obj_t *obj, double *intensity);

void getloc_dispsphere(obj_t *obj, double *location);

void getnorm_dispsphere(obj_t *obj, double *norm_vec);

void getbump_dispsphere(obj_t *obj, double *norm_vec);

//PPMPixel *getpixel(int width, int height, PPMImage *image);

#endif
