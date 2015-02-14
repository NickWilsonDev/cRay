#ifndef MATERIAL_H
#define MATERIAL_H 

#include <stdio.h>
#include "structs.h"
#include "ray.h"
#include <ctype.h>

int material_init(FILE *in, material_t *mat);

int material_dump(FILE *out, material_t *mat);

#endif
