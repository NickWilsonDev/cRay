#ifndef PPLANE_H
#define PPLANE_H
/*
 * pplane.h
 */
obj_t *pplane_init(FILE *in, int objtype, float refraction_index);

double vec_get1(FILE *in, double *dndx);

void pplane0_amb(obj_t *obj, double *value);

void pplane1_amb(obj_t *obj, double *value);

void pplane2_amb(obj_t *obj, double *value);

#endif
