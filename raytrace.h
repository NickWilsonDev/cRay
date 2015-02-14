#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "structs.h"

/* Object types */

#define FIRST_TYPE   10
#define LIGHT        10
#define SPOTLIGHT    11
#define PROJECTOR    12
#define SPHERE       13
#define PLANE        14
#define FINITE_PLANE 15
#define TILED_PLANE  16
#define TEX_PLANE    17
#define REF_SPHERE   18
#define P_SPHERE     19
#define P_PLANE      20
#define PARABOLOID   21
#define CYLANDER     22
#define CONE         23
#define HYPERBOLOID  24
#define TORUS        25
#define TEX_SPHERE   26
#define TRIANGLE     27
#define DISP_SPHERE  28
#define LAST_TYPE    29
//#define LAST_LIGHT   12

#define TEX_FIT       1
#define TEX_TILE      2

#define True          1
#define False         0

#define MAX_INT       100
#define MAX_COLOR     255
#define MAX_DIST      200

obj_t *find_closest_obj(list_t *list, double *viewer, double *unit_dir, 
                        obj_t *obj, double *min_dist);

void ray_trace(model_t *model, double base[3], double dir[3], 
                double intensity[3], double total_dist, obj_t *last_hit);

#endif
