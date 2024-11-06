#ifndef RT_H
#define RT_H

#include <stdlib.h>

const double infinity = 1.0 / 0.0;
const double pi = 3.1415926535897932385;

double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

/*
 * generate a random real in [0,1)
 */
double random_double() {
    return (float) rand() / (float)(RAND_MAX+1.0);
}

double random_double_range(double min, double max) {
    return min + (max-min) * random_double();
}

#endif
