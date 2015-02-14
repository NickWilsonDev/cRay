#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ray.h"
#include "plane.h"
#include "veclib.h"
#include "material.h"
#include "structs.h"
#include "pplane.h"
/*
 * pplane.c
 * author Nick Wilson and Kao Thao
 * version 4.7.13
 *
 * This file contains the functions that deal with procedural planes.
 */

/* table of function pointers */
static void (*plane_shaders[])(obj_t *obj, double *intensity) =
{
    pplane0_amb,
    pplane1_amb,
    pplane2_amb
};
#define NUM_SHADERS sizeof(plane_shaders)/sizeof(void *)

/*
 * pplane_init - This function invokes the plane_init function to construct 
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
obj_t *pplane_init(FILE *in, int objtype, float refraction_index) {
    char buf[MAXCHAR];
    obj_t *new;
    double *dndx;// = 2;
    double  tempdouble = 0;
    int ndx = 0;

    dndx = &tempdouble;
    new = (obj_t *)plane_init(in, objtype, refraction_index);
    fgets(buf, MAXCHAR, in);
    ndx = (int)vec_get1(in, dndx);
    fgets(buf, MAXCHAR, in);

    if (ndx != 1)
        return(0);

    ndx = (int) *dndx;
    if (ndx >= NUM_SHADERS)
        return(0);

    new->getamb = plane_shaders[ndx];
    return(new);
}

/*
 * vec_get1
 */
double vec_get1(FILE *in, double *dndx) {
    return (scanf("%lf", dndx));
}

/*
 * pplane0_amb
 *
 * unknown for right now it will make no change to anything
 */
void pplane0_amb(obj_t *obj, double *value) {
    
    if ((value[0] / 2.0) >= 0) {
        value[0] = value[0] + 5;
        value[1] = value[1] + 20;
        value[2] = value[2];
    } else {
        value[0] = value[0];
        value[1] = value[1];
        value[2] = value[2];
    }
}

/*
 * pplane1_amb
 *
 * swirls on a plane?
 */
void pplane1_amb(obj_t *obj, double *value) {
    double vec[3];
    plane_t *p = (plane_t *)(obj->priv);
    int isum;
    double sum;
    vec_copy3(obj->material.ambient, value);
    diff3(p->point, obj->hitloc, vec);
    sum = 1000 + vec[0] * vec[1] * vec[1] / 100 + vec[0] * vec[1] / 100;

    isum = sum;
    if (isum & 1)
        value[0] = 0; // zap red
    else
        value[2] = 0; // zap blue
}

/*
 * pplane2_amb
 *
 * continously  modulating shading
 */
void pplane2_amb(obj_t *obj, double *value) {
    double vec[3];
    double v1;
    double t1;

    plane_t *p = (plane_t *)(obj->priv);

    vec_copy3(obj->material.ambient, value);
    diff3(p->point, obj->hitloc, vec);

    v1 = (vec[0] / sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
    t1 = acos(v1);

    if (vec[1] < 0) //acos() returns values in [0,PI]
        t1 = 2 * M_PI - t1; // extend to [0, 2PI] here

    value[0] *= (1 + cos(2 * t1));
    value[1] *= (1 + cos(2 * t1 + 2 * M_PI / 3));
    value[2] *= (1 + cos(2 * t1 + 4 * M_PI / 3));
}
