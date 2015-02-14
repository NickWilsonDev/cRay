/**
 * author Nick Wilson and Kao Thao
 * version 3.21.13
 * structs.h
 *
 * This header file will contain most of the structs used in the raytracer.
 *
 * ----------------we are allowed to copy these structs into ray.h---------
 */

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct projection_type {
    int win_size_pixel[2];
    double win_size_world[3];
    double view_point[3];
}proj_t;

typedef struct material_type {
    double ambient[3]; /* Reflectivity for materials */
    double diffuse[3];
    double specular[3];
}material_t;

typedef struct obj_type {
    struct obj_type *next; /* Next object in list */
    int objid; /* Numeric serial # for debug */
    int objtype; /* Type code (14 -> Plane ) */
    float refraction_index;

    /* hits function */
    double (*hits)(double *base, double *dir, struct obj_type *);

    /* Optional plugins for retrieval of reflectivity */
    /* useful for the ever-popular tiled floor */
    void (*getamb)(struct obj_type *, double *);

    void (*getdiff)(struct obj_type *, double *);
    void (*getspec)(struct obj_type *, double *);

    void (*getnorm)(struct obj_type *, double *); //this if for diffuse norm

    void (*getloc)(struct obj_type *, double *);
    int (*illum_check)(struct obj_type *, double *);

    /* Reflectivity for reflective objects */
    material_t material;

    /* These fields used only in illuminating objects (lights) */
    void (*getemiss)(struct obj_type *, double *);
    double emissivity[3]; //For lights

    void *priv; /* Private type-dependent data */

    double hitloc[3]; /* Last hit point */
    double normal[3]; /* Normal at hit point */
} obj_t;

typedef struct list_type {
    obj_t *first;
    obj_t *last;
}list_t;

typedef struct model_type {
    proj_t *proj;
    list_t *lights;
    list_t *scene;
}model_t;

/* Infinite plane */
typedef struct plane_type {
    double point[3]; /* A point on the plane */
    double normal[3]; /* A normal vector to the plane */

    void *priv; /* Data for specialized types(finite plane ect.) */
} plane_t;

/* Sphere */
typedef struct sphere_type {
    double center[3];
    double radius;
} sphere_t;

/* Disk */
typedef struct disk_type {
    double center [3];
    double radius;
    double normal[3]; /* normal vector to the disk // similar to plane */
} disk_t;

/* Point light source */
typedef struct light_type {
    double emissivity[3];
    double location[3];

    void *priv; /* Data for specialized types of lights(spotlights ect.) */
} light_t;

/* finite plane */
typedef struct fplane_type {
    double xdir[3];         /* x axis direction */
    double size[2];         /* width x height */
    double rotmat[3][3];    /* rotation matrix */
    double lasthit[2];      /*used for textures */
    void *priv;             /* data for specialized types */
} fplane_t;

/* triangle */
typedef struct tri_type {
    double point1[3];
    double point2[3];
    double point3[3];
} tri_t;

/* tiled plane */
typedef struct tplane_type {
    double xdir[3];          /* orientation of the tiling */
    double size[2];          /* size of the tiling */
    double rotmat[3][3];     /* rotation matrix */
    material_t background;   /* background color */
} tplane_t;

/* spotlights */
typedef struct spotlight_type {
    double direction[3];     /* cone centerline vector */
    double theta;            /* half-width of cone in deg */
    double costheta;         /* precomputed cos(theta) */
} spotlight_t;

/* texture - used by texplane_t  */
typedef struct texture_type {
    int size[2];             /* x dim - y dim */
    unsigned char *texbuf;   /* pixmap buffer */
} texture_t;

/* textured plane */
typedef struct texplane_type {
    int texmode;             /* 1 -> fit and 2-> tile */
    char texname[40];        /* name of .ppm file */
    texture_t *texture;      /* pointer to texture struct */
} texplane_t;

/* cylander */
typedef struct cyl_type {
    double center[3];        /* given in file, center point of base */
    double centerline[3];    /* given */
    double radius;           /* given */
    double height;           /* given */
    double rotmat[3][3];
    double irot[3][3];
} cyl_t;

/* paraboloid */
typedef struct parab_type {
    double center[3];         /* center of base */
    double centerline[3];     /*  */
    double radius;            /* radius of base */
    double height;
    double scale;             /* r^2 / h */
    double rotmat[3][3];
    double irot[3][3];
} parab_t;

/* cone */
typedef struct cone_type {
    double center[3];         /* center of base */
    double centerline[3];
    double radius;            /* radius of base */
    double height;
    double rotmat[3][3];
    double irot[3][3];
    double k;
} cone_t;

/* hyperboloid */
typedef struct hyperb_type {
    double center[3];          /* center of base */
    double centerline[3];
    double radius;             /* radius of base */
    double height;
    double scale;              /* r^2 /h */
    double rotmat[3][3];
    double irot[3][3];
} hyperb_t;

/* projection plane */
typedef struct projplane_type {
    double flen;               /* focal len */
    double center[3];          /* light source */
} projplane_t;

/* torus */
typedef struct torus_type {     /* type 24 */
    double center[3];           /* center of torus */
    double axis_revolution[3];  /* unit vector pointing through the middle
                                   of the torus */
    double major_radius;
    double minor_radius;
} torus_t;

/* structs for reading ppm image to get textures ect. */
typedef struct {
    unsigned char red, green, blue;
} PPMPixel;

typedef struct {
    int x, y;
    PPMPixel *data;
} PPMImage;

/* texsphere */
typedef struct texsphere_type {
    
    double center[3];
    double radius;
    PPMImage *image;    
} texsphere_t;

/* displacement sphere */
typedef struct dispsphere_type {
    double center[3];
    double radius;
    PPMImage *image;
} dispsphere_t;

#endif
