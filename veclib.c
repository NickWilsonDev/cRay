/**
 * author Nick Wilson and Kao Thao
 * version 3.5.12
 * veclib.c
 *
 * This file will define several functions for performing basic mathimatical
 * operations on vectors. For example addition, subtraction, and dot product.
 * We are assuming that all vectors are three dimensional.
 */

#include <stdio.h>
#include "veclib.h"
#include <math.h>
#include "ray.h"

/**
 * dot3 - Function than computes the dot product of the two vectors passed in 
 * as parameters. Returns a double since mathematically the dot product 
 * produces a scalar and not a vector.
 * parameters
 * *A - pointer to an array that contains components of the first vector
 * *B - pointer to an array that contains components of the second vector
 *
 * return
 * type double - this is the result, it is a double (just a number) rather 
 * than a vector
 */
double dot3(double *A, double *B) {
    double inner_product = 0;
    int i = 0;
    for (; i < 3; i++) {
        inner_product = inner_product + ((A[i]) * (B[i]));
    }
    return inner_product;
}

/**
 * scale3 - Function that multiplies a vector by a scalar. This should result
 * int the resulting vector to appear to have "stretched" or "shrunk" by the 
 * factor represented by the scalefactor parameter.
 * parameters
 * scalefactor - double that is the scalar that the components of the vector
 *                  will be multiplied by
 * *invector   - pointer to an array that stores the components that will be
 *                  "scaled"
 * *outvector  - pointer to an array that will store the components resulting
 *                  from the scaling of the components specified in *invector
 */
void scale3(double scalefactor, double *invector, double *outvector) {
    int i = 0;
    for (; i < 3; i++) {
        outvector[i] = invector[i] * scalefactor;
    }
}

/**
 * length3 - Function finds length or magnitude of vector that was passed in as
 * a parameter.
 * parameter
 * *targetvec - a pointer to an array that contains components of vector that
 *              we wish to find the length of
 */
double length3(double *targetvec) {
    double length;
    length = dot3(targetvec, targetvec);
    return (sqrt(length));
}

/**
 * diff3 - Function simply subtracts two vecors and stores the result within a 
 * third. It uses a for loop to traverse through the components of the vectors 
 * and subtracts these components and stores the result in the array pointed
 * to by v3. 
 * parameters
 * *v1 - pointer to an array of components of the first vector
 * *v2 - pointer to an array of components of the second vector
 * *v3 - pointer to an array that will store the components that result from
 *          the subtraction of the other two vectors
 */

void diff3(double *v1, double *v2, double *v3) {
    int i = 0;
    for (; i < 3; i++) {
        v3[i] = v2[i] - v1[i];
    }
}

/**
 * sum - Function simply adds two vecors and stores the result within a third.
 * It uses a for loop to traverse through the components of the vectors and 
 * adds these components together.
 * parameters
 * *v1 - pointer to an array of components of the first vector
 * *v2 - pointer to an array of components of the second vector
 * *v3 - pointer to an array that will store the components that result from
 *          the addition of the other two vectors
 */
void sum(double *v1, double *v2, double *v3) {
    int i = 0;
    for (; i < 3; i++) {
        v3[i] = v2[i] + v1[i];
    }
}

/**
 * unitvec - Function that takes two parameters, both of type double. The first
 * is the vector that we wish to find the "unit vector" of. The second is where
 * we store the components of the unit vector. It uses the formula of the 
 * unitvector = v(x)/length(v1) + v(y)/length(v1) + v(z)/length(v1)
 * params
 * v1 - a pointer to the vector that we will derive the unit vector from
 * v2 - a pointer to a vector that will contain the unit vectors components
 */
void unitvec(double *v1, double *v2) {
    double tempscalar = length3(v1);
    int i = 0;
    for (; i < 3; i++) {
        v2[i] = v1[i] / tempscalar;
    }
}

/**
 * vecprn3 - Function takes two parameters, a character pointer and a double
 * pointer. It prints out the array of characters that the char pointer points
 * too. And uses a for loop to traverse and print the contents of the array
 * that the double pointer points too.
 * params
 * label - a pointer to an array of characters
 * v     - a pointer to an array of doubles, the contents of which make up the
 *          components of the vector
 */
void vecprn3(char *label, double *v) {
    fprintf(stderr,"%s\n", label);
    int i = 0;
    for (; i < 3; i++) {
        fprintf(stderr,"%f ", v[i]);
    }
    fprintf(stderr,"\n");
}

/*
 * vec_copy3 - function takes two parameters, an input vector that will be 
 * copied and a vector to store the duplicated data.
 *
 * params
 * v1 - a pointer to an array of doubles
 * v2 - a pointer to an array of doubles
 */
void vec_copy3(double *v1, double *v2) {
    int i;
    for(i = 0; i < 3; i++) {
        v2[i] = v1[i];
    }
}

/*
 * angle3 - function takes two vectors and performs several operations with  
 * them. The result of this operation is the angle between the two vectors
 */
double angle3(double *v1, double *v2) {
    double dot = dot3(v1, v2);
    double denominator = (length3(v1) * length3(v2));
    double answer = dot / denominator;

    return (acos(answer));
}

/*
 * cross - function computes the cross product of two input vectors
 *
 * Parameters: v1 - type double, left vector
 *             v2 - type double, right vector
 *             v3 - type double, output vector
 */
void cross(double *v1, double *v2, double *v3) {
    v3[0] = (v1[1] * v2[2] - v1[2] * v2[1]);
    v3[1] = (v1[2] * v2[0] - v1[0] * v2[2]);
    v3[2] = (v1[0] * v2[1] - v1[1] * v2[0]);
}

/*
 * transpose - function takes a 2 dimensional square array and transposes it
 *
 * Parameters: in - original matrix
 *             out - output matrix
 *
 */
void transpose(double in[][3], double out[][3]) {
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            out[i][j] = in[j][i];
        }
    }
}

/*
 * matmult - function multiplies two matrices(2 dimensional arrays that are
 * square, 3 x 3) together
 * WARNING - the two matrices must be 3x3 this function will not work
 *           otherwise
 */
void matmult(double inleft[][3], double inright[][3]) {
    int i;
    int j;
    int k;
    double temp[3][3];

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                temp[i][j] = inleft[i][k] * inright[k][j]; // this may not be right
            }
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            inleft[i][j] = temp[i][j];
        }
    }
}

/*
 * matprn - function should display the contents of an array given as a 
 * parameter.
 */
void matprn(char *label, double mat[3][3]) {
    int i;
    int j;

    fprintf(stderr, "%s\n", label);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            fprintf(stderr, "%lf ", mat[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

/*
 * project - function projects a vector onto a plane
 *
 * Parameters: n - plane normal
 *             v - input vector
 *             w - projected vector
 */
void project(double *n, double *v, double *w) {
    double temp[3];
    double tempstep = dot3(n, v);
    scale3(tempstep, n, temp);
    diff3(temp, v, w);
}

/*
 * xform - function performs a linear transform in four dimensional space by
 * applying a 3 x 3 matrix to a 3 x 1 column vector.
 *
 * Parameters: y - transform matrix
 *             x - input vector
 *             z - output vector
 */
void xform(double y[][3], double *x, double *z) {
    double temp[3];
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        temp[i] = 0;
        for (j = 0; j < 3; j++) {
            temp[i] += y[i][j] * *(x + j);
        }
    }
    scale3(1.0, temp, z);
}

/*
 * reflect - function computes the reflected vector
 *
 * Parameters: unitin - unit vector in the incoming direction
 *             unitnorm - outward surface normal
 *             unitout - unit vector in direction of bounce
 */
void reflect(double *unitin, double *unitnorm, double *unitout) {
    scale3(dot3(unitin, unitnorm), unitnorm, unitnorm);
    scale3(2, unitnorm, unitnorm);
    diff3(unitnorm, unitin, unitout);
}

/*
 * refract - function computes the refracted vector
 *
 * Parameters: entry    - int if neg then we are entering refractive object
 *                        if positive then we are exiting refractive object
 *             unitin   - unit vector in the incoming direction
 *             unitnorm - outward surface normal
 *             unitout  - unit vector of direction of bounce
 *             index    - index of refraction
 */

/////////////// may still be buggy////////////////
void refract(int entry, float index, double *unitin, double *unitnorm, double *unitout) {
    double n;
    double dir_epsilon[3];
    vec_copy3(unitin, dir_epsilon);
    
    fprintf(stderr, "unitin == [%lf, %lf, %lf]\n", unitin[0], unitin[1], unitin[2]);
    scale3(0.001, dir_epsilon, dir_epsilon);
    if (entry == -1) {
         n = AIR / ((double)index);
    } else {
        n = ((double)index) / AIR;
    }

    double c1 =  -1 * dot3(unitnorm, unitin);
    double c2 = sqrt(1 - (n * n) * (1 - (c1 * c1)));
    double tempscale = n * c1 - c2;
    double temp[3];
    double temp2[3];
    scale3(tempscale, unitnorm, temp);
    scale3(n, unitin, temp2);
    sum(temp2, temp, unitout);

    fprintf(stderr, "unitout == [%lf, %lf, %lf]\n", unitout[0], unitout[1], unitout[2]);
    fprintf(stderr, "dir_epsilon[2] == %lf\n", dir_epsilon[2]);
    sum(dir_epsilon, unitout, unitout);

    unitvec(unitout, unitout);
    //double cosI = dot3(unitin, unitnorm);
    //double sinT = n * n * (1.0 - cosI * cosI);

    //if (sinT > 1.0) {
        // vector is invalid
    //    unitout[0] = NAN;
    //    unitout[1] = NAN;
    //    unitout[2] = NAN;
    //} else {
    //    double temp[3] = {0, 0, 0};
    //    double temp2[3] = {0, 0, 0};
    //    scale3(n, unitin, temp);

    //    scale3((n + sqrt(1.0 - sinT)), unitnorm, temp2);
    //    diff3(temp2, temp, unitout);
    //    //////// trying to get rid of refraction acne

   // }

}
