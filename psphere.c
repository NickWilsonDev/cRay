#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "sphere.h"
#include "veclib.h"
#include "ray.h"
#include "structs.h"
#include "psphere.h"
#include "pplane.h"

/*
 * psphere.c
 * author Nick Wilson and Kao Thao
 * version 4.7.13
 *
 * This file contains the functions that deal with procedural procedural 
 * spheres.
 */

/* table of function pointers */
static void (*sphere_shaders[])(obj_t *obj, double *intensity) =
{
    psphere0_amb,
    psphere1_amb,
    psphere2_amb
};
#define NUM_SHADERS sizeof(sphere_shaders)/sizeof(void *)

/*
 * psphere_init - This function invokes the sphere_init function to construct 
 * the object and then overrides the default getamb() function, replaceing it
 * with the shader function whose index is provided in the model description
 * files.
 *
 * Parameters: *in - pointer to a file
 *              objtype - an int representing the type of object we are 
 *                          dealing with
 *
 * Return:
 *      obj_t
 */
obj_t *psphere_init(FILE *in, int objtype, float refraction_index) {
    char buf[MAXCHAR];
    obj_t *new;
    double *dndx;// = 2;
    double  tempdouble = 0;
    int ndx = 0;

    dndx = &tempdouble;

    new = (obj_t *)sphere_init(in, objtype, refraction_index);

    fgets(buf, MAXCHAR, in);
    ndx = (int)vec_get1(in, dndx);

    fprintf(stderr, "shader index is %d\n", ndx);

    fgets(buf, MAXCHAR, in);

    if (ndx != 1)
        return(0);

    ndx = (int) *dndx;
    if (ndx >= NUM_SHADERS)
        return(0);

    new->getamb = sphere_shaders[ndx];
    return(new);
}

/*
 * vec_get1
 */


/*
 * pplane0_amb
 *
 * unknown for right now it will make no change to anything
 */
void psphere0_amb(obj_t *obj, double *value) {
    if (value[2] > 0  && value[0] < 10) {
        value[0] = value[0] + 20;
        value[1] = value[1];
        value[2] = value[2] + 30;
    } else {
        value[0] = value[0];
        value[1] = value[1];
        value[2] = value[2];
    }
}


/*
 * psphere1_amb
 *
 * swirls on a sphere?
 */
void psphere1_amb(obj_t *obj, double *value) {
    double vec[3];
    sphere_t *p = (sphere_t *)(obj->priv);
    int isum;
    double sum;
    vec_copy3(obj->material.ambient, value);
    diff3(p->center, obj->hitloc, vec);
    sum = 1000 + vec[0] * vec[1] * vec[1] / 100 + vec[0] * vec[1] / 100;

    isum = sum;
    if (isum & 1)
        value[0] = 0;	// zap red
    else
        value[2] = 0;	// zap blue
}

/*
 * psphere2_amb
 *
 * continously  modulating shading
 */
void psphere2_amb(obj_t *obj, double *value) {
    double vec[3];
    double v1;
    double t1;

    sphere_t *p = (sphere_t *)(obj->priv);

    vec_copy3(obj->material.ambient, value);
    diff3(p->center, obj->hitloc, vec);

    v1 = (vec[0] / sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
    t1 = acos(v1);

    if (vec[1] < 0)		//acos() returns values in [0,PI]
        t1 = 2 * M_PI - t1; 	// extend to [0, 2PI] here

    value[0] *= (1 + cos(2 * t1));
    value[1] *= (1 + cos(2 * t1 + 2 * M_PI / 3));
    value[2] *= (1 + cos(2 * t1 + 4 * M_PI / 3));
}
