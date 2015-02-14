#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ray.h"
#include "dispsphere.h"
#include "sphere.h"
#include "structs.h"
#include "malloc.h"
#include "veclib.h"
#include "perlin.h"
#include "util.h"

obj_t *dispsphere_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    dispsphere_t *sphere = NULL;
    obj = object_init(in, objtype, refraction_index); 
    sphere = (dispsphere_t*) Malloc(sizeof(dispsphere_t));

    obj->hits = hits_dispsphere;
    obj->getamb = getamb_dispsphere;
    obj->getdiff = getdiff_dispsphere;
    obj->getspec = getspec_dispsphere;
    obj->getnorm = getnorm_dispsphere;

    obj->getloc = getloc_dispsphere;

    obj->priv = sphere;

    char buf[MAXCHAR];

    get_3_doubles(&sphere->center[0], &sphere->center[1], &sphere->center[2]);
    get_1_double(&sphere->radius);
    scanf("%s", buf);
    sphere->image = readPPM(buf);
    fprintf(stderr, "image filename == %s\n", buf); 
    fgets(buf, MAXCHAR, in); // consume remaining dispts

    dispsphere_dump(stderr, obj); //might need an rc variable

    return obj;
}

int dispsphere_dump(FILE *out, obj_t *obj) {
    dispsphere_t *sphere = obj->priv;
    int rc;

    fprintf(stderr, "Dumping object of type dispSphere\n");
    rc = material_dump(out, &obj->material); //this func print to stderr
    fprintf(stderr, "dispSphere data \n");
    fprintf(stderr, "center - \t%lf \t%lf \t%lf\n", sphere->center[0], sphere->center[1], sphere->center[2]);
    fprintf(stderr, "radius - \t%lf\n\n", sphere->radius);
    fprintf(out, "refraction index == %f\n", obj->refraction_index);

    return rc;
}

/*
 * hits_dispsphere - function provides a way of telling whether the sphere was hit
 * by a ray or not.
 * 
 * Parameter: base - origin of the ray
 *            dir  - direction of the ray
 *            obj  - the object we hit
 *
 * Return : distance to the hit location from base
 */
double hits_dispsphere(double *base, double *dir, obj_t *obj) {
    dispsphere_t *sphere = (dispsphere_t *) obj->priv;

    int i;

    double center[3];
    diff3(sphere->center, sphere->center, center);

    double new_base[3];
    diff3(sphere->center, base, new_base);

    double a = dot3(dir, dir);
    if (a == 0.0) {
        fprintf(stderr, "Divide by zero error.");
        exit(EXIT_FAILURE);
    }
    
        double b = 2 * dot3(new_base, dir);

        double c = dot3(new_base, new_base) - (sphere->radius * sphere->radius);

        double discriminate = (b * b) - (4 * a * c);

        /////////////////// test stuff ///////////////
        //discriminate = 
        ////////////////// test stuff ///////////////
        if (discriminate <= 0) { return -1; }

        double th = ((-1 * b) - sqrt(discriminate)) / (2 * a);

        double hitpoint[3];
        scale3(th, dir, hitpoint);

        double hit[3];
        sum(base, hitpoint, hit);

        for (i = 0; i < 3; i++) {
            obj->hitloc[i] = hit[i];
        }
        /* modify hitpoint with pixel from displacement map */
        PPMPixel pixel = get_pixel_from_map(sphere, obj->hitloc);
        double temp_sum = (pixel.red * 0.1) + (pixel.green * 0.1) 
                          + (pixel.blue * 0.1);
        //fprintf(stderr, "temp_sum == %lf\n", temp_sum);
        //if (temp_sum == 703.0)
        //    return -1; // temp if pixel is black then there is no hit


        // get normal at sphere for hitpoint
        double snorm[3];
        diff3(sphere->center, obj->hitloc, snorm);
        double length = length3(snorm);
        scale3((1/length), snorm, obj->normal);
        unitvec(obj->normal, obj->normal);
        //////////////////
        fprintf(stderr, "temp sum == %lf\n", temp_sum);    
        fprintf(stderr, "distance to hit is == %lf\n", th); 
        th = th - temp_sum;
        if (th < 0.0001)
            th = 0.0;
        //if (th > 200.0)
        //    th = 0.0;
    return th;
}

/*
 * Function gets the appropriate pixel from the displacement map and returns
 * it.
 */
PPMPixel get_pixel_from_map(dispsphere_t *sphere, double *hitloc) {
    double pi = 22 / 7; // may make a float

    double temp[3];
    vec_copy3(hitloc, temp);
    unitvec(temp, temp);
    float theta = acos(temp[1]);
    float phi =  atan(temp[0] / temp[2]);

    if (phi < 0.0)
        phi += 2 * pi;

    float u = (phi / (2 * pi)) - (pi/5); // the second term rotates dispture
    float v = theta / pi;
    
    PPMImage *image = sphere->image;

    int width = (int) (u * (image->x - 1));
    int height = (int)(v * (image->y - 1));

    PPMPixel pixel = image->data[((image->x) * height) + width];
    return pixel;
}


/*
 * Function gets the sphere's ambient light characteristics and stores them
 * into the "intensity" parameter which is an array of doubles.
 * But instead of getting ambient value from material we will get pixel from
 * PPMImage and use its values in the intensity vector.
 */
void getamb_dispsphere(obj_t *obj, double *intensity) {
   
    intensity[0] = obj->material.ambient[0];
    intensity[1] = obj->material.ambient[1];
    intensity[2] = obj->material.ambient[2];
}

void getdiff_dispsphere(obj_t *obj, double *diffuse) { 
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

void getspec_dispsphere(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.specular[0]);
    intensity[1] = (obj->material.specular[1]);
    intensity[2] = (obj->material.specular[2]);
}

void getloc_dispsphere(obj_t *obj, double *location) {
    sphere_t *temp = obj->priv;
    location[0] = temp->center[0];
    location[1] = temp->center[1];
    location[2] = temp->center[2];
}

/*
 * getnorm_dispsphere - Function computes the normal vector for a sphere at the 
 *                  hitpoint. Formula is as follows hitlocation - center of
 *                  sphere equals normal vector to sphere at hitlocation.
 * Parameters: obj - sphere object that will contain the data that is needed 
 *                   for calculating the normal vector
 *             norm_vector - pointer to an array of doubles that will store the
 *                           the normal vectors components
 */
void getnorm_dispsphere(obj_t *obj, double *norm_vector) {
    norm_vector[0] = obj->normal[0];
    norm_vector[1] = obj->normal[1];
    norm_vector[2] = obj->normal[2];
}


void getbump_dispsphere(obj_t *obj, double *norm_vector) {
}

