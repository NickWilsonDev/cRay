#include <stdio.h>
#include "plane.h"
#include "structs.h"
#include "malloc.h"
#include "object.h"
#include "veclib.h"
#include "material.h"
#include "ray.h"
#include "fplane.h"

/*
 * fplane.c - This file deals with finite planes
 */

/*
 * fplane_init() - function initializes the finite plane object
 *
 * Parameter: in - pointer to a file
 *            objtype - int representing type of object
 *
 * Return obj_t
 */
obj_t *fplane_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    plane_t *plane = NULL;
    fplane_t *finiteplane = NULL;
    //plane = Malloc(sizeof(plane_t));

    obj = plane_init(in, objtype, refraction_index);
    plane = obj->priv;
    finiteplane = Malloc(sizeof(fplane_t)); //may need to change this to our malloc
    finiteplane->lasthit[0] = 0;
    finiteplane->lasthit[1] = 0;

    obj->hits = fplane_hits;

    plane->priv = finiteplane;
    char buf[MAXCHAR];

    fprintf(stderr, "inside fplane_init()\n");
    //read in x direction and size
    fgets(buf, MAXCHAR, in);
    scanf("%lf %lf %lf", &finiteplane->xdir[0], &finiteplane->xdir[1], &finiteplane->xdir[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts
    scanf("%lf %lf", &finiteplane->size[0], &finiteplane->size[1]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    // project xdir onto infinite plane
    double temp[3];
    unitvec(finiteplane->xdir, temp); // converted to unit vector
    vec_copy3(temp, finiteplane->xdir);
    unitvec(plane->normal, temp);
    vec_copy3(temp, plane->normal);
    project(plane->normal, finiteplane->xdir, finiteplane->xdir);



    // compute and populate rotational matrix
    unitvec(finiteplane->xdir, finiteplane->rotmat[0]);
    unitvec(plane->normal, finiteplane->rotmat[2]);
    vecprn3("normal after unitvec ", plane->normal);
    vecprn3("xdir after unitvec", finiteplane->xdir);

    cross(finiteplane->rotmat[2], finiteplane->rotmat[0], finiteplane->rotmat[1]);
    matprn("rotation matrix ", finiteplane->rotmat);

    //fplane_dump(stderr, obj);

    return obj;
}

/*
 * fplane_dump() - function prints out the data for the finite plane
 *
 * Parameters: out - pointer to a file
 *             obj - pointer to an obj_t structure
 *
 * Return int - signifying success or failure
 */
int fplane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = obj->priv;
    fplane_t *pln = plane->priv;
    int rc; 
    
    fprintf(stderr,"inside fplane_dump()\n");
    fprintf(out, "Dumping object of type Finite Plane\n");
    rc = material_dump(out, &obj->material);//this func print out reflectivities
    fprintf(out, "Finite Plane data -\n");
    fprintf(out, "xdir - \t%lf \t%lf \t%lf\n", pln->xdir[0], pln->xdir[1],
            pln->xdir[2]);
    fprintf(out, "size - \t%lf \t%lf\n", pln->size[0], pln->size[1]);

    return rc;
}

/*
 * fplane_hits() - function finds out if the finite plane has been hit
 *
 * Parameter: base - pointer to an array of doubles, origin position of the 
 *                   ray
 *            dir  - pointer to an array of doubles, direction of the ray
 *            obj  - pointer to an obj_t struct
 *
 * Return: double - signifies whether finite plane was hit or not -1 means 
 *                  miss
 */
double fplane_hits(double *base, double *dir, obj_t *obj) {
    double t;
    double newhit[3];

    t = hits_plane(base, dir, obj);
    if (t < 0.00001)
        return(t);

    plane_t *pln = obj->priv;
    fplane_t *finiteplane = pln->priv;
    
    fprintf(stderr, "fplane_hits lasthit== %lf, %lf\n", finiteplane->lasthit[0], finiteplane->lasthit[1]);
    //this is where we shift location of finite plane to make calculations
    //easier
    //vecprn3("original hit point", obj->hitloc);

    diff3(pln->point, obj->hitloc, newhit);
    xform(finiteplane->rotmat, newhit, newhit);

    //matprn("rotation matrix", plane->rotmat);
    //vecprn3("new hit point ", newhit);
    if ((newhit[0] > finiteplane->size[0]) || (newhit[0] < 0.0))
        return(-1);

    if ((newhit[1] > finiteplane->size[1]) || (newhit[1] < 0.0))
        return(-1);

    finiteplane->lasthit[0] = newhit[0];
    finiteplane->lasthit[1] = newhit[1];
    
    fprintf(stderr, "fplane_hits lasthit== %lf, %lf\n", finiteplane->lasthit[0], finiteplane->lasthit[1]);
    
    return t;
}

