#include "rt.h"
#include <stdlib.h>
#include <math.h>

const double infinity = 1.0 / 0.0;
const double pi = 3.1415926535897932385;

double random_double_range(double min, double max) {
    return min + (max - min) * random_double();
}

double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

double radians_to_degrees(double r) {
    return r * 180.0 / pi;
}