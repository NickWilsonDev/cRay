#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture.h"
#include "structs.h"
#include "malloc.h"
#include "projection.h"

/*
 * author Nick Wilson and Kao Thao
 * version 4.29.13
 *
 * texture.c - this file contains three functions, texture_load(), 
 *             texture_map(), and texel_get().
 */

/*
 * texture_load() - this function attempts to read an image file that is part
 * of the structure specified as a parameter. It will parse the p6.ppm header
 * and if successful, load the data into a texture buffer.
 * 
 * Parameters: tp - structure pointer of type texplane_t
 *
 * return: int - 1 for success
 *             - 0 for failure
 */
int texture_load(texplane_t *tp) {
    /* attempt to fopen() the image file and return 'exit failure' on error */
    int rc = 0; // boolean for reading (0 = success, otherwise = fail)
    FILE *fp;
    char *file_name = tp->texname;
    char buffer[16];
    char c;
    int rgb_comp_color;

    unsigned int texture_x_dim; // may need to change these back to just ints
    unsigned int texture_y_dim;

    /* attempt to open file */
    fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "file name was null, doesnt exist\n");
        return 1;
    }


    /* read the .ppm header returning 'exit failure' if its not a
     * P6.ppm image */
    if (!fgets(buffer, sizeof(buffer), fp)) {
        fprintf(stderr, "Could not read header information\n");
        return 1;
    }
    
    /* Check the image format */
    if (buffer[0] != 'P' || buffer[1] != '6') {
        fprintf(stderr, "Invalid image format (not 'P6')\n");
        return 1;
    }

    /* malloc a texture_t structure and link it to the texplane_t */
    texture_t *temptexture = (texture_t*) Malloc(sizeof(texture_t));
    if (temptexture == NULL) {
        fprintf(stderr, "Could not allocate memory for texture_t\n");
        return 1;
    }
    tp->texture = temptexture;

    /* Check for comments */
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n');
            c = getc(fp);
    }
    ungetc(c, fp);
    fprintf(stderr, "inside texture load\n");
    
    /* save texture dimensions in the texture_t */
    if (fscanf(fp, "%d %d", &texture_x_dim, &texture_y_dim) != 2) {
        fprintf(stderr, "Invalid image size\n");
        return 1;
    } else {
        temptexture->size[0] = texture_x_dim;
        temptexture->size[1] = texture_y_dim;
    }

    /* read rgb component */
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component\n");
        return 1;
    }

    /* check rgb component depth */
    if (rgb_comp_color != RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bit components\n", file_name);
        return 1;
    }

    /* malloc a texture buffer to hold the pixel data and save its address in
     * the texture_t */
    unsigned char *texture_buffer = (unsigned char*) Malloc(3 * texture_x_dim 
                                     * texture_y_dim * sizeof(unsigned char));
                                            
    fprintf(stderr, "attempting to read texture data into array\n");
    /* read in the pixel data from pixmap */
    rc = fread(texture_buffer, 3 * texture_x_dim, texture_y_dim, fp);
    if (rc != texture_y_dim) {
        fprintf(stderr, "Error loading image '%s'\n", file_name);
        return 1;
    } else {
        rc = 0;
    }
     
    temptexture->texbuf = texture_buffer;
    //fprintf(stderr, "first pixel in texture r=%c, g=%c, b=%c\n", texture_buffer[1],
    //                texture_buffer[2], texture_buffer[3]);
    fclose(fp);
    /* return success or failure */
    return rc;
}

/* texture_map() - this function determines the texel that maps to the most 
 * recent hit point on the object.
 *
 * Parameters: finiteplane - pointer to structure of type fplane_t
 *             texel - pointer to an array of doubles
 *
 * Return: int - 1 is success
 *             - 0 is failure
 */
int texture_map(fplane_t *finiteplane, double *texel) {
    /* recover pointers to texplane_t and the texture_t */
    texplane_t *texp = finiteplane->priv;
    texture_t *texture = texp->texture;

    //fprintf(stderr, "begining of texture_map()\n");
    if ((texp->texmode) == TEX_FIT) {
    /* compute the fractional x-offset, xfrac, of the texel as fp->lashit[0] /
     * fp->size[0] */

        fprintf(stderr, "finiteplane size== %lf, %lf\n", finiteplane->size[0], finiteplane->size[1]);
        fprintf(stderr, "texture_map lasthit== %lf, %lf\n", finiteplane->lasthit[0], finiteplane->lasthit[1]);
        float xfrac = (finiteplane->lasthit[0]) / finiteplane->size[0];
        float yfrac = (finiteplane->lasthit[1]) / finiteplane->size[1];
        if (xfrac < 0.0)
            xfrac = 0;
        if (yfrac < 0.0)
            yfrac = 0;
        fprintf(stderr, "xfrac= %f, yfrac= %f\n", xfrac, yfrac);
        texel_get(texture, xfrac, yfrac, texel);
    } else {
        /* the way we will do this is through a
         * map_pix_to_world(fp->lasthit, pixhit); function
         * This is for 'tiling' mode
         */
        double pixhit[2];
        map_world_to_pix(finiteplane->lasthit, pixhit);
        float xfrac = (double)(1.0) * ((int)pixhit[0] % texture->size[0]) / texture->size[0];
        float yfrac = (double)(1.0) * ((int)pixhit[1] % texture->size[1]) / texture->size[1];

        texel_get(texture, xfrac, yfrac, texel);
    }
    return 1; //success?
}

/*
 * texel_get() - function copies the contents of the texel at fractional 
 * position(xrel, yrel) into the texel passed as a parameter
 *
 * Parameters: tex - pointer to structure type texture_t
 *             xrel - double
 *             yrel - double
 *             texel - an array of doubles size 3
 */
void texel_get(texture_t *tex, double xrel, double yrel, double texel[3]) {
    unsigned char *texloc;
    int xtex;   /* x coordinate of the texel */
    int ytex;   /* y coordinate of the texel */

    /* multiply x rel and y rel by respective texture dimensions getting
     * xtex and ytex */
    xtex = (int) (xrel * (tex->size[0] - 1));
    ytex = (int) (yrel * (tex->size[1] - 1));

    //fprintf(stderr, "xyrel== %lf, %lf\n", xrel, yrel);
    //fprintf(stderr, "x ytex== %d, %d\n", xtex, ytex);
    /* ensure 0 <= xtex < x texture size and 0 <= ytex < y texture size */
    if (xtex >= 0 && xtex < tex->size[0]) {
        if (ytex >= 0 && ytex < tex->size[1]) {
            // point texloc to proper position in texture array
            texloc = tex->texbuf;
            texloc = texloc + ((tex->size[0] * ytex) + xtex); 
        }
    } else {
        xtex = 0;
        ytex = 0;
    }

    /* compute offset, texloc, of the (ytex, xtex) in texture in the usual
     * way
     */

     texel[0] = *texloc / 255.0;
     texel[1] = *(texloc + 1) / 255.0;
     texel[2] = *(texloc + 2) / 255.0;

#ifdef DBG_TEXMAP
//    fprintf(stderr, "pixel components red = %c, green = %c, blue= %c\n",
//            *texloc, *(texloc + 1), *(texloc + 2));
#endif
}
