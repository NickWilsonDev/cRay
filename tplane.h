#ifndef TPLANE_H
#define TPLANE_H

#include "structs.h"

obj_t *tplane_init(FILE *in, int objtype, float refraction_index);

int tplane_dump(FILE *out, obj_t *obj);

void tp_diff(obj_t *obj, double *value);

void tp_amb(obj_t *obj, double *value);

void tp_spec(obj_t *obj, double *value);

int tp_select(obj_t *obj);
#endif
