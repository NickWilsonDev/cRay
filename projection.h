#ifndef PROJECTION_H
#define PROJECTION_H

#include "structs.h"

proj_t *projection_init(int argc, char **argv, FILE *in);

int projection_dump(FILE *out, proj_t *proj);

void map_pix_to_world(proj_t *proj, int x, int y, double *world);

void map_world_to_pix(double *lasthit, double *pixhit);

double randpix(int x);
#endif
