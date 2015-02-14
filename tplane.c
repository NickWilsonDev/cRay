#include <stdio.h>
#include "plane.h"
#include "ray.h"
#include "structs.h"
#include "malloc.h"
#include "object.h"
#include "veclib.h"
#include "material.h"
#include "fplane.h"
#include "tplane.h"
#include "util.h"

/*
 * author: Nick Wilson and Kao Thao
 * version: 4.26.13
 *
 * tplane.c - this file deals with modeling tiled planes
 *
 */

obj_t *tplane_init(FILE *in, int objtype, float refraction_index) {
    tplane_t *tp = NULL;
    plane_t *p = NULL;
    obj_t *obj = NULL;

    /* invoke "constructor" for "parent class" */
    obj = plane_init(in, objtype, refraction_index);

    /* Create the tplane_t object and point the plane_t to it */
    tp = Malloc(sizeof(tplane_t));

    p = obj->priv;

    p->priv = tp;

    /* override the default reflectivity functions */
    obj->getamb = tp_amb;
    obj->getdiff = tp_diff;
    obj->getspec = tp_spec;

    /* load xdir and size fields as done in fplane */
    get_3_doubles(&tp->xdir[0], &tp->xdir[1], &tp->xdir[2]);
    get_2_doubles(&tp->size[0], &tp->size[1]);

    /* finally load the background material reflectivity */
    // need to call material load here
    material_init(in, &tp->background);

    // project xdir onto infinite plane
    unitvec(tp->xdir, tp->xdir); // converted to unit vector
    unitvec(p->normal, p->normal);
    project(p->normal, tp->xdir, tp->xdir);

    // compute and populate rotational matrix
    unitvec(tp->xdir, tp->rotmat[0]);
    unitvec(p->normal, tp->rotmat[2]);
    cross(tp->rotmat[2], tp->rotmat[0], tp->rotmat[1]);

    tplane_dump(stderr, obj);
    return(obj);
}

/*
 * tplane_dump
 *
 */
int tplane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = obj->priv;
    tplane_t *pln = plane->priv;
    int rc;


    fprintf(stderr, "inside tplane_dump()\n");
    fprintf(out, "Dumping object of type Tiled Plane\n");
    rc = material_dump(out, &obj->material); // this prints out reflectivity
    fprintf(out, "Tiled Plane data - \n");
    fprintf(out, "xdir - \t%lf \t%lf \t%lf\n", pln->xdir[0], pln->xdir[1], pln->xdir[2]);
    fprintf(out, "size - \t%lf \t%lf\n", pln->size[0], pln->size[1]);

    return rc;
}

/*
 * tp_diff - function returns tiled planes diff value
 *
 */
void tp_diff(obj_t *obj, double *value) {
    plane_t *pln = (plane_t *)obj->priv;
    tplane_t *tp = (tplane_t *)pln->priv;

    if (tp_select(obj))
        vec_copy3(obj->material.diffuse, value);
    else
        vec_copy3(tp->background.diffuse, value);
}

/*
 * tp_amb - function returns tiled planes ambient value
 *
 */
void tp_amb(obj_t *obj, double *value) {
    plane_t *pln = (plane_t *)obj->priv;
    tplane_t *tp = (tplane_t *)pln->priv;

    if (tp_select(obj))
        vec_copy3(obj->material.ambient, value);
    else
        vec_copy3(tp->background.ambient, value);
}

/*
 * tp_spec - function returns tiled planes specular value
 *
 */
void tp_spec(obj_t *obj, double *value) {
    plane_t *pln = (plane_t *)obj->priv;
    tplane_t *tp = (tplane_t *)pln->priv;

    if (tp_select(obj))
        vec_copy3(obj->material.specular, value);
    else
        vec_copy3(tp->background.specular, value);
}

/*
 * tp_select - function determines whether a foreground tile or a background
 * tile was hit.
 *
 */
int tp_select(obj_t *obj) {
    int relx;
    int rely;
    int temp;
    double newhit[3];
    plane_t *pln = (plane_t *)obj->priv;
    tplane_t *tp = (tplane_t *)pln->priv;

    //shift location of tiles(that act like finite planes) to make calculations
    //easier
    diff3(pln->point, obj->hitloc, newhit);

    //construct rotation matrix, that will rotate plane normal into z-axis and
    // the xdir vector into the x-axis
    double what[2];
    xform(tp->rotmat, newhit, what);
    //vecprn3("what", what);

    // in order to fill "holes" nasty hack? 
    relx = (int)(10000 + what[0] / tp->size[0]);
    rely = (int)(10000 + what[1] / tp->size[1]);

    temp = (relx + rely);

    //fprintf(stderr, "relx = %d   rely = %d \n", relx, rely);

    if ((temp % 2) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}
