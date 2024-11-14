#ifndef VEC3_H
#define VEC3_H

#include <stdbool.h>

typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;

typedef vec3 point3;

char *vec3_repl(vec3 u);

double vec3_length_squared(vec3 vec);

double vec3_length(vec3 vec);

vec3 vec3_add(vec3 v1, vec3 v2);

vec3 vec3_subtract(vec3 v1, vec3 v2);

vec3 vec3_scale(vec3 v1, double k);

vec3 vec3_div(vec3 v1, double k);

vec3 vec3_unit_vector(vec3 u);

double vec3_dot(vec3 v1, vec3 v2);

vec3 vec3_hadamard_prod(vec3 u, vec3 v);

vec3 vec3_cross(vec3 u, vec3 v);

vec3 vec3_random();

vec3 vec3_random_range(double min, double max);

vec3 vec3_random_unit_vector();

vec3 vec3_random_on_hemisphere(vec3 *normal);

bool vec3_near_zero(vec3 u);

vec3 vec3_reflect(vec3 v, vec3 n);

vec3 vec3_negate(vec3 u);

vec3 vec3_refract(vec3 uv, vec3 n, double etai_over_etat);

vec3 random_in_unit_disk();

#endif
