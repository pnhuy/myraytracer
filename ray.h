#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct ray {
    point3 origin;
    vec3 direction;
} ray;

point3 ray_at(ray *r, double t);

#endif
