#include <stdio.h>
#include <stdlib.h>
#include "raytrace.h"
#include "projection.h"
#include "model.h"
#include "structs.h"
#include "light.h"
#include "plane.h"
#include "sphere.h"
#include "ray.h"
#include "pplane.h"
#include "psphere.h"
#include "fplane.h"
#include "tplane.h"
#include "spotlight.h"
#include "disk.h"
#include "cylander.h"
#include "texplane.h"
#include "texsphere.h"
#include "triangle.h"
#include "cone.h"
#include "dispsphere.h"
//#include "torus.h"

/*
 * model.c - this file deals with setting up the virtual world it calls the 
 * necessary init functions to do this.
 */

/*
 * dummy_init - function is just a placeholder inside the table of function
 * pointers.
 */
obj_t *dummy_init(FILE *in, int objtype, float refraction_index) { return(NULL); }

/*
 * table of function pointers for init functions
 */
obj_t *(*obj_loaders[])(FILE *in, int objtype, float refraction_index) = 
{
    light_init,     /* object type 10 (light) */
    spotlight_init, /* object type 11 (spotlight)*/
    dummy_init,     /* placeholder for type 12 */
    sphere_init,    /* object type 13 (sphere)*/
    plane_init,     /* object type 14 */
    fplane_init,    /* object type 15 (finite_plane)*/
    tplane_init,    /* object type 16 (tiled_plane)*/
    texplane_init,     /* placeholder for type 17 (tex_plane)*/
    dummy_init,     /* placeholder for type 18 */
    psphere_init,     /* placeholder for type 19 (procedure_sphere)*/
    pplane_init,     /* object type 20 (procedure_plane)*/
    dummy_init,     /* placeholder for type 21 (paraboloid)*/
    cylander_init,     /* placeholder for type 22 (cylander) */
    cone_init,     /* placeholder for type 23 (cone) */
    dummy_init,     /* placeholder for type 24 (hyperboloid) */
    dummy_init,     /* placeholder for type 25 (torus)  */
    texsphere_init,   /* object type 26 (texsphere) */
    triangle_init,  /* object type 27 (triangle) */
    dispsphere_init, /* object type 28 (displacement sphere) */
    disk_init       /* placeholder for type */

};

/*
 * model_init - funtion loads our scene
 *
 * Parameter: in - pointer to a file
 *            model - pointer to a model_t structure that contains all the
 *                    objects
 *
 * Return: int - signifies success or failure
 */
int model_init(FILE *in, model_t *model) {
    char buf[MAXCHAR];
    int objtype;
    //float refraction_index;
    double refraction_index;
    obj_t *new = NULL;

    /* load the objects in the scene */
    while (scanf("%d", &objtype) == 1) { //1 is the number of item assigned
        if (scanf("%lf", &refraction_index) == 1) {}
        else {
            refraction_index = 1.0;
        }

        fgets(buf, MAXCHAR, in); /*consume rest of line */
        if ((objtype >= FIRST_TYPE) && (objtype <= LAST_TYPE)) {
            new = (*obj_loaders[objtype - FIRST_TYPE])(in, objtype,
                                                       refraction_index);
            if (new == NULL) {
                fprintf(stderr, "model tried to add NULL object\n");
                return(-2);
            }
            /* add new object to proper list (scene or lights) */
            if ((new->objtype) != 10 && (new->objtype != 11)) {
                list_add(model->scene, new);
                fprintf(stderr,"shouldve added %d\n", new->objtype);
            } else {

                fprintf(stderr,"light added %d\n", new->objtype);
                list_add(model->lights, new);
            }
        } else {
            /* invalid object id */
            fprintf(stderr, "Invalid object type %d \n", objtype);
            return(-1);
        }
    }
    return(0);
}

/*
 * model_dump - function calls the objects dump functions to print out all the
 *              data for all the objects in the scene
 *
 * Parameters: out - pointer to a file
 *             model - pointer to a model_t struct that contains our scene
 *
 * Return int - signifies success or failure
 */
int model_dump(FILE *out, model_t *model) {
    fprintf(stderr, "model.c model_dump() \n");

    proj_t *proj = (proj_t*) model->proj;     
    list_t *list = (list_t*) model->scene; //->first;
    obj_t *temp = list->first;
    fprintf(stderr, "Model Dump\n");
    int rc = 0;
    rc = projection_dump(out, proj);
    if (temp == NULL)
        fprintf(stderr,"temp is apparently null inside model dump\n");
    while (temp) {
          switch (temp->objtype) {  
            case LIGHT:
                fprintf(stderr, "starting light_dump()\n");
                light_dump(out, temp);
                break;
            case PLANE:
                fprintf(stderr,"starting plane_dump()\n");
                plane_dump(out, temp);
                break;
            case SPHERE:
                fprintf(stderr,"starting sphere_dump()\n");
                sphere_dump(out, temp);
                break;
            case P_PLANE:
                fprintf(stderr,"procedure plane dump()\n");
                break;
            case FINITE_PLANE:
                fprintf(stderr,"finite plane dump()\n");
                fplane_dump(out, temp);
            case TILED_PLANE:
                fprintf(stderr,"tiled plane dump()\n");
                break;
            default:
                break;
        }
        temp = temp->next;
    }
    return rc;
}
