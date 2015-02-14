#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ray.h"
#include "sphere.h"
#include "structs.h"
#include "malloc.h"
#include "veclib.h"
#include "cone.h"

obj_t *cone_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    cone_t *cone = NULL;
    obj = object_init(in, objtype, refraction_index); //was typecasting to obj_t * 
    cone = (cone_t*) Malloc(sizeof(cone_t));

    /* void functions */
    obj->hits = hits_cone;
    obj->getamb = getamb_cone;
    obj->getdiff = getdiff_cone;
    obj->getspec = getspec_cone;
    obj->getnorm = getnorm_cone;
    obj->priv = cone;

    char buf[MAXCHAR];

    scanf("%lf %lf %lf", &cone->center[0], &cone->center[1],
            &cone->center[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    scanf("%lf %lf %lf", &cone->centerline[0], &cone->centerline[1],
            &cone->centerline[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    scanf("%lf %lf", &cone->radius, &cone->height);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    cone->k = (cone->radius / cone->height) * (cone->radius / cone->height);

    cone_dump(stderr, obj); //might need an rc variable

    return obj;
}

int cone_dump(FILE *out, obj_t *obj) {
    cone_t *cone = obj->priv;
    int rc;

    fprintf(out, "Dumping object of type cone\n");
    rc = material_dump(out, &obj->material); //this func print to stderr
    fprintf(out, "cone data \n");
    fprintf(out, "center - \t%lf \t%lf \t%lf\n", cone->center[0],
            cone->center[1], cone->center[2]);


    fprintf(out, "centerline - \t%lf \t%lf \t%lf\n", cone->centerline[0],
            cone->centerline[1], cone->centerline[2]);

    fprintf(out, "radius - \t%lf\n\n", cone->radius);
    fprintf(out, "height - \t%lf\n\n", cone->height);

    return rc;
}

/*
 * hits_cone - function provides a way of telling whether the cone 
 * was hit by a ray or not.
 * 
 * Parameter: base - origin of the ray
 *            dir  - direction of the ray
 *            obj  - the object we hit
 *
 * Return : distance to the hit location from base
 */
double hits_cone(double *base, double *dir, obj_t *obj) {
    cone_t *cone = obj->priv;
    int rotated = 0; // boolean set to false initially
    double th;
    
    /* unit vector in direction ray is traveling */
    double dir_ray[3];
    unitvec(dir, dir_ray);

    /* we will move the cylinder to origin to make math easier */
    /* make new base of ray */
    double newbase[3];
    diff3(base, cone->center, newbase);
    //vecprn3("old base of ray ", base);
    //vecprn3("new base of ray ", newbase);

    /* make new center of cone */
    double new_center[3];
    diff3(cone->center, cone->center, new_center);
    //vecprn3("new cone center should be 0 0 0,,", new_center);

    /* The next step is to rotate the center_line so that it lies on the
     * y-axis, if it already does then there is no need to use rotation
     * matrix
     */
    
    if (cone->centerline[0] != 0.0 && cone->centerline[2] != 0.0) {
        rotated = 1; // set to true
        /* construct and populate rotational matrix */
        double yaxis[3] = {0, 1, 0};
        scale3(1, cone->centerline, cone->rotmat[1]); // was unitvec
        cross(cone->centerline, yaxis, cone->rotmat[0]);
        cross(cone->rotmat[0], cone->rotmat[1], cone->rotmat[2]);
        transpose(cone->rotmat, cone->irot);

        /* apply rotation to newbase and direction vector */
        xform(cone->rotmat, newbase, newbase);
        xform(cone->rotmat, dir_ray, dir_ray);

    } 
    /* centerline of cylinder was on the pos y-axis */
    double a = (dir_ray[0] * dir_ray[0]) + (dir_ray[2] * dir_ray[2]);
    a = a - (cone->k * (dir_ray[1] * dir_ray[1]));

    double b = (newbase[0] * dir_ray[0]) + (newbase[2] * dir_ray[2]);
    b = 2 * (b - cone->k * dir_ray[1] * newbase[1]);
    double c = ((newbase[0] * newbase[0]) + (newbase[2] * newbase[2]));
    c = c - (cone->k * newbase[1] * newbase[1]);
               
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

    /* check and see if value is within bounds of finite cone */
    if (hitpoint[1] < 0.0001)
        return -1;
    if (hitpoint[1] > cone->height )
        return -1;
         
    //fprintf(stderr, "hit\n");
    // get normal at cone for hitpoint
    double cnorm[3];
    cnorm[0] = 2 * hitpoint[0];
    cnorm[1] = -2 * cone->k * hitpoint[1];
    cnorm[2] = 2 * hitpoint[2];
    unitvec(cnorm, cnorm);

    if (rotated) {
        /* rotate and then translate hitpoint and normal vector back */
        if (cone->centerline[1] > 0.0) {
            xform(cone->irot, hitpoint, obj->hitloc);
            xform(cone->irot, cnorm, obj->normal);
            vecprn3("hitlocation after rotation back ", obj->hitloc);
        } else {
            double yaxis[3] = {0, -1, 0};
            scale3(1, cone->centerline, cone->rotmat[1]); // was unitvec
            cross(cone->centerline, yaxis, cone->rotmat[0]);
            cross(cone->rotmat[0], cone->rotmat[1], cone->rotmat[2]);
            xform(cone->rotmat, hitpoint, obj->hitloc);
            xform(cone->rotmat, cnorm, obj->normal);
        }
    }
    sum(obj->hitloc, cone->center, obj->hitloc);
    //fprintf(stderr, "th == %lf\n", th);
    th = th * -1;
    return th;
}

void getamb_cone(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.ambient[0]);
    intensity[1] = (obj->material.ambient[1]);
    intensity[2] = (obj->material.ambient[2]);
}

void getdiff_cone(obj_t *obj, double *diffuse) { //may have to change intensity
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

void getspec_cone(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.specular[0]);
    intensity[1] = (obj->material.specular[1]);
    intensity[2] = (obj->material.specular[2]);
}

/*
 * getnorm_cone - Function computes the normal vector for a cone at 
 *                    the hitpoint. Formula is as follows hitlocation - center
 *                    of cone equals normal vector to cone at 
 *                    hitlocation. 
 *
 * Parameters: obj - cone object that 
 *                    will contain the data that is needed for calculating the
 *                    normal vector
 *             norm_vector - pointer to an array of doubles that will store the
 *                           the normal vectors components
 */
void getnorm_cone(obj_t *obj, double *norm_vector) {
    norm_vector[0] = obj->normal[0];
    norm_vector[1] = obj->normal[1];
    norm_vector[2] = obj->normal[2];
   
}
