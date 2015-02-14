#ifndef ILLUMINATE_H
#define ILLUMINATE_H

#include "structs.h"

/*
 * illuminate.h
 */

//void ray_trace(list_t *objs, double *base, double *dir, double *pix, 
//                  double total_dist);

void diffuse_illumination(model_t *model, obj_t *hitobj, double *intensity);

int process_light(list_t *lst, obj_t *hitobj, obj_t *lobj, double *ivec);

#endif
