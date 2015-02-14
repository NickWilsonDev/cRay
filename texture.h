#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include "structs.h"
#include "ray.h"
#include "object.h"
#include "material.h"

int texture_load(texplane_t *tp);

int texture_map(fplane_t *fp, double *texel);

void texel_get(texture_t *tex, double xrel, double yrel, double texel[3]);
#endif
