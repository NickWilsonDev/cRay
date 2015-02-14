#include <stdio.h>
#include <stdlib.h>
#include "perlin.c"

int main() {
    int i = 0;
    double noise;
    double x;
    init_noise();
    while (i < 100) {
        init_noise();
        x = (double)i;
        noise = pnoise(x, x, -x);
        printf("i == %d  noise == %lf\n", i, noise);
        i++;
    }
    return 0;
}
