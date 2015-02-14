/*
 * @author Nick Wilson
 * @version 7.31.13
 * util.c - This file contains several functions that aid in various tasks
 */
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "util.h"

/* quartic_solver - function should solve a quartic equation that is of the 
 *                  form   Ax^4 + Bx^3 + Cx^2 + Dx + E
 * param : coefficient - an array of size 5 containing the coefficients of the
 *                       equation
 *
 * return : void
 */
void quartic_solver(float coefficient[5], float *x1, float *x2, float *x3,
                    float *x4) {
    /* first we simplify the quartic by dividing each term by the first term */
    float a = 1.0;
    float b = coefficient[1] / coefficient[0];
    float c = coefficient[2] / coefficient[0];
    float d = coefficient[3] / coefficient[0];
    float e = coefficient[4] / coefficient[0];

    /* define f, g, and h these are intermediary variables to make things
     * simpler
     */
    float f = c - (3 * (b * b) / 8);

    float g = d + ((b * b * b) / 8) - (b * (c / 2));

    float h = e - (3 * (b * b * b * b) / 256) + ((b * b) * (c / 16));
          h = h - (b * (d / 4));

    /* f, g, and h are then "plugged in" to a cubic equation */
    /* y^3 + (f/2)y^2 + ((f^2 - 4h)/16)y - g^2/64 = 0 */
    /* these cubic coefficients will then be sent to the cubic solver */
    float y1 = 1;
    float y2 = (f / 2);
    float y3 = (((f * f) - (4 * h)) / 16);
    float y4 = -(g * g) / 64;

    float coef[4] = {y1, y2, y3, y4};
    float root1;
    float root2;
    float root3;
    
    if (check_cubic(coef) == 3) {
        cubic_solver(coef, &root1, &root2, &root3);
    
        float p;
        float q;

        float test1 = fabs(root1);
        float test2 = fabs(root2);
        float test3 = fabs(root3);

        if (test1 <= 0.0001) {
            printf("root1 was zero\n");
            p = sqrtf(root2);
            q = sqrtf(root3);
        } else if (test2 <= 0.0001) {
            printf("root2 must have been zero \n");
            p = sqrtf(root1);
            q = sqrtf(root3);
        } else if (test3 <= 0.0001) {
            printf("root3 was zero\n");
            p = sqrtf(root1);
            q = sqrtf(root2);
        }

        float r = -(g / (8 * p * q));

        float s = (b / (4 * a));

        *x1 = p + q + r - s;
        *x2 = p - q - r - s;
        *x3 = -p + q - r - s;
        *x4 = -p - q + r - s;
    } else { }


}

/* check_quartic - a function that takes an array containing the coefficients
 *                 of the quartic as a parameter. It performs tests on the 
 *                 coefficients, that were described on the wikipedia page for
 *                 quartic equations, in order to determine the nature of the 
 *                 roots. The nature of the roots(solutions) to the quartic
 *                 would involve whether the solutions are real or complex, 
 *                 and how many of the roots fit into each of these categories.
 *                 
 * param : coefficient - an array of the coefficients of the quartic equation
 *
 * return : int 
 *              4 - means 4 real roots
 *              2 - means 2 real roots, 2 complex
 *              0 - means 0 real roots, 4 complex
 *              -1 - means something went wrong
 */
int check_quartic(float coefficient[5]) {
    float a = coefficient[0];
    float b = coefficient[1];
    float c = coefficient[2];
    float d = coefficient[3];
    float e = coefficient[4];

    /* derived values that come up often in the formulas */
    float a_2 = (a * a);
    float a_3 = (a * a * a);
    float b_2 = (b * b);
    float b_3 = (b * b * b);
    float c_2 = (c * c);
    float c_3 = (c * c * c);
    float d_2 = (d * d);
    float d_3 = (d * d * d);
    float e_2 = (e * e);
    float e_3 = (e * e * e);

    float delta = 256 * a_3 * e_3 - 192 * a_2 * b * d * e_2 - 128 * a_2 * c_2
                  * e_2 + 144 * a_2 * c * d_2 * e - 27 * a_2 * (d_2 * d_2);

    delta = delta + 144 * a * b_2 * c *e_2 - 6 * a * b_2 * d_2 * e - 80 * a *
            b * c_2 * d * e + 18 * a * b * c * d_3 + 16 * a * (c_2 * c_2) * e
            - 4 * a * c_3 * d_2 - 27 * (b_2 * b_2) * e_2 + 18 * b_3 * c * d *
            e - 4 * b_3 * d_3 - 4 * b_2 * c_3 * e + b_2 * c_2 * d_2;

    float P = 8 * a * c - 3 * b_2;

    float delta_subzero = c_2 - 3 * b * d + 12 * a * e;

    float D = 64 * a_3 * e - 16 * a_2 * c_2 + 16 * a * b_2 * c - 16 * a_2 * b
              * d - 3 * (b_2 * b_2);
    
    // now to perform the tests with if elses
    if (delta < 0) {
        return 2;
    } else if (delta > 0) {
        if (P < 0 && D < 0) {
            return 4;
        } else {
            return 0;
        }
    } else if (delta == 0) {
        if (P < 0 && D < 0 && delta_subzero != 0) {
            return 4;
        } else if (P > 0 || D > 0) {
            return 2;
        } else if (delta_subzero == 0 && D != 0) {
            return 4;
        } else if (D == 0) {
            if (P < 0) {
                return 2;
            } else if (P > 0) {
                return 0;
            } else if (delta_subzero == 0) {
                return 4;
            }
        }
    }
    return -1;
}

/* cubic_solver - function should solve a cubic eqaution that is of the form
 *                Ax^3 + Bx^2 + Cx + D
 *                The website "www.1728.org/cubic2.htm" was helpful for this.
 * param : coefficient - an array of size 5 containing the coefficients of the
 *                       equation
 *         x1 - pointer to a float
 *         x2 - pointer to a float
 *         x3 - pointer to a float
 *
 * return : an array of floats containing the solution or solutions to the
 *          cubic
 */
void cubic_solver(float coefficient[4], float *x1, float *x2, float *x3) {
    float a = 1;
    float b = coefficient[1] / coefficient[0];
    float c = coefficient[2] / coefficient[0];
    float d = coefficient[3] / coefficient[0];

    int test = check_cubic(coefficient);

    if (test == 1) {
        float f = (3 * c / a) - ((b * b) / (a * a));
        f = f / 3;

        float g = (2 * (b * b * b) / (a * a * a)) - (9 * b * (c / (a * a)));
        g = g + (27 * (d / a));
        g = g / 27;

        float h = (g * g) / 4;
        h = h + ((f * f * f) / 27);

        float R = -(g / 2) + sqrtf(h);

        float S = cbrtf(R);

        float T = -(g / 2) - sqrtf(h);

        float U = cbrtf(T);

        float X = (S + U) - (b / (3 * a));
        
        /* X contains the value for the only real solution of the cubic, we
         * will not ignore solutions that involve imaginary numbers in this
         * program
         */
        //float answer[3] = {X, NAN, NAN};
        *x1 = X;
        *x2 = 23; // this was = to blank before

    } else if (test == 3 || test == 0) {
        float f = (3 * c / a) - ((b * b) / (a * a));
        f = f / 3;

        float g = (2 * (b * b * b) / (a * a * a)) - (9 * b * (c / (a * a)));
        g = g + (27 * (d / a));
        g = g / 27;

        float h = (g * g) / 4;
        h = h + ((f * f * f) / 27);

        float i = sqrtf(((g * g) / 4) - h);

        float j = cbrtf(i);

        float K = acosf(-(g / (2 * i)));

        float L = j * (-1);

        float M = cosf((K / 3));

        float N = (sqrtf(3) * sinf((K / 3)));

        float P = (b / (3 * a) * (-1));

        float X1 = (2 * j) * cosf((K / 3)) - (b / (3 * a));

        float X2 = L * (M + N) + P;

        float X3 = L * (M - N) + P;

        /* X 1 through 3 contain the real solutions to the cubic. */
        //float answer[3];
        *x1 = X1;
        *x2 = X2;
        *x3 = X3;

        //return answer;
    }

}

/*
 * check_cubic - function checks a cubic to see whether it has one real
 *               solution, three real equal solutions, or three real solutions.
 * param : coefficient - an array of size 4 containing the coefficients of the
 *                       equation.
 *
 * return int - 1 means one real solution
 *              0 means all solutions are equal to one another
 *              3 means 3 real solutions
 *              -1 means something went wrong
 */
int check_cubic(float coefficient[4]) {
    float a = coefficient[0];
    float b = coefficient[1];
    float c = coefficient[2];
    float d = coefficient[3];

    float f = (3 * c / a) - ((b * b) / (a * a));
    f = f / 3;

    float g = (2 * (b * b * b) / (a * a * a)) - (9 * b * (c / (a * a)));
          g = g + (27 * (d / a));
          g = g / 27;

    float h = (g * g) / 4;
          h = h + ((f * f * f) / 27);

    if (h > 0)
        return 1;

    if (h == 0 && f == 0 && g == 0)
        return 0;

    if (h <= 0)
        return 3; // this means there are 3 real solutions

    return -1;
}



/*
 * Function scans input file and stores next three doubles into the proper
 * locations, that were specified by the parameter.
 */
void get_3_doubles(double *target1, double *target2, double *target3) {
    while (scanf("%lf %lf %lf", target1, target2, target3) != 3) {
        scanf("%*s");
    }
    scanf("%*s");
}


/*
 * Function scans input file and stores next two doubles into the proper
 * locations, that were specified by the parameter.
 */
void get_2_doubles(double *target1, double *target2) {
    while (scanf("%lf %lf", target1, target2) != 2) {
        scanf("%*s");
    }
    scanf("%*s");
}

/*
 * Function scans input file and stores next double into the proper
 * location, that were specified by the parameter.
 */
void get_1_double(double *target1) {
    while (scanf("%lf", target1) != 1) {
        scanf("%*s");
    }
    scanf("%*s");
}


