#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ray.h"
#include "projection.h"
#include "malloc.h"

/* needed static pointer for texture mapping */
static proj_t *p;

proj_t *projection_init(int argc, char **argv, FILE *in) {
    proj_t *proj = NULL;
    proj = Malloc(sizeof(proj_t));
    
    p = proj; // for texture mapping
    char buf[MAXCHAR];

    if (argc != 3) {
        fprintf(stderr, "Usage <executable> <x-window> <y-window>");
        // gonna free some stuff just to be tidy
        free(proj);
        proj = NULL;
        return NULL;
    }
    proj->win_size_pixel[0] = atoi(argv[1]);
    proj->win_size_pixel[1] = atoi(argv[2]);

    scanf("%lf %lf %lf", &proj->win_size_world[0], &proj->win_size_world[1],
            &proj->win_size_world[2]);
    fgets(buf, MAXCHAR, in);
    scanf("%lf %lf %lf", &proj->view_point[0], &proj->view_point[1],
            &proj->view_point[2]);
    fgets(buf, MAXCHAR, in);
    fgets(buf, MAXCHAR, in);
   
    p = proj; // so we can access this for texure mapping 
    return proj;
}

int projection_dump(FILE *out, proj_t *proj) {
    fprintf(out, "Scene data - \n");
    fprintf(out, "Window size pixel - \t%d \t%d \n", proj->win_size_pixel[0],
            proj->win_size_pixel[1]);
    fprintf(out, "Window size world - \t%lf \t%lf \t%lf \n",
            proj->win_size_world[0], proj->win_size_world[1],
            proj->win_size_world[2]);
    fprintf(out, "View point - \t%lf \t%lf \t%lf \n\n", proj->view_point[0],
            proj->view_point[1], proj->view_point[2]);

    return 0;
}

/*
 * map_pix_to_world - map a pixel coordinate to a world coord
 *
 * Parameters:   proj - pointer to a project definition
 *               x, y - x and y pixel coordinates
 *               world - pointer to three doubles
 *
 */
void map_pix_to_world(proj_t *proj, int x, int y, double *world) {
    *(world + 0) = (double)x / (proj->win_size_pixel[0] - 1) * 
                    proj->win_size_world[0];
    *(world + 0) -= proj->win_size_world[0] / 2.0;

    *(world + 1) = (double)y / (proj->win_size_pixel[1] - 1) *
                    proj->win_size_world[1];
    *(world + 1) -= proj->win_size_world[1] / 2.0;

    *(world + 2) = 0.0;
}

// assuming i need to map world coordinate to pixel coordinate?
void map_world_to_pix(double *lasthit, double *pixhit) {
     //pixhit[0] =  lasthit[0] * ((p->win_size_pixel[0]) - 1) / (p->win_size_world[0]);
     //pixhit[0] += ((p->win_size_world[0]) / 2);

     //pixhit[1] =  lasthit[1] * ((p->win_size_pixel[1]) - 1) / (p->win_size_world[1]);
     //pixhit[1] += ((p->win_size_world[1]) / 2);
    pixhit[0] = lasthit[0] / p->win_size_world[0];

    pixhit[1] = lasthit[1] / p->win_size_world[1];
}

double randpix(int x) {
    double new_x;
    srand(x);
    new_x = (double) (rand() % 256);
    return new_x;
    //return 1;
}
