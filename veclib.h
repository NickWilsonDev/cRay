#ifndef VECLIB_H
#define VECLIB_H

/**
 * author Nick Wilson
 * version 3.5.12
 * veclib.h
 *
 * This header file simply contains the function prototypes for the veclib.c
 * file.
 */

double dot3(double *a, double *b);

void scale3(double, double *v1, double *v2);

double length3(double *v1);

void diff3(double *v1, double *v2, double *v3);

void sum(double *v1, double *v2, double *v3);

void unitvec(double *v1, double *v2);

void vecprn3(char *label, double *v1);

void vec_copy3(double *v1, double *v2);

double angle3(double *v1, double *v2);

void cross(double *v1, double *v2, double *v3);

void transpose(double in[][3], double out[][3]);

void matmult(double inleft[][3], double inright[][3]);

void matprn(char *label, double mat[3][3]);

void project(double *n, double *v, double *w);

void xform(double[][3], double *x, double *z);

void reflect(double *unitin, double *unitnorm, double *unitout);

void refract(int entry, float index, double *unitin, double *unitnorm, double *unitout);
#endif
