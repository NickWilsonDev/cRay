#ifndef RAY_H
#define RAY_H

/* Object types */

#define FIRST_TYPE   10
#define LIGHT        10
#define SPOTLIGHT    11
#define PROJECTOR    12
#define SPHERE       13
#define PLANE        14
#define FINITE_PLANE 15
#define TILED_PLANE  16
#define TEX_PLANE    17
#define REF_SPHERE   18
#define P_SPHERE     19
#define P_PLANE      20
#define PARABOLOID   21
#define CYLANDER     22
#define CONE         23
#define HYPERBOLOID  24
#define TORUS        25
#define TEX_SPHERE   26
#define TRIANGLE     27
#define DISP_SPHERE  28
#define LAST_TYPE    29
//#define LAST_LIGHT   12

#define RECURSION_MAX 30
extern int recursion_depth;

/* indices of refraction */
/* retrieved from hyperphysics.phy-astr.gsu.edu/hbase/tables/indrf.html */
#define WATER        1.33
#define ICE          1.31
#define CROWN_GLASS  1.52 /* typical crown glass */
#define AIR          1.0

#define TEX_FIT       1
#define TEX_TILE      2

#define True          1
#define False         0
#define MAXCHAR       256

#define PI 3.14
#define RGB_COMPONENT_COLOR 255
#endif
