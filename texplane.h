#ifndef TEXPLANE_H
#define TEXPLANE_H
#include "structs.h"

/*
 * author Nick Wilson
 * version 7.20.13
 *
 * texplane.h - this file contains function prototypes that are used by the 
 *              texplane.c file
 */
obj_t *texplane_init(FILE *in, int objtype, float refraction_index);

/* these may not be correctly defined yet */
int texplane_dump(FILE *out, obj_t *obj);

void texplane_amb(obj_t *obj, double *intensity);
#endif
