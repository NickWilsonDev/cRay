#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ray.h"
#include "sphere.h"
#include "structs.h"
#include "malloc.h"
#include "veclib.h"
#include "disk.h"

obj_t *disk_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    disk_t *disk = NULL;
    obj = object_init(in, objtype, refraction_index); //was typecasting to obj_t * 
    disk = (disk_t*) Malloc(sizeof(disk_t));

    obj->hits = hits_disk;
    obj->getamb = getamb_disk;
    obj->getdiff = getdiff_disk;
    obj->getspec = getspec_disk;

    obj->priv = disk;

    char buf[MAXCHAR];

    scanf("%lf %lf %lf", &disk->center[0], &disk->center[1],
            &disk->center[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    scanf("%lf", &disk->radius);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    scanf("%lf %lf %lf", &disk->normal[0], &disk->normal[1],
            &disk->normal[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts
    
    disk_dump(stderr, obj); //might need an rc variable

    return obj;
}

int disk_dump(FILE *out, obj_t *obj) {
    disk_t *disk = obj->priv;
    int rc;

    fprintf(out, "Dumping object of type disk\n");
    rc = material_dump(out, &obj->material); //this func print to stderr
    fprintf(out, "disk data \n");
    fprintf(out, "center - \t%lf \t%lf \t%lf\n", disk->center[0], disk->center[1], disk->center[2]);
    fprintf(out, "radius - \t%lf\n\n", disk->radius);

    return rc;
}

/*
 * hits_disk - function provides a way of telling whether the disk was hit
 * by a ray or not.
 * 
 * Parameter: base - origin of the ray
 *            dir  - direction of the ray
 *            obj  - the object we hit
 *
 * Return : distance to the hit location from base
 */
double hits_disk(double *base, double *dir, obj_t *obj) {
    disk_t *disk = (disk_t *) obj->priv;
// not done
// not done doesnt work
    double test;
   // double *center = disk->center;
    double diff[3];
    double diff2[3];
    double radius = disk->radius;

    double radius_squared = radius * radius;
    diff3(dir, disk->center, diff);
    diff3(dir, disk->center, diff2);

    test = dot3(diff, diff2);

    if (test <= radius_squared) {
        return test;
    }
    return 1;
    /*
    int i;

    double center[3];
    diff3(disk->center, disk->center, center);

    double new_base[3];
    diff3(disk->center, base, new_base);

    double a = dot3(dir, dir);
    if (a == 0.0) {
        fprintf(stderr, "Divide by zero error.");
        exit(EXIT_FAILURE);
    }
    double b = 2 * dot3(new_base, dir);

    double c = dot3(new_base, new_base) - (disk->radius * disk->radius);

    double discriminate = (b * b) - (4 * a * c);

    if (discriminate <= 0) { return -1; }

    double th = ((-1 * b) - sqrt(discriminate)) / (2 * a);

    double hitpoint[3];
    scale3(th, dir, hitpoint);

    double hit[3];
    sum(base, hitpoint, hit);

    for (i = 0; i < 3; i++) {
        obj->hitloc[i] = hit[i];
    }

    // get normal at disk for hitpoint
    double snorm[3];
    diff3(disk->center, obj->hitloc, snorm);
    double length = length3(snorm);
    scale3((1/length), snorm, obj->normal);
    unitvec(obj->normal, obj->normal);

    return th;
    */
}

void getamb_disk(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.ambient[0]);
    intensity[1] = (obj->material.ambient[1]);
    intensity[2] = (obj->material.ambient[2]);
}

void getdiff_disk(obj_t *obj, double *diffuse) { //may have to change intensity
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

void getspec_disk(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.specular[0]);
    intensity[1] = (obj->material.specular[1]);
    intensity[2] = (obj->material.specular[2]);
}


