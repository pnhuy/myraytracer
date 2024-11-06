#include "ray.h"

point3 ray_at(ray *r, double t) {
    return vec3_add(r->origin, vec3_scale(r->direction, t));
}
