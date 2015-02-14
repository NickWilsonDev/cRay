#include <stdlib.h>
#include <stdio.h>
#include "illuminate.h"
#include "raytrace.h"
#include "veclib.h"
#include "light.h"
#include "structs.h"
#include "spotlight.h"
#include <math.h>

/*
 * illuminate.c
 * author Nick Wilson and Kao Thao
 * version 4.9.13
 *
 * This file contains functions that help model Diffuse illumination.
 */

/*
 * diffuse_illumination - function checks all lights in the list and calls
 *                        process light on each
 *
 * Parameters: model - pointer to the model structure
 *             hitobj - object that was hit by the ray
 *             intensity - where to add the intensity
 */
void diffuse_illumination(model_t *model, obj_t *hitobj, double *intensity) {
    obj_t *current = model->lights->first;

    while(current) {
        obj_t *temp_light = current;
        process_light(model->scene, hitobj, temp_light, intensity);
        current = current->next;
    }
}

/*
 * process_light - 
 *
 * Parameters: lst - list of all objects
 *             hitobj - the object hit by the ray
 *             lobj - the current light source
 *             ivec - [r, g, b] intensity vector
 *
 * Return: type int - 0 if nothing is changed
 *                    1 if diffuse light values were added to intensity vec
 */
int process_light(list_t *lst, obj_t *hitobj, obj_t *lobj, double *ivec) {
    double dir[3]; // vec between hitpoint and light location
    double distance_to_light; // distance from hitlocation to light
    double mindist = 1000;//MAX_INT; // 100?
    double cos; //angle

    light_t *light = lobj->priv;

    /* If the light is a directional light such as a spotlight or */
    /* a projector it may have a special visibility function */
    /* inner if determines whether or not obj is illuminated or not */
    if (lobj->illum_check) {
        if (lobj->illum_check(lobj, hitobj->hitloc) == 0)
            return 0;
    }

    diff3(hitobj->hitloc, light->location, dir);
    distance_to_light = length3(dir);

    unitvec(dir, dir);

    double temp[3];
    hitobj->getnorm(hitobj, temp);
    cos = dot3(temp, dir);

#ifdef DBG_DIFFUSE
    light_t *lt = lobj->priv;
    fprintf(stderr,"=========================================\n");
    fprintf(stderr,"hit object type was %d\n ", hitobj->objtype);
    vecprn3("hit point was ", hitobj->hitloc);
    vecprn3("normal at hitpoint was   ", temp);
    fprintf(stderr,"light object id was       %d\n", lobj->objid);
    vecprn3("light center was            ", lt->location);
    vecprn3("unit vector to light is ", dir);
    fprintf(stderr, "distance to light is ======= %lf\n ", distance_to_light);
    fprintf(stderr, "cosine is  %lf\n", cos);
    fprintf(stderr,"==========================================\n");
#endif

    // if the object occludes itself return
    double answer = dot3(dir, temp);
    if (answer < 0.00001) {
       // fprintf(stderr,"object occluded itself objecttype ==%d\n",
       //         hitobj->objtype);
        return 0;
    }

    // two conditions to check for
    // whether this new ray hits any object at all and the new object is
    // closer to hitloc than the light
    obj_t *closest_obj = NULL;

    //fprintf(stderr,"mindist before call to findclosestobj() %lf\n", mindist);
    closest_obj = find_closest_obj(lst, hitobj->hitloc, dir, hitobj, &mindist);
    //fprintf(stderr,"mindist is after call to findclosestobj() %lf\n", mindist);

    if ((closest_obj != NULL) && (mindist < distance_to_light)) {
#ifdef DBG_DIFFUSE
/* IF occluded by another object */
            fprintf(stderr,"hit object occluded by %d\n", closest_obj->objid);
            fprintf(stderr,"distance was == %lf \n", mindist);
#endif
        return 0;
    }

    double diffuse[3];

    hitobj->getdiff(hitobj, diffuse);
    //vecprn3("ivec before addition is ", ivec);

    //fprintf(stderr, "distance to light == %lf\n", distance_to_light);
    //fprintf(stderr, "cos(angle) === %lf\n", cos);

    *(ivec + 0) += (diffuse[0] * light->emissivity[0] * 
                    (cos / distance_to_light));
    *(ivec + 1) += (diffuse[1] * light->emissivity[1] * 
                    (cos / distance_to_light));
    *(ivec + 2) += (diffuse[2] * light->emissivity[2] * 
                    (cos / distance_to_light));
#ifdef DBG_DIFFUSE
        fprintf(stderr,"################################################\n");
        vecprn3("emissivity of the light ", light->emissivity);
        vecprn3("diffuse reflectivity ", diffuse);
        vecprn3("current ivec  ", ivec);
        fprintf(stderr,"###############################################\n");
#endif

    return 1;
}
