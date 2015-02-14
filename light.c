#include <stdio.h>
#include "structs.h"
#include "malloc.h"
#include "object.h"
#include "veclib.h"
#include "light.h"
#include "material.h"

/*
 * author: Nick Wilson and Kao Thao
 * version: 4.20.13
 *
 * light.c - This file contains functions that model lights
 */

/*
 * light_init - function that initilizes a light object
 *
 * Parameters: in - pointer to a file
 *             objtype - int representing type of object
 *
 * return: obj_t
 */
obj_t *light_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    light_t *light = NULL;

    obj = object_init(in, objtype, refraction_index);
    light = Malloc(sizeof(light_t));

    //void pointers to functions
    obj->hits = hits_light;

    obj->priv = light;
    char buf[256];

    // read in data that is specific to light
    scanf("%lf %lf %lf", &light->emissivity[0], &light->emissivity[1],
            &light->emissivity[2]);
    fgets(buf, 256, in); //Consume remaining texts
    scanf("%lf %lf %lf", &light->location[0], &light->location[1],
            &light->location[2]);
    fgets(buf, 256, in); //Consume remaining texts

    //light_dump(stderr, obj);

    return obj;
}

/*
 * light_dump() - function prints out light data
 *
 * Parameter: out - pointer to a file
 *            obj - pointer to a obj_t struct
 *
 * Return: int - signifies success or failure of function
 */
int light_dump(FILE *out, obj_t *obj) {
    light_t *light = obj->priv;
    int rc;

    fprintf(out, "Dumping object of type Light\n");
    rc = material_dump(out, &obj->material);// print out reflectivities
    fprintf(out, "Light data -\n");
    fprintf(out, "emisitivity - \t%lf \t%lf \t%lf\n", light->emissivity[0],
            light->emissivity[1], light->emissivity[2]);
    fprintf(out, "location - \t%lf \t%lf \t%lf\n\n", light->location[0],
            light->location[1], light->location[2]);

    return rc;
}

/*
 * hits_light - function is just a blank function that always returns 0
 *
 * Parameter: base - origin of the ray
 *            dir  - direction of the ray
 *            obj - pointer to an obj_t structure
 *
 * Return: always returns zero since our lights cannot be hit
 */
double hits_light(double *base, double *dir, obj_t *obj) {
    return 0;
}
