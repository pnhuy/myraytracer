#include "vec3.h"
#include "rt.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char *vec3_repl(vec3 v) {
    char *result = malloc(50 * sizeof(char)); // Allocate enough memory for the string
    if (result == NULL) {
        return NULL; // Handle memory allocation failure
    }
    snprintf(result, 50, "(%f, %f, %f)", v.x, v.y, v.z);
    return result;
}

double vec3_length_squared(vec3 vec) {
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

double vec3_length(vec3 vec) { return sqrt(vec3_length_squared(vec)); }

vec3 vec3_add(vec3 v1, vec3 v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

vec3 vec3_subtract(vec3 v1, vec3 v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

vec3 vec3_scale(vec3 v1, double k) {
    v1.x *= k;
    v1.y *= k;
    v1.z *= k;
    return v1;
}

vec3 vec3_div(vec3 v, double k) {
    return vec3_scale(v, 1 / k);
}

double vec3_dot(vec3 v1, vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 vec3_cross(vec3 u, vec3 v) {
    vec3 c;
    c.x = u.y * v.z - u.z * v.y;
    c.y = u.z * v.x - u.x * v.z;
    c.z = u.x * v.y - u.y * v.x;
    return c;
}

vec3 vec3_hadamard_prod(vec3 u, vec3 v) {
    vec3 r;
    r.x = u.x * v.x;
    r.y = u.y * v.y;
    r.z = u.z * v.z;
    return r;
}

vec3 vec3_unit_vector(vec3 v) {
    return vec3_div(v, vec3_length(v));
}

bool vec3_equal(vec3 u, vec3 v) {
    return (u.x == v.x && u.y == v.y && u.z == v.z);
}

vec3 vec3_random() {
    vec3 r = {random_double(), random_double(), random_double()};
    return r;
}

vec3 vec3_random_range(double min, double max) {
    vec3 r = {random_double_range(min, max), random_double_range(min, max), random_double_range(min, max)};
    return r;
}

vec3 vec3_random_unit_vector() {
    while (true) {
        vec3 p = vec3_random_range(-1, 1);
        double lensq = vec3_length_squared(p);
        if (1e-160 < lensq && lensq <= 1)
            return vec3_div(p, vec3_length(p));
    }
}

vec3 vec3_random_on_hemisphere(vec3 *normal) {
    vec3 on_unit_sphere = vec3_random_unit_vector();
    if (vec3_dot(on_unit_sphere, *normal) > 0.0) {
        return on_unit_sphere;
    } else {
        return vec3_scale(on_unit_sphere, -1);
    }
}

bool vec3_near_zero(vec3 e) {
    double s = 1e-8;
    return (fabs(e.x) < s) && (fabs(e.y) < s) && (fabs(e.z) < s);
}

vec3 vec3_negate(vec3 u) {
    return vec3_scale(u, -1.0);
}

vec3 vec3_reflect(const vec3 v, const vec3 n) {
    return vec3_subtract(v, vec3_scale(n, 2 * vec3_dot(v, n)));
}


vec3 vec3_refract(const vec3 uv, const vec3 n, double etai_over_etat) {
    double cos_theta = fmin(vec3_dot(vec3_negate(uv), n), 1.0);
    vec3 r_out_perp = vec3_scale(vec3_add(uv, vec3_scale(n, cos_theta)), etai_over_etat);
    vec3 r_out_parallel = vec3_scale(n, -sqrt(fabs(1.0 - vec3_length_squared(r_out_perp))));
    vec3 r = vec3_add(r_out_perp, r_out_parallel);
    return r;
}