#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for determining max amount of threads
#include <pthread.h> // this is for multiple threads
#include "structs.h"
#include "image.h"
#include "ray.h"
#include "raytrace.h"
#include "malloc.h"
#include "projection.h"
#include "veclib.h"
#include "raytrace.h"

#define NUM_THREADS sysconf(_SC_NPROCESSORS_ONLN)

#ifdef DBG_AA
    #define AA_SAMPLES 10
#endif

int recursion_depth;
/**
 *Author: Nick Wilson & Kao Thao
 *Version: 2.24.13
 * image.c
 */

//void *cruncher(void *threadid) {
//not done
//}
/*
 * make_image - Function that will print out all information necessary for 
 *              making a ppm image type. This function also calls make_pixel
 *              function for every pixel in the image. Prints the ppm image 
 *              header and then the rgb values for every pixel.
 * Parameter:   model - pointer to type model_t
 */
void make_image(model_t *model) {
///////////////////////////////////////
#ifdef NUM_THREADS
    fprintf(stderr, "===================================================\n");
    fprintf(stderr, "number of cores == %ld\n", NUM_THREADS);
#endif
    //////////////////////////////////
    unsigned char *pixmap = NULL;
    // computer size of output image and malloc() pixmap
    int columns = 0;
    int rows    = 0;

    columns = (model->proj)->win_size_pixel[0];
    rows    = (model->proj)->win_size_pixel[1];

    pixmap = (unsigned char*) Malloc(3 * columns * rows
                * sizeof(unsigned char));

    unsigned char *pixmap_location = pixmap;
    
    int x;
    int y;
    
    for (y = 0; y < rows; y++) {
        for (x = 0; x < columns; x++) {
#ifdef DBG_PIX
    fprintf(stderr, "\n");
    fprintf(stderr,"Pix: ROW = %d COL = %d\n %ld\n", x, y, NUM_THREADS);
#endif
            make_pixel(model, x, rows - y, (pixmap_location + (3 *
                            (y * columns + x))));
        }
    }
    
    /* gonna try a multiple thread version in order to speed things up */
    /* --------------------------------------------------------------- */
/*    int x;
    int y;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < columns; x++) {
#ifdef DBG_PIX
    fprintf(stderr, "\n");
    fprintf(stderr,"Pix: ROW = %d COL = %d\n", x, y);
#endif
            make_pixel(model, x, rows - y, (pixmap_location + (3 *
                            (y * columns + x))));
        }
    } */
/* end of threaded part ---------------------------------------------- */

    // print out header for ppm filei
    fprintf(stderr, "fuck\n");
    fprintf(stdout, "P6 %d %d 255\n", columns, rows);
    fwrite(pixmap, sizeof(unsigned char), columns * rows * 3, stdout);
    free(pixmap);
    pixmap_location = NULL;
    pixmap          = NULL;
}

/*
 * make_pixel -     performs calculations for a pixel
 * Parameters:      model - our virtual world
 *                  x - pixel x coordinates
 *                  y - pixel y coordinates
 *                  pixval - to (r, g, b) in pixmap
 */
void make_pixel(model_t *model, int x, int y, unsigned char *pixval) {

    double *world = Malloc(3 * sizeof(double));
    double *intensity = Malloc(3 * sizeof(double));


    map_pix_to_world(model->proj, x, y, world);
#ifdef DBG_WORLD
    fprintf(stderr, "world coordinates( %f , %f - \n)", world[0], world[1]);
#endif

    //initialize intensity to (0.0, 0.0, 0.0);
    intensity[0] = 0.0;
    intensity[1] = 0.0;
    intensity[2] = 0.0;


    //compute unit vector dir. in the direction from the view_point to world
    double tempvec[3];
    diff3(model->proj->view_point, world, tempvec);
    unitvec(tempvec, tempvec);
#ifdef DBG_AA
    float fragmentx;
    float fragmenty;
    double temp_sample[3];

    for (fragmentx = x; fragmentx < (x + 1.0); fragmentx += 0.5) {
        for (fragmenty = y; fragmenty < (x + 1.0); fragmenty += 0.5) {
            //float coef = 0.25f;
            //recursion_depth = 0;
            ray_trace(model, model->proj->view_point, tempvec, intensity, 0.0, NULL);
        }
        //scale3(0.25, intensity, intensity);
        int i;
        for (i = 0; i < 3; i++) {
            temp_sample[i] = intensity[i] + temp_sample[i];
        }
    }
    scale3(0.25, intensity, intensity);
    int i;
    for (i = 0; i < 3; i++) {
        intensity[i] = temp_sample[i];
    }
#endif

#ifndef DBG_AA
    ray_trace(model, model->proj->view_point, tempvec, intensity, 0.0, NULL);
#endif

/* ----------------Begin of Anti-Aliasing --------------------- */
#ifdef DBG_AA
/*
    int k;
    double temp_intensity[3];
    for (k = 0; k < AA_SAMPLES; k++) {
        map_pix_to_world(model->proj, x, y, world);
        diff3(model->proj->view_point, world, tempvec);
        unitvec(tempvec, tempvec);
        ray_trace(model, model->proj->view_point, tempvec, intensity, 0.0, NULL);
        int i;
        for (i = 0; i < 3; i++) {
            temp_intensity[i] = temp_intensity[i] + intensity[i];
        }
    }
    int i;
    for (i = 0; i < 3; i++) {
        intensity[i] = temp_intensity[i];
    }
    scale3((1.0 / AA_SAMPLES), intensity, intensity);
*/
#endif
/* -----------------end of Anti-Aliasing -------------------------*/

    //vecprn3("intensity before values are clamped", intensity);
    //clamp each element of intensity to the range[0.0, 1.0]
    if (intensity[0] > 1) {
        intensity[0] = 1;
    } else if (intensity[0] < 0) {
        intensity[0] = 0;
    }

    if (intensity[1] > 1) {
        intensity[1] = 1;
    } else if (intensity[1] < 0) {
        intensity[1] = 0;
    }

    if (intensity[2] > 1) {
        intensity[2] = 1;
    } else if (intensity[2] < 0) {
        intensity[2] = 0;
    }

    //set(r,g,b) components of vector pointed to by pixval to 255 *
    //corresponding intensity
    *pixval = MAX_COLOR * intensity[0];//MAX_COLOR * intensity[0];
    *(pixval + 1) = MAX_COLOR * intensity[1];//MAX_COLOR * intensity[1];
    *(pixval + 2) = MAX_COLOR * intensity[2];//MAX_COLOR * intensity[2];

#ifdef DBG_AMB
    fprintf(stderr,"ambient vals %5.11f %5.11f %5.11f\n", intensity[0],
                intensity[1], intensity[2]);
#endif

    free(world);
    free(intensity);
    world = NULL;
    intensity = NULL;
}
