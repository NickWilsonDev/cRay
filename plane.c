#include <stdio.h>
#include "plane.h"
#include "ray.h"
#include "structs.h"
#include "malloc.h"
#include "object.h"
#include "veclib.h"
#include "material.h"
#include "util.h"

/*
 * plane.c - file contains functions that model a infinite plane type object
 */

/*
 * plane_init - function initializes a plane object
 *
 * Parameters: in - pointer to a file that will be read from
 *             objtype - type int, signifies what type of object we are
 *                       dealing with
 *
 * Return: obj_t - the object that has been initialized
 */
obj_t *plane_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    plane_t *plane = NULL;

    obj = object_init(in, objtype, refraction_index);
    plane = Malloc(sizeof(plane_t));

    //void pointers to functions
    obj->hits = hits_plane;
    obj->getamb = getamb_plane;
    obj->getdiff = getdiff_plane;
    obj->getspec = getspec_plane;

    obj->getnorm = getnorm_plane;
    obj->getloc = getloc_plane;
    obj->priv = plane;
    
    get_3_doubles(&plane->normal[0], &plane->normal[1], &plane->normal[2]);
    get_3_doubles(&plane->point[0], &plane->point[1], &plane->point[2]);

    plane_dump(stderr, obj);
    fprintf(stderr, "plane_init() just before return\n");
    return obj;
}

/*
 * plane_dump - function prints out data about the plane object
 *
 * Parameter: out - pointer to a file we will write too
 *            obj - pointer to a obj_t structure that signifies the object we
 *                  are describing
 *
 * Return: int - signifies success or failure
 */
int plane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = obj->priv;
    int rc;

    fprintf(out, "Dumping object of type Plane\n");
    rc = material_dump(out, &obj->material); // print out reflectivities
    fprintf(out, "Plane data -\n");
    fprintf(out, "normal - \t%lf \t%lf \t%lf\n", plane->normal[0],
            plane->normal[1], plane->normal[2]);
    fprintf(out, "point - \t%lf \t%lf \t%lf\n\n", plane->point[0],
            plane->point[1], plane->point[2]);

    return rc;
}

/*
 * hits_plane - function determines whether or not the plane object has been
 *              hit by an array
 *
 * Parameters: base - origin of the ray
 *             dir  - direction of the ray
 *             obj  - the object that we are checking for a hit
 *
 * Returns: double
 */
double hits_plane(double *base, double *dir, obj_t *obj) {
    plane_t *plane = (plane_t *) obj->priv;
    int i;
    double NdotQ = dot3(plane->normal, plane->point);
    double NdotV = dot3(plane->normal, base);
    double NdotD = dot3(plane->normal, dir);

    if (NdotD == 0.0) { return -1; }

    double th = (NdotQ - NdotV) / (NdotD);

    if (th < 0.00001) { return -1; }

    double thD[3];
    scale3(th, dir, thD);

    double H[3];
    sum(base, thD, H);

    if (H[2] > 0.00001) { return -1; }

    for (i = 0; i < 3; i++) {
        obj->hitloc[i] = H[i];
    }
    return th;
}

/*
 * getamb_plane
 *
 */
void getamb_plane(obj_t *obj, double *intensity) {
    intensity[0] += (obj->material.ambient[0]);
    intensity[1] += (obj->material.ambient[1]);
    intensity[2] += (obj->material.ambient[2]);
}

/*
 * getdiff_plane
 *
 */
void getdiff_plane(obj_t *obj, double *diffuse) {
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

/*
 * getspec_plane
 *
 */
void getspec_plane(obj_t *obj, double *specular) {
    specular[0] = (obj->material.specular[0]);
    specular[1] = (obj->material.specular[1]);
    specular[2] = (obj->material.specular[2]);
}


void getloc_plane(obj_t *obj, double *location) {
    plane_t *temp = obj->priv;

    location[0] = temp->point[0];
    location[1] = temp->point[1];
    location[2] = temp->point[2];
}

void getnorm_plane(obj_t *obj, double *norm_vec) {
    plane_t *temp_plane = obj->priv;
    int i;
    for (i = 0; i < 3; i++) {
        norm_vec[i] = temp_plane->normal[i];
    }
}

