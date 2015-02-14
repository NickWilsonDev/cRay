#include <stdio.h>
#include "plane.h"
#include "structs.h"
#include "malloc.h"
#include "object.h"
#include "veclib.h"
#include "material.h"
#include "ray.h"
#include "triangle.h"

/*
 * triangle.c - This file deals with triangles
 */

/*
 * fplane_init() - function initializes the triangle object
 *
 * Parameter: in - pointer to a file
 *            objtype - int representing type of object
 *
 * Return obj_t
 */
obj_t *triangle_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    tri_t *triangle = NULL;

    obj = object_init(in, objtype, refraction_index);

    triangle = Malloc(sizeof(tri_t));
    

    obj->hits = triangle_hits;

    obj->priv = triangle;
    char buf[MAXCHAR];

    fprintf(stderr, "inside triangle_init()\n");
    scanf("%lf %lf %lf", &triangle->point1[0], &triangle->point1[1], &triangle->point1[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts

    scanf("%lf %lf %lf", &triangle->point2[0], &triangle->point2[1], &triangle->point2[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts
    
    scanf("%lf %lf %lf", &triangle->point3[0], &triangle->point3[1], &triangle->point3[2]);
    fgets(buf, MAXCHAR, in); //Consume remaining texts
    
    /* compute planes normal first */
    double a[3];
    double b[3];
    diff3(triangle->point1, triangle->point2, a);
    diff3(triangle->point1, triangle->point3, b);
    cross(a, b, obj->normal);

    triangle_dump(stderr, obj);
    return obj;
}

/*
 * triangle_dump() - function prints out the data for the triangle
 *
 * Parameters: out - pointer to a file
 *             obj - pointer to an obj_t structure
 *
 * Return int - signifying success or failure
 */
int triangle_dump(FILE *out, obj_t *obj) {
    //plane_t *plane = obj->priv;
    tri_t *tri = obj->priv;
    int rc; 
    
    fprintf(stderr,"inside triangle_dump()\n");
    fprintf(out, "Dumping object of type triangle\n");
    rc = material_dump(out, &obj->material);//this func print out reflectivities
    fprintf(out, "Triangle data -\n");
    fprintf(out, "point1 - \t%lf \t%lf \t%lf\n", tri->point1[0], tri->point1[1],
            tri->point1[2]);

    fprintf(out, "point2 - \t%lf \t%lf \t%lf\n", tri->point2[0], tri->point2[1],
            tri->point2[2]);
    fprintf(out, "point3 - \t%lf \t%lf \t%lf\n", tri->point3[0], tri->point3[1],
            tri->point3[2]);
    return rc;
}

/*
 * triangle_hits() - function finds out if the triangle has been hit
 *
 * Parameter: base - pointer to an array of doubles, origin position of the 
 *                   ray
 *            dir  - pointer to an array of doubles, direction of the ray
 *            obj  - pointer to an obj_t struct
 *
 * Return: double - signifies whether triangle was hit or not -1 means 
 *                  miss
 */
double triangle_hits(double *base, double *dir, obj_t *obj) {
    double t;
    double newhit[3];

    t = hits_plane(base, dir, obj);
    if (t < 0.00001)
        return(t);

    fprintf(stderr, "ray hits plane\n");
    //plane_t *pln = obj->priv;
    tri_t *tri = obj->priv;
    double normal[3];
    vec_copy3(obj->normal, normal);
    
    double n_dot_raydir = dot3(normal, dir);
    if (n_dot_raydir == 0.0) {
        return -1;
    }

    /* compute d parameter */
    double d = dot3(normal, tri->point1);
    /* compute t */
    t = -(dot3(normal, base) + d) / n_dot_raydir;
    /* need to check if triangle is behind the ray */
////////

    /* compute intersection point */
    double point[3];
    scale3(t, dir, point);
    sum(point, base, point);

    /*************** inside out test *****************/
    double vec_c[3]; // vector perpendicular to triangle plane

    // edge 0
    double edge0[3];
    diff3(tri->point1, tri->point2, edge0);
    double vp0[3];
    diff3(tri->point1, point, vp0);
    cross(edge0, vp0, vec_c);
    if (dot3(normal, vec_c) < 0)
        return -1; // point is on the right side

    // edge 1
    double edge1[3];
    diff3(tri->point2, tri->point3, edge1);
    double vp1[3];
    diff3(tri->point2, point, vp1);
    cross(edge0, vp1, vec_c);
    if (dot3(normal, vec_c) < 0)
        return -1; // point is on the right side
    

    // edge2 
    double edge2[3];
    diff3(tri->point3, tri->point1, edge2);
    double vp2[3];
    diff3(tri->point3, point, vp2);
    cross(edge2, vp2, vec_c);
    if (dot3(normal, vec_c) < 0)
        return -1; // point is on the right side and outside triangle

    return t;

}

