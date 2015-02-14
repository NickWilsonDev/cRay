#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ray.h"
#include "sphere.h"
#include "structs.h"
#include "malloc.h"
#include "veclib.h"
#include "perlin.h"
#include "util.h"

obj_t *sphere_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    sphere_t *sphere = NULL;
    obj = object_init(in, objtype, refraction_index); //was typecasting to obj_t * 
    sphere = (sphere_t*) Malloc(sizeof(sphere_t));
    
    obj->hits = hits_sphere;
    obj->getamb = getamb_sphere;
    obj->getdiff = getdiff_sphere;
    obj->getspec = getspec_sphere;
    obj->getnorm = getnorm_sphere; //getbump_sphere;

    obj->getloc = getloc_sphere;

    obj->priv = sphere;

    get_3_doubles(&sphere->center[0], &sphere->center[1], &sphere->center[2]);
    get_1_double(&sphere->radius);

    sphere_dump(stderr, obj); //might need an rc variable

    return obj;
}

int sphere_dump(FILE *out, obj_t *obj) {
    sphere_t *sphere = obj->priv;
    int rc;

    fprintf(out, "Dumping object of type Sphere\n");
    rc = material_dump(out, &obj->material); //this func print to stderr
    fprintf(out, "Sphere data \n");
    fprintf(out, "center - \t%lf \t%lf \t%lf\n", sphere->center[0], sphere->center[1], sphere->center[2]);
    fprintf(out, "radius - \t%lf\n\n", sphere->radius);
    fprintf(out, "refraction index == %f\n", obj->refraction_index);
    return rc;
}

/*
 * hits_sphere - function provides a way of telling whether the sphere was hit
 * by a ray or not.
 * 
 * Parameter: base - origin of the ray
 *            dir  - direction of the ray
 *            obj  - the object we hit
 *
 * Return : distance to the hit location from base
 */
double hits_sphere(double *base, double *dir, obj_t *obj) {
    sphere_t *sphere = (sphere_t *) obj->priv;

    int i;
    //double th;
    
    double center[3];
    diff3(sphere->center, sphere->center, center);
    
    double new_base[3];
    diff3(sphere->center, base, new_base);
    //vecprn3("old base", base);
    //vecprn3("new base", new_base);
    //vecprn3("dir", dir);

    double a = dot3(dir, dir);
    if (a == 0.0) {
        fprintf(stderr, "Divide by zero error.");
        exit(EXIT_FAILURE);
    }
    
        double b = 2 * dot3(new_base, dir);

        double c = dot3(new_base, new_base) - (sphere->radius * sphere->radius);

        double discriminate = (b * b) - (4 * a * c);

        if (discriminate <= 0.00001) { return -1; }

        double th = ((-1 * b) - sqrt(discriminate)) / (2 * a);

        double hitpoint[3];
        scale3(th, dir, hitpoint);

        double hit[3];
        sum(base, hitpoint, hit);
        for (i = 0; i < 3; i++) {
            obj->hitloc[i] = hit[i];// + noise;

        }

        // get normal at sphere for hitpoint
        double snorm[3];
        diff3(sphere->center, obj->hitloc, snorm);
        double length = length3(snorm);
        scale3((1/length), snorm, obj->normal);
        unitvec(obj->normal, obj->normal);

    fprintf(stderr, "th == %lf\n", th);    
////// gonna try adding a small random amount to the intersection
    //th = th - (rand() % 10);
    
    //th = th - sin(rand());
   // th = th - tan(rand());
    return th;
}

/*
 * Function gets the sphere's ambient light characteristics and stores them
 * into the "intensity" parameter which is an array of doubles.
 */
void getamb_sphere(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.ambient[0]);
    intensity[1] = (obj->material.ambient[1]);
    intensity[2] = (obj->material.ambient[2]);
}

void getdiff_sphere(obj_t *obj, double *diffuse) { //may have to change intensity
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

void getspec_sphere(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.specular[0]);
    intensity[1] = (obj->material.specular[1]);
    intensity[2] = (obj->material.specular[2]);
}

void getloc_sphere(obj_t *obj, double *location) {
    sphere_t *temp = obj->priv;
    location[0] = temp->center[0];
    location[1] = temp->center[1];
    location[2] = temp->center[2];
}

/*
 * getnorm_sphere - Function computes the normal vector for a sphere at the 
 *                  hitpoint. Formula is as follows hitlocation - center of
 *                  sphere equals normal vector to sphere at hitlocation.
 * Parameters: obj - sphere object that will contain the data that is needed 
 *                   for calculating the normal vector
 *             norm_vector - pointer to an array of doubles that will store the
 *                           the normal vectors components
 */
void getnorm_sphere(obj_t *obj, double *norm_vector) {
    norm_vector[0] = obj->normal[0];
    norm_vector[1] = obj->normal[1];
    norm_vector[2] = obj->normal[2];
}


void getbump_sphere(obj_t *obj, double *norm_vector) {
    
    double noise = pnoise(0.8 * obj->normal[0], 1.15 * obj->normal[1], 0.5 * obj->normal[2]);
    double noise2 = pnoise(obj->normal[0], obj->normal[1], obj->normal[2]);
    norm_vector[1] = obj->normal[1] + 8 * noise;

    noise = pnoise(0.8 * obj->normal[0], 1.15 * obj->normal[1], 0.5 * obj->normal[2]);
    norm_vector[2] = obj->normal[2] - 1 * noise + noise2;
    norm_vector[3] = obj->normal[3] + 3 * noise + noise2;

}
