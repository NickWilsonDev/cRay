#ifndef IMAGE_H
#define IMAGE_H

#include "structs.h"

/**
 * image.h
 */

/*
 * make_image - Function that will print out all information necessary for 
 *              making a ppm image type. This function also calls make_pixel
 *              function for every pixel in the image. Prints the ppm image 
 *              header and then the rgb values for every pixel.
 * Parameter:   model - pointer to type model_t
 */
void make_image(model_t *model);

/*
 * Parameters:      model - ??
 *                  x - pixel x coordinates
 *                  y - pixel y coordinates
 *                  pixval - to (r, g, b) in pixmap
 */
void make_pixel(model_t *model, int x, int y, unsigned char *pixval);

#endif
