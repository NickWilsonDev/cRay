#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ray.h"
#include "texsphere.h"
#include "sphere.h"
#include "structs.h"
#include "malloc.h"
#include "veclib.h"
#include "perlin.h"
#include "util.h"

obj_t *texsphere_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    texsphere_t *sphere = NULL;
    obj = object_init(in, objtype, refraction_index); 
    sphere = (texsphere_t*) Malloc(sizeof(texsphere_t));

    obj->hits = hits_texsphere;
    obj->getamb = getamb_texsphere;
    obj->getdiff = getdiff_texsphere;
    obj->getspec = getspec_texsphere;
    obj->getnorm = getnorm_texsphere;

    obj->getloc = getloc_texsphere;

    obj->priv = sphere;

    char buf[MAXCHAR];

    get_3_doubles(&sphere->center[0], &sphere->center[1], &sphere->center[2]);
    get_1_double(&sphere->radius);
    scanf("%s", buf);
    sphere->image = readPPM(buf);
    fprintf(stderr, "image filename == %s\n", buf); 
    fgets(buf, MAXCHAR, in); // consume remaining texts

    //texsphere_dump(stderr, obj); //might need an rc variable

    return obj;
}

int texsphere_dump(FILE *out, obj_t *obj) {
    texsphere_t *sphere = obj->priv;
    int rc;

    fprintf(stderr, "Dumping object of type texSphere\n");
    rc = material_dump(out, &obj->material); //this func print to stderr
    fprintf(stderr, "texSphere data \n");
    fprintf(stderr, "center - \t%lf \t%lf \t%lf\n", sphere->center[0], sphere->center[1], sphere->center[2]);
    fprintf(stderr, "radius - \t%lf\n\n", sphere->radius);
    fprintf(out, "refraction index == %f\n", obj->refraction_index);

    PPMImage *image = sphere->image;
    PPMPixel *data = image->data;
    int x, y;
    for (y = 0; y < image->y; y++) {
        for (x = 0; x < image->x; x++) {
            fprintf(stderr, "   %d %d %d ", data->red, data->green, data->blue);
        }
        fprintf(stderr, "\n");
    }
    return rc;
}

/*
 * hits_texsphere - function provides a way of telling whether the sphere was hit
 * by a ray or not.
 * 
 * Parameter: base - origin of the ray
 *            dir  - direction of the ray
 *            obj  - the object we hit
 *
 * Return : distance to the hit location from base
 */
double hits_texsphere(double *base, double *dir, obj_t *obj) {
    texsphere_t *sphere = (texsphere_t *) obj->priv;

    int i;
    //double th;

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

        if (discriminate <= 0) { return -1; }

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
    
    return th;
}

/*
 * Function gets the sphere's ambient light characteristics and stores them
 * into the "intensity" parameter which is an array of doubles.
 * But instead of getting ambient value from material we will get pixel from
 * PPMImage and use its values in the intensity vector.
 */
void getamb_texsphere(obj_t *obj, double *intensity) {
    texsphere_t *sphere = obj->priv;
    double pi = 22 / 7; // may make a float

//////////////////////////////// temp solution /////////////////////
    double temp[3];
    vec_copy3(obj->hitloc, temp);
    unitvec(temp, temp);
///////////////////// temp 2 /////////////////////////////
// this works but texture is stretched and warped
    float theta = acos(temp[1]);
    float phi =  atan(temp[0] / temp[2]);

    if (phi < 0.0)
        phi += 2 * pi;

    float u = (phi / (2 * pi)) - (pi/5); // the second term rotates texture
    float v = theta / pi;
    
    PPMImage *image = sphere->image;

    int width = (int) (u * (image->x - 1));
    int height = (int)(v * (image->y - 1));

#ifdef DBG_TEXMAP
    fprintf(stderr, "sphere->center %lf, %lf, %lf\n", sphere->center[0], sphere->center[1], sphere->center[2]);
    fprintf(stderr, "imagex = %d, imagey = %d\n", image->x, image->y);
    fprintf(stderr, "u = %lf, v = %lf\n", u, v);
    fprintf(stderr, "width == %d, height == %d\n", width, height);
    fprintf(stderr, "--------------------------------------------\n");
#endif     

    PPMPixel pixel = image->data[((image->x) * height) + width];

#ifdef DBG_TEXMAP
    fprintf(stderr, "pixel components red= %d, green= %d, blue= %d\n",
             pixel.red, pixel.green, pixel.blue);
#endif
    intensity[0] = (double)pixel.red / 255;
    intensity[1] = (double)pixel.green / 255;
    intensity[2] = (double)pixel.blue / 255;
}

void getdiff_texsphere(obj_t *obj, double *diffuse) { //may have to change intensity
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

void getspec_texsphere(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.specular[0]);
    intensity[1] = (obj->material.specular[1]);
    intensity[2] = (obj->material.specular[2]);
}

void getloc_texsphere(obj_t *obj, double *location) {
    sphere_t *temp = obj->priv;
    location[0] = temp->center[0];
    location[1] = temp->center[1];
    location[2] = temp->center[2];
}

/*
 * getnorm_texsphere - Function computes the normal vector for a sphere at the 
 *                  hitpoint. Formula is as follows hitlocation - center of
 *                  sphere equals normal vector to sphere at hitlocation.
 * Parameters: obj - sphere object that will contain the data that is needed 
 *                   for calculating the normal vector
 *             norm_vector - pointer to an array of doubles that will store the
 *                           the normal vectors components
 */
void getnorm_texsphere(obj_t *obj, double *norm_vector) {
    norm_vector[0] = obj->normal[0];
    norm_vector[1] = obj->normal[1];
    norm_vector[2] = obj->normal[2];
}


void getbump_texsphere(obj_t *obj, double *norm_vector) {
}

/*
 * Function gets desired pixel from from the image that will be mapped to the
 * sphere. Only works with PPM format.
 */
PPMPixel *getpixel(int width, int height,  PPMImage *image) {
    PPMPixel *pixel = (PPMPixel*) Malloc(sizeof(PPMPixel));
    pixel = &image->data[width  + (width * height)];

    fprintf(stderr, "pixel components red= %d, green= %d, blue= %d\n",
             pixel->red, pixel->green, pixel->blue);
//////////////////////////////////// losing it after stack returns probably
//have to malloc something
    return pixel;
}
