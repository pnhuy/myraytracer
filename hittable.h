#ifndef HITTABLE_H
#define HITTABLE_H

#include "color.h"
#include "ray.h"
#include <stdbool.h>
#include <stdio.h>

/*
 * HIT RECORD
 */

typedef enum material_type {
    lambertian,
    metal,
} material_type;

typedef struct material {
    material_type type;
    color albedo;
} material;

typedef struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    material mat;
    bool (*scatter)(ray *r_in, struct hit_record *rec, color *attenuation, ray *scattered);
} hit_record;

void set_face_normal(hit_record *h, const ray *r, vec3 *outward_normal) {
    h->front_face = vec3_dot(r->direction, *outward_normal);
    h->normal = h->front_face ? *outward_normal : vec3_scale(*outward_normal, -1);
}

/*
 * MATERIAL
 */

bool lambertian_scatter(ray *r_in, hit_record *rec, color *attenuation, ray *scattered) {
    vec3 scatter_direction = vec3_add(rec->normal, vec3_random_unit_vector());

    if (vec3_near_zero(scatter_direction)) {
        scatter_direction = rec->normal;
    }

    *scattered = (ray){rec->p, scatter_direction};
    *attenuation = rec->mat.albedo;
    return true;
}

bool metal_scatter(ray *r_in, hit_record *rec, color *attenuation, ray *scattered) {
    vec3 reflected = vec3_reflect(r_in->direction, rec->normal);
    *scattered = (ray){rec->p, reflected};
    *attenuation = rec->mat.albedo;
    return true;
}

void set_material(material *mat, hit_record *rec) {
    rec->mat = *mat;
    if (mat->type == lambertian) {
        rec->scatter = &lambertian_scatter;
    } else if (mat->type == metal) {
        rec->scatter = &metal_scatter;
    }
}

#endif
