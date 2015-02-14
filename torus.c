#include <stdlib.h>
#include <stdio.h>
#include "torus.h"
#include "veclib.h"
#include "util.h"
#include "malloc.h"

/* 
 * torus.c - file should contain functions that should help model a torus
 * /////////////////////////// Citation ///////////////////////////////
 * www.wowway.com/~phkahler/torus.pdf‎
 * almost all the ideas came from a pdf from this site
 * /////////////////////////// End Citation ///////////////////////////
 *
 * Author - besides above - Nick Wilson and Kao Thao
 * version 5.1.13
 *
 */

/* torus_init - function initializes a torus object
 *
 */
obj_t *torus_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    torus_t *torus = NULL;

    obj = object_init(in, objtype, refraction_index);
    torus = (torus_t*) Malloc(sizeof(torus_t));

    //void pointers to functions
    obj->hits = hits_torus;
    obj->getamb = getamb_torus;
    obj->getdiff = getdiff_torus;
    obj->getspec = getspec_torus;
    obj->getnorm = getnorm_torus;

    obj->priv = torus;
    char buf[MAXCHAR];

    // read in torus data
    scanf("%lf %lf %lf", &torus->center[0], &torus->center[1],
            &torus->center[2]);
    fgets(buf, MAXCHAR, in);

    scanf("%lf %lf %lf", &torus->axis_revolution[0],
            &torus->axis_revolution[1], &torus->axis_revolution[2]);
    fgets(buf, MAXCHAR, in);

    
    scanf("%lf", &torus->major_radius);
    fgets(buf, MAXCHAR, in);

    scanf("%lf", &torus->minor_radius);
    fgets(buf, MAXCHAR, in);

    torus_dump(stderr, obj);
    return obj;
}

int torus_dump(FILE *out, obj_t *obj) {
    torus_t *torus = obj->priv;
    int rc;

    fprintf(out, "Dumping object of type Torus\n");
    rc = material_dump(out, &obj->material);
    fprintf(out, "Torus data\n");
    fprintf(out, "center - \t%lf \t%lf \t%lf\n", torus->center[0],
                 torus->center[1], torus->center[2]);
    fprintf(out, "axis of revolution\t%lf \t%lf \t%lf", torus->axis_revolution[0],
            torus->axis_revolution[1], torus->axis_revolution[2]);

    fprintf(out, "major radius \t%lf", torus->major_radius);
    fprintf(out, "minor radius \t%lf", torus->minor_radius);
    return rc;
}


/*
 * hits_torus - function determines whether torus is hit or not.
 * 
 *
 */
double hits_torus(double *base, double *dir, obj_t *obj) {
    torus_t *torus = obj->priv;

    
    double r = torus->minor_radius;
    double R = torus->major_radius;

    /* unit vector in direction that ray is traveling */
    unitvec(dir, dir);

    /* move torus to center and rotate it to make calculations easier */
    /* make new base of ray */
    double newbase[3];
    diff3(torus->center, base, newbase);

    /* new center of torus */
    double newcenter[3];
    diff3(torus->center, torus->center, newcenter);


    /* a few calculations that will make later operations easier */
    double m = dot3(dir, dir);
    double n = dot3(dir, newbase);
    double o = dot3(newbase, newbase);
    double p = dot3(dir, torus->axis_revolution);
    double q = dot3(newbase, torus->axis_revolution);


    float coefficient[5];
    coefficient[0] = m * m;
    coefficient[1] = 4 * m * n;
    coefficient[2] = (4 * n * n) + (2 * m * (o - (r * r) - (R * R))) + (4 * (R * R) + (dir[2] * dir[2]));
    coefficient[3] = (4 * n) * (o - (r * r) - (R * R)) + (8 * (R * R) * newbase[2] * dir[2]);
    coefficient[4] = (o - (r * r) - (R * R) * (o - (r * r) - (R * R)));
    coefficient[4] = coefficient[4] + 4 * (R * R) * (newbase[2] * newbase[2]);
    coefficient[4] = coefficient[4] - 4 * (R * R) * (r * r);


    int numroots = check_quartic(&coefficient[5]);
    
    if (numroots == 0 || numroots == -1)
        return -1;

    float roots[4];
    quartic_solver(coefficient, &roots[0], &roots[1], &roots[2], &roots[3]);
    
    // fill out objects hitlocation
    //scale3(th, dir, dir); // may need to change this
    double th = roots[0]; // probably need to change this
    return th; 
}


void getamb_torus(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.ambient[0]);
    intensity[1] = (obj->material.ambient[1]);
    intensity[2] = (obj->material.ambient[2]);
}

void getdiff_torus(obj_t *obj, double *diffuse) { //may have to change intensi
    diffuse[0] = (obj->material.diffuse[0]);
    diffuse[1] = (obj->material.diffuse[1]);
    diffuse[2] = (obj->material.diffuse[2]);
}

void getspec_torus(obj_t *obj, double *intensity) {
    intensity[0] = (obj->material.specular[0]);
    intensity[1] = (obj->material.specular[1]);
    intensity[2] = (obj->material.specular[2]);
}

void getnorm_torus(obj_t *obj, double *norm_vector) {
    norm_vector[0] = obj->normal[0];
    norm_vector[1] = obj->normal[1];
    norm_vector[2] = obj->normal[2];
}

