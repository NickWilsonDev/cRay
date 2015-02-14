#ifndef PSPHERE_H
#define PSPHERE_H
/*
 * psphere.h
 */
obj_t *psphere_init(FILE *in, int objtype, float refraction_index);

//double vec_get1(FILE *in, double *dndx);

void psphere0_amb(obj_t *obj, double *value);

void psphere1_amb(obj_t *obj, double *value);

void psphere2_amb(obj_t *obj, double *value);

#endif
