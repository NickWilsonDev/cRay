#ifndef MODEL_H
#define MODEL_H

#include "structs.h"
#include "list.h"
#include "plane.h"
#include "sphere.h"
#include "light.h"
#include "fplane.h"

int model_init(FILE *in, model_t *model);

int model_dump(FILE *out, model_t *model);

#endif
