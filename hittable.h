#ifndef HITTABLE_H
#define HITTABLE_H

#include "color.h"
#include "ray.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

/*
 * HIT RECORD
 */

typedef enum material_type {
    lambertian,
    metal,
    dielectric,
} material_type;

typedef struct material {
    material_type type;
    color albedo;
    double fuzz;
    double refraction_index;
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
    h->front_face = vec3_dot(r->direction, *outward_normal) < 0;
    h->normal = h->front_face ? *outward_normal : vec3_negate(*outward_normal);
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
    reflected = vec3_add(vec3_unit_vector(reflected), vec3_scale(vec3_random_unit_vector(), rec->mat.fuzz));
    *scattered = (ray){rec->p, reflected};
    *attenuation = rec->mat.albedo;
    return (vec3_dot(scattered->direction, rec->normal) > 0);
}

bool dielectric_scatter(ray *r_in, hit_record *rec, color *attenuation, ray *scattered) {
    *attenuation = (color){1.0, 1.0, 1.0};
    double ri = rec->front_face ? (1.0 / rec->mat.refraction_index) : rec->mat.refraction_index;
    
    vec3 unit_direction = vec3_unit_vector(r_in->direction);
    vec3 direction = vec3_refract(unit_direction, rec->normal, ri);

    *scattered = (ray){rec->p, direction};
    return true;
    
}

void set_material(material *mat, hit_record *rec) {
    rec->mat = *mat;
    if (mat->type == lambertian) {
        rec->scatter = &lambertian_scatter;
    } else if (mat->type == metal) {
        rec->scatter = &metal_scatter;
    } else if (mat->type == dielectric) {
        rec->scatter = &dielectric_scatter;
    }
}

#endif
