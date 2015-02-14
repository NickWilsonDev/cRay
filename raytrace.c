#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raytrace.h"
#include "veclib.h"
#include "ray.h"
#include "illuminate.h"

int entry = 1;
/*
 * author Nick Wilson and Kao Thao
 * version 3.31.13
 *
 * raytrace.c
 *
 */

/*
 * ray_trace - This function traces a single ray and returns the composite
 *             intensity of the light it encounters. It is recursive and so the
 *             start of the ray cannot be assumed to be the viewpoint. 
 *             Recursion will not be involved until we take on specular light.
 *
 * Parameters:  model  - pointer to the model container
 *              base[] - location of viewer, or previous hit(in the recursive
 *                       version)
 *              dir[]  - unit vector in the direction of the object
 *              intensity[] - intensity returned from location
 *              total_dist - distance ray has traveled so far
 *              last_hit - obj that reflected this ray or NULL
 */
void ray_trace(model_t *model, double base[3], double dir[3], double 
                intensity[3], double total_dist, obj_t *last_hit) {
    //We will call find_closest_object function to find the closest object that
    //is hit, if none are hit then NULL will be returned. The distance from the
    //base of the ray to the nearest hitpoint is returned in mindist.
    //viewpoint is supplied from file(stdin)
    obj_t *closest = NULL;
    double mindist = 100.0;
    double specref[3]   = {0.0, 0.0, 0.0};
    //double refrac[3]    = {0.0, 0.0, 0.0};
    double refracint[3] = {0.0, 0.0, 0.0};
    double specint[3]   = {0.0, 0.0, 0.0};

    if (total_dist > 100.001)
        return;

    fprintf(stderr, "original ray --- [%lf, %lf, %lf]\n", dir[0], dir[1], dir[2]);

    closest = find_closest_obj(model->scene, base, dir, NULL, &mindist);
    if (closest == NULL) {
        fprintf(stderr, "closest was NULL\n");
        return;
    }

#ifdef DBG_HIT
    fprintf(stderr, " HIT %d: %5f", closest->objtype, mindist);
    fprintf(stderr, "(%5f, %5f, %5f)\n", closest->hitloc[0],
            closest->hitloc[1], closest->hitloc[2]);
#endif

    total_dist = total_dist + mindist;

    //set intensity to the ambient refletivity of closest
    closest->getamb(closest, intensity);

    // call diffuse_illumination to add diffuse lighting to object
    diffuse_illumination(model, closest, intensity);

    scale3((1/total_dist), intensity, intensity);

    //specular/recursive part
    closest->getspec(closest, specref);

    if (dot3(specref, specref) > 0.0001) {
        double ref_dir[3] = {0.0, 0.0, 0.0};
        double norm[3];
        closest->getnorm(closest, norm);

        //compute direction, ref_dir, of the reflected ray.
        reflect(dir, norm, ref_dir);

        fprintf(stderr, "reflected ray -- [%lf, %lf, %lf]\n", ref_dir[0], ref_dir[1], ref_dir[2]);
        ray_trace(model, closest->hitloc, ref_dir, specint, total_dist,
                  closest);
        // multiply specref by specint leaving result in specref
        int i;
        for (i = 0; i < 3; i++) {
            specref[i] = specref[i] * specint[i];
        }
    }

    if (closest->refraction_index != 1.0) {
        // if entry is negative = entering object
        //    entry is positive = exiting object
        fprintf(stderr, "entry is == %d\n", entry);
        entry = entry * -1;
        double refrac_dir[3] = {0.0, 0.0, 0.0};
        double refrac_norm[3];
        closest->getnorm(closest, refrac_norm);
        if (entry == 1)
            refrac_norm[2] = refrac_norm[2] * -1;
        refract(entry, closest->refraction_index, dir, refrac_norm, refrac_dir);
        obj_t *rclosest;
        rclosest = find_closest_obj(model->scene, closest->hitloc, refrac_dir, NULL, &mindist);
        if (rclosest == NULL)
            return;
        
        fprintf(stderr, "refracted ray -- [%lf, %lf, %lf]\n", refrac_dir[0], refrac_dir[1], refrac_dir[2]);
        double critical_angle = dot3(refrac_norm, refrac_dir);
        double sin_angle = sin(critical_angle);
        if (sin_angle <= -1.0001 || sin_angle >= 1.0001) {
            intensity[0] = 100.0;
            fprintf(stderr, "bad angle in refraction\n");
        } else {
        //    fprintf(stderr, "critical angle = %lf\n", critical_angle);
        //    //compute direction, ref_dir, of the reflected ray.
            if (isnan(refrac_dir[0]) == 0 && isnan(refrac_dir[1]) == 0 && isnan(refrac_dir[2]) == 0) {
                fprintf(stderr, "!refract\n");
                ray_trace(model, rclosest->hitloc, refrac_dir, refracint, total_dist,
                        closest);


                scale3(5, refracint, refracint);
            } else {
                fprintf(stderr, "else in refraction\n");
                return;
            }
        }
    }
    // sum up specular reflection intensity, refraction intensity
    // color = reflection color * Kr + refraction color * (1 - Kr)
    // need to use snell's equation to figure out what to scale refractive
    // color and reflective color by, uses angle, reflective will go up as
    // refractive goes down and vice versa

    //scale3(10, refracint, refracint);

    sum(intensity, specref, intensity);

    sum(intensity, refracint, intensity);

}

/*
 * find_closest_obj - 
 *
 * Parameters: list - pointer to list containing all objects in scene except
 *                    possibly the lights
 *
 *             viewer - location of viewer or previous hit
 *             unit_dir - unit vector in direction of object
 *             obj       - object that the hits function will be called on
 *             mindist  - the minimum distance to check for a hit
 *
 * Returns:
 *          obj_t - the closest object
 */
obj_t *find_closest_obj(list_t *list, double *viewer, double *unit_dir, 
                        obj_t *obj, double *min_dist) {
    double dist = 0.0;

    obj_t *current = list->first;
    obj_t *ret_obj = NULL;

    // Check every object in the list
    while(current != NULL) {
        obj_t *temp_obj = current;
        dist = temp_obj->hits(viewer, unit_dir, temp_obj);
#ifdef DBG_FIND
            fprintf(stderr, "FND %d: %lf -\n", temp_obj->objid, dist);
#endif
        if (dist < *min_dist && dist >= 0.0) {
            *min_dist = dist;
            ret_obj = temp_obj;
        }
        
#ifdef DBG_FND
        fprintf(stderr, "\nFND   %d ::DIST- %f\n", temp_obj->objid, dist);
        vecprn3("base  ", viewer);
        vecprn3("unit_dir ", unit_dir);
#endif
        current = current->next;
    }
    return ret_obj;
}

