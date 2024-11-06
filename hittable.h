#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"

#include <stdbool.h>
#include <math.h>
#include <stdio.h>

typedef struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
} hit_record;

void set_face_normal(hit_record *h, const ray *r, vec3 *outward_normal) {
    h->front_face = vec3_dot(r->direction, *outward_normal);
    h->normal = h->front_face ? *outward_normal : vec3_scale(*outward_normal, -1);
}

typedef struct sphere {
    bool (*hit)(struct sphere *h, ray *r, interval ray_t, hit_record *rec);
    point3 center;
    double radius;
} sphere;

bool sphere_hit(sphere *s, struct ray *r, interval ray_t, struct hit_record *rec) {
    vec3 oc = vec3_subtract(s->center, r->origin);
    double a = vec3_length_squared(r->direction);
    double h = vec3_dot(r->direction, oc);
    double c = vec3_length_squared(oc) - s->radius * s->radius;

    double discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;
    
    double sqrtd = sqrt(discriminant);

    double root = (h - sqrtd) / a;
    if (!interval_surrounds(&ray_t, root)) {
        root = (h + sqrtd) / a;
        if (!interval_surrounds(&ray_t, root)) {
            return false;
        }
    }

    rec->t = root;
    rec->p = ray_at(r, root);
    vec3 outward_normal = vec3_div(vec3_subtract(rec->p, s->center), s->radius);
    set_face_normal(rec, r, &outward_normal);

    return true;
}

sphere sphere_init(point3 center, double radius) {
    sphere s;

    s.center = center;
    s.radius = radius;
    s.hit = sphere_hit;

    return s;
}

#endif
