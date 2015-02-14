#include <stdio.h>
#include "structs.h"
#include "spotlight.h"
#include "object.h"
#include "ray.h"
#include "light.h"
#include "material.h"
#include "veclib.h"
#include <math.h>
#include "malloc.h"

/*
 * spotlight.c - this file contains functions that model spotlights
 * author Nick Wilson and Kao Thao
 * version 4.27.13
 *
 */

/*
 * spotlight_init() - function loads spotlight data
 *
 */
obj_t *spotlight_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj;
    light_t *light;
    spotlight_t *spot;
    int pcount = 0;
    double hits[3];

    obj = light_init(in, objtype, refraction_index);
    if (obj == NULL)
        return(obj);

    light = (light_t *)obj->priv;
    spot = Malloc(sizeof(spotlight_t));

    light->priv = (spotlight_t *)spot;
    char buf[MAXCHAR];

    // read in double[3] vector that models point on floor that the centerline
    // hits , and also the cone's half-width theta in degrees

    pcount = scanf("%lf %lf %lf", &hits[0], &hits[1], &hits[2]);

    fprintf(stderr, "pcount in spotlight.c after assignment to hits == %d\n",
            pcount);
    diff3(light->location, hits, spot->direction);
    fgets(buf, MAXCHAR, in); // consume remaining texts
    pcount += scanf("%lf", &spot->theta);
    fgets(buf, MAXCHAR, in);

    fprintf(stderr, "pcount in spotlight.c == %d\n", pcount);
    // quick check to see whether enough doubles were loaded
    if (pcount != 4)
        return(0);

    // compute cos(theta) and save in spotlight structure
    // remember that cos() wants radians not degrees, so convert
    double temp = (spot->theta * M_PI) / 180;
    spot->costheta = cos(temp);
    obj->illum_check = spot_visible;

    return(obj);
}

/*
 * spot_visible 
 */
int spot_visible(obj_t *lobj, double *hitloc) {
    double dir_to_light[3];
    light_t *light = lobj->priv;
    spotlight_t *spot = light->priv;

    diff3(light->location, hitloc, dir_to_light);
    unitvec(dir_to_light, dir_to_light);

    // need to compute center line from center of spotlight
    double center_line[3];
    unitvec(spot->direction, center_line);

    // if this test is passed then hitloc is illuminated
    if (dot3(dir_to_light, center_line) > (spot->costheta)) {
        return 1;
    }

    return 0;
}

