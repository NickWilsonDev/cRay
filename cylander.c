#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ray.h"
#include "sphere.h"
#include "structs.h"
#include "malloc.h"
#include "veclib.h"
#include "cylander.h"

obj_t *cylander_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    cyl_t *cylander = NULL;
    obj = object_init(in, objtype, refraction_index); //was typecasting to obj_t * 
    cylander = (cyl_t*) Malloc(sizeof(cyl_t));

    /* void functions */
    obj->hits = hits_cylander;
    obj->getamb = getamb_cylander;
    obj->getdiff = getdiff_cylander;
    obj->getspec = getspec_cylander;
    obj->getnorm = getnorm_cylander;
    obj->getloc = getloc_cylander;
    obj->priv = cylander;

    char buf[MAXCHAR];

    scanf("%lf %lf %lf", &cylander->center[0], &cylander->center[1],
            &cylander->center[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    scanf("%lf %lf %lf", &cylander->centerline[0], &cylander->centerline[1],
            &cylander->centerline[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    scanf("%lf %lf", &cylander->radius, &cylander->height);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    cylander_dump(stderr, obj); //might need an rc variable

    return obj;
}

int cylander_dump(FILE *out, obj_t *obj) {
    cyl_t *cylander = obj->priv;
    int rc;

    fprintf(out, "Dumping object of type cylander\n");
    rc = material_dump(out, &obj->material); //this func print to stderr
    fprintf(out, "cylander data \n");
    fprintf(out, "center - \t%lf \t%lf \t%lf\n", cylander->center[0],
            cylander->center[1], cylander->center[2]);
    fprintf(out, "radius - \t%lf\n\n", cylander->radius);

    fprintf(out, "centerline - \t%lf \t%lf \t%lf\n", cylander->centerline[0],
            cylander->centerline[1], cylander->centerline[2]);
    fprintf(out, "height - \t%lf\n\n", cylander->height);

    return rc;
}

/*
 * hits_cylander - function provides a way of telling whether the cylander 
 * was hit by a ray or not.
 * 
 * Parameter: base - origin of the ray
 *            dir  - direction of the ray
 *            obj  - the object we hit
 *
 * Return : distance to the hit location from base
 */
double hits_cylander(double *base, double *dir, obj_t *obj) {
    cyl_t *cylander = obj->priv;
    int rotated = 0; // boolean set to false initially
    double th;
    
    /* unit vector in direction ray is traveling */
    double dir_ray[3];
    unitvec(dir, dir_ray);

    /* we will move the cylinder to origin to make math easier */
    /* make new base of ray */
    double newbase[3];
    diff3(base, cylander->center, newbase);
    //vecprn3("old base of ray ", base);
    //vecprn3("new base of ray ", newbase);

    /* make new center of cylander */
    double new_center[3];
    diff3(cylander->center, cylander->center, new_center);
    //vecprn3("new cylander center should be 0 0 0,,", new_center);

    /* The next step is to rotate the center_line so that it lies on the
     * y-axis, if it already does then there is no need to use rotation
     * matrix
     */
    
    if (cylander->centerline[0] != 0.0 && cylander->centerline[2] != 0.0) {
        rotated = 1; // set to true
        /* construct and populate rotational matrix */
        double yaxis[3] = {0, 1, 0};
        scale3(1, cylander->centerline, cylander->rotmat[1]); // was unitvec
        cross(cylander->centerline, yaxis, cylander->rotmat[0]);
        cross(cylander->rotmat[0], cylander->rotmat[1], cylander->rotmat[2]);
        transpose(cylander->rotmat, cylander->irot);

        /* apply rotation to newbase and direction vector */
        xform(cylander->rotmat, newbase, newbase);
        xform(cylander->rotmat, dir_ray, dir_ray);

    } 
    /* centerline of cylinder was on the pos y-axis */
    double a = (dir_ray[0] * dir_ray[0]) + (dir_ray[2] * dir_ray[2]);
    double b = 2 * ((newbase[0] * dir_ray[0]) + (newbase[2] * dir_ray[2]));
    double c = ((newbase[0] * newbase[0]) + (newbase[2] * newbase[2])) - 
                (cylander->radius * cylander->radius);
    //double a = dot3(dir_ray, dir_ray) - (dot3(dir_ray, unitvec(
    //            cylander->centerline, cylander->centerline)) * (dot3(dir_ray, cylander->centerline)));
    //double b = 2 * (dot3(dir_ray,
               
    
    double discriminate = (b * b) - (4 * a * c);

    fprintf(stderr, "a== %lf, b== %lf, c==%lf\n", a, b, c);
    fprintf(stderr, "discriminant== %lf\n", discriminate);
    if (discriminate <= 0) { 
        //fprintf(stderr, "discriminant was less than zero\n");
        return -1; }

    /* for now we will just take this quadratic answer, we may need the
     * otherone for refraction, maybe not though
     */
    fprintf(stderr, "positive\n");
    th = ((-1 * b) - sqrt(discriminate)) / (2 * a);
    double th2 = ((-1 * b) + sqrt(discriminate)) / (2 * a);
    fprintf(stderr, "th== %lf\n", th);
       
    fprintf(stderr, "th2== %lf\n", th2);
    if (th2 > th)
        th = th2;

    double hitpoint[3];
    scale3(th, dir_ray, hitpoint);
    /* this should be the solution to vector equation */

    /* check and see if value is within bounds of finite cylander */
    if (hitpoint[1] < 0)
        return -1;
    if (hitpoint[1] > cylander->height)
        return -1;
         
    //fprintf(stderr, "hit\n");
    // get normal at cylander for hitpoint
    double cnorm[3];
    cnorm[0] = hitpoint[0];
    cnorm[1] = 0.0;
    cnorm[2] = hitpoint[2];
    unitvec(cnorm, cnorm);

    if (rotated) {
        /* rotate and then translate hitpoint and normal vector back */
        xform(cylander->irot, hitpoint, obj->hitloc);
        xform(cylander->irot, cnorm, obj->normal);
        vecprn3("hitlocation after rotation back ", obj->hitloc);
    }
    sum(obj->hitloc, cylander->center, obj->hitloc);
    //fprintf(stderr, "th == %lf\n", th);
    th = th * -1;
    return th;
}

void getamb_cylander(obj_t *obj, double *intensity) {
    fprintf(stderr, "getamb cylander\n");
    intensity[0] = (obj->material.ambient[0]);
    intensity[1] = (obj->material.ambient[1]);
    intensity[2] = (obj->material.ambient[2]);
}

void getdiff_cylander(obj_t *obj, double *diffuse) { //may have to change intensity
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

void getspec_cylander(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.specular[0]);
    intensity[1] = (obj->material.specular[1]);
    intensity[2] = (obj->material.specular[2]);
}

void getloc_cylander(obj_t *obj, double *location) {
    cyl_t *temp = obj->priv;
    location[0] = temp->center[0];
    location[1] = temp->center[1];
    location[2] = temp->center[2];
}

/*
 * getnorm_cylander - Function computes the normal vector for a cylander at 
 *                    the hitpoint. Formula is as follows hitlocation - center
 *                    of cylander equals normal vector to cylander at 
 *                    hitlocation. 
 *
 * Parameters: obj - cylander object that 
 *                    will contain the data that is needed for calculating the
 *                    normal vector
 *             norm_vector - pointer to an array of doubles that will store the
 *                           the normal vectors components
 */
void getnorm_cylander(obj_t *obj, double *norm_vector) {
    norm_vector[0] = obj->normal[0];
    norm_vector[1] = obj->normal[1];
    norm_vector[2] = obj->normal[2];
   
}
