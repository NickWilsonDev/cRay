#ifndef TEXSPHERE_H
#define TEXSPHERE_H

#include <stdio.h>
#include "structs.h"
#include "ray.h"
#include <ctype.h>
#include "object.h"
#include "material.h"
#include "ppmread.h"

obj_t *texsphere_init(FILE *in, int objtype, float refraction_index);

int texsphere_dump(FILE *out, obj_t *obj);

double hits_texsphere(double *base, double *dir, obj_t *obj);

void getamb_texsphere(obj_t *obj, double *intensity);

void getdiff_texsphere(obj_t *obj, double *intensity);

void getspec_texsphere(obj_t *obj, double *intensity);

void getloc_texsphere(obj_t *obj, double *location);

void getnorm_texsphere(obj_t *obj, double *norm_vec);

void getbump_texsphere(obj_t *obj, double *norm_vec);

PPMPixel *getpixel(int width, int height, PPMImage *image);

#endif
