#include <stdio.h>
#include <ctype.h>
#include "structs.h"
#include "ray.h"
#include "material.h"
#include "util.h"

/*
 * material_init - function reads in the basic data that all objects have in
 *                 common, ambient, diffuse, and specular values.
 *
 * Parameters in - pointer to a file
 *            mat - pointer to a material_t structure where the data will be
 *                  stored
 *
 * Return int - signifying success or failure
 */
int material_init(FILE *in, material_t *mat) {
    int rc = 0;      // boolean for reading (0-success, otherwise=failure)

    get_3_doubles(&mat->ambient[0], &mat->ambient[1], &mat->ambient[2]);
    get_3_doubles(&mat->diffuse[0], &mat->diffuse[1], &mat->diffuse[2]);
    get_3_doubles(&mat->specular[0], &mat->specular[1], &mat->specular[2]);
    
    return rc;
}

/*
 * material_dump - function prints out material data for an object
 *
 * Parameters: out - pointer to a file
 *             mat - pointer to a material_t struct where the data is stored
 * Returns: int - signifies success or failure
 */
int material_dump(FILE *out, material_t *mat) { //print formatted listing of datas
    fprintf(out, "Material data - \n");
    fprintf(out, "Ambient  - \t%lf \t%lf \t%lf\n", mat->ambient[0],
            mat->ambient[1], mat->ambient[2]);
    fprintf(out, "Diffuse  - \t%lf \t%lf \t%lf\n", mat->diffuse[0],
            mat->diffuse[1], mat->diffuse[2]);
    fprintf(out, "Specular - \t%lf \t%lf \t%lf\n\n", mat->specular[0],
            mat->specular[1], mat->specular[2]);

    return 0;
}
