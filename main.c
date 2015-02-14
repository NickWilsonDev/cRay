#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "list.h"
#include "model.h"
#include "image.h"
#include "malloc.h"
#include "projection.h"

/*
 * author Kao Thao and Nick Wilson
 * version 3.17.13
 * main.c - This file contains the main function for the ray tracer.
 * returns:
 * an int
 */
int main(int argc, char **argv) {
    model_t *model = (model_t *) Malloc(sizeof(model_t));
    int rc;

    model->proj = (proj_t*) projection_init(argc, argv, stdin);
    if (model->proj == NULL) {
        free(model);
        exit(EXIT_FAILURE);
    } else {
        //projection_dump(stderr, model->proj);

        model->lights = list_init();
        model->scene  = list_init();

        rc = model_init(stdin, model);
   //     model_dump(stderr, model);

        fprintf(stderr,"rc in main.c is %d\n", rc);
        if (rc == 0) { make_image(model); }

        //fprintf(stderr, "shouldve written image\n");
        free_list(model->scene);
        free_list(model->lights);
        
        model->scene = NULL;
        model->lights = NULL;
        free((model->proj));
        free(model);
        model->proj = NULL;
        model = NULL;
        return (EXIT_SUCCESS);
    }
}
