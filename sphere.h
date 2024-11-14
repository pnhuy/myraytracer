#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "interval.h"

#include <math.h>

typedef struct sphere {
    bool (*hit)(struct sphere *h, ray *r, interval ray_t, hit_record *rec);
    point3 center;
    double radius;
    material *mat;
} sphere;

bool sphere_hit(sphere *s, struct ray *r, interval ray_t, struct hit_record *rec);

sphere sphere_init(point3 center, double radius, material *mat);

#endif
