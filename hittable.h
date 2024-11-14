#ifndef HITTABLE_H
#define HITTABLE_H

#include "color.h"
#include "ray.h"
#include "rt.h"
#include <math.h>
#include <stdbool.h>

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

void set_face_normal(hit_record *h, const ray *r, vec3 *outward_normal);

bool lambertian_scatter(ray *r_in, hit_record *rec, color *attenuation, ray *scattered);

bool metal_scatter(ray *r_in, hit_record *rec, color *attenuation, ray *scattered);

double reflectance(double cosine, double refraction_index);

bool dielectric_scatter(ray *r_in, hit_record *rec, color *attenuation, ray *scattered);

void set_material(material *mat, hit_record *rec);

#endif
