#ifndef VEC3_H
#define VEC3_H

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

#endif
