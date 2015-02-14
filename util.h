#ifndef UTIL_H
#define UTIL_H

#include "ray.h"

void quartic_solver(float coefficient[5], float *x1, float *x2, float *x3,
                    float *x4);

void cubic_solver(float coefficient[4], float *x1, float *x2, float *x3);

int check_cubic(float coefficient[4]);

int check_quartic(float coefficient[5]);

void get_3_doubles(double *tar1, double *tar2, double *tar3);

void get_2_doubles(double *tar1, double *tar2);

void get_1_double(double *tar1);
#endif
