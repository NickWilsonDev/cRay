#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "ray.h"
#include "object.h"
#include "veclib.h"
#include "texture.h"
#include "texplane.h"
#include "plane.h"
#include "fplane.h"
#include "malloc.h"

/*
 * author Nick Wilson and Kao Thao
 * version 4.29.13
 *
 * texplane.c - this file contains functions that deal with loading and 
 * displaying textured planes. The include texplane_load, texplane_dump, 
 * texplane_amb, texplane_diff, and texplane_spec.
 *
 */

/*
 * texplane_load() - function initializes the textured plane.
 * 
 * Parameters: in - pointer to a file
 *             lst - pointer to a list
 *             objtype - type of object represented by an int
 *
 * Return - obj_t
 */
obj_t *texplane_init(FILE *in, int objtype, float refraction_index) {
    texplane_t *texp = NULL;
    fplane_t *finiteplane = NULL;
    plane_t *plane = NULL;
    obj_t *obj = NULL;
    char buf[MAXCHAR];
    
    fprintf(stderr, "begining of texplane_init()\n");
    /* Create fplane_t, plane_t, and obj_t structures */
    obj = fplane_init(in, objtype, refraction_index);
    if (obj == NULL) {
        fprintf(stderr, "call to fplane_init from texplane_init was unsuccessfull\n");
        return NULL;
    }
    fprintf(stderr, "inside texplane_init()\n");


    plane = obj->priv;
    finiteplane = plane->priv;

    texp = Malloc(sizeof(texplane_t));
    finiteplane->priv = texp;//finiteplane;

    /* assign functions to void pointers */
    obj->hits = fplane_hits;
    obj->getamb = texplane_amb;  // this is where the action will be
    
    /* read in texture file name and texture file mode */
    scanf("%39s", texp->texname);
    fgets(buf, MAXCHAR, in);
    scanf("%d", &texp->texmode);
    fgets(buf, MAXCHAR, in);

#ifdef DBG_TEXMAP
    fprintf(stderr, "texture name= %s\n", texp->texname);
    fprintf(stderr, "texture mode= %d\n", texp->texmode);
#endif

    if (texture_load(texp)) {
        fprintf(stderr, "Error loading texture from texplane.c\n");
        return(NULL);
    }
    fprintf(stderr, "return from texplane init\n");
    return(obj);
}


/* 
 * texplane_amb() - function returns intensity that is the product of the 
 * objects reflectivity with the texel that maps to the hit point. The 
 * texture_map() function in texture.c determines what texel maps to the hit
 * point.
 *
 * Parameters: obj - pointer to obj_t structure
 *             value - pointer to an array of doubles
 */
void texplane_amb(obj_t *obj, double *value) {
    plane_t *plane = obj->priv;
    fplane_t *finiteplane = plane->priv; // was casting it to proper object type
    int rc;
    double texel[3];
     
    fprintf(stderr, "-------------------------------------------------\n");
    fprintf(stderr, "begining texplane_amb()\n");
    fprintf(stderr, "lasthit inside texplaneamb  %lf, %lf\n", finiteplane->lasthit[0], finiteplane->lasthit[1]);
    rc = texture_map(finiteplane, texel);
    if (rc != 1) {
        fprintf(stderr, "There was a problem in 'texture_map()' function\n");
    }
    //fprintf(stderr, "rc from texture_map() is %d\n", rc);

    /* compute product of reflectivity and texel */
    int i;
    for (i = 0; i < 3; i++) {
        value[i] = texel[i];
    }
    //fprintf(stderr, "value =  %lf, %lf, %lf\n", value[0], value[1], value[2]);
}
