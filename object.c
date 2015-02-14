#include <stdio.h>
#include "material.h"
#include "structs.h"
#include "object.h"
#include "malloc.h"

/*
 * object.c - this file provides the "generic" object init and dump functions
 */

/*
 * object_init - function is the "generic" object init function
 *
 * Parameter: in               - pointer to a file
 *            objtype          - int signifing type of object
 *            refraction_index - float containing index of refraction for the
 *                               the object
 *
 * Return: obj_t
 */
obj_t *object_init(FILE *in, int objtype, float refraction_index) {
    obj_t *obj = NULL;
    static int id = 0;
    obj = (obj_t *)Malloc(sizeof(obj_t));
    obj->next = NULL;
    obj->objtype = objtype;
    obj->objid = ++id;
    fprintf(stderr, "initializing obj %d\n", obj->objtype);
    ////////////////////////
    if (refraction_index != EOF) {
        obj->refraction_index = refraction_index;
    } else {
        obj->refraction_index = 0.0;
    }

    if (objtype != LIGHT && objtype != SPOTLIGHT) {
        material_init(in, &obj->material);
    }

    return obj;
}

/*
 * object_dump - function is the "generic" object dump function
 *
 * Parameter: out - pointer to a file to write too
 *            obj - pointer to obj_t struct, which is model for an object
 *
 * Return: int - signifies success or failure
 */
int object_dump(FILE *out, obj_t *obj) {
    int rc;
    rc = material_dump(out, &obj->material);
    fprintf(out, "Object Dump - \n");
    fprintf(out, "id: \t%d", obj->objid);
    fprintf(out, "type: \t%d\n", obj->objtype);
    fprintf(out, "normal: \t%lf \t%lf \t%lf \n\n", obj->normal[0],
            obj->normal[1], obj->normal[2]);
    fprintf(out, "refraction index == %f\n", obj->refraction_index);
    
    return rc;
}
