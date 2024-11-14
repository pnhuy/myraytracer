#include "hittable.h"

void set_face_normal(hit_record *h, const ray *r, vec3 *outward_normal) {
    h->front_face = vec3_dot(r->direction, *outward_normal) < 0;
    h->normal = h->front_face ? *outward_normal : vec3_negate(*outward_normal);
}

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
    double cos_theta = fmin(vec3_dot(vec3_negate(unit_direction), rec->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double())
        direction = vec3_reflect(unit_direction, rec->normal);
    else
        direction = vec3_refract(unit_direction, rec->normal, ri);

    *scattered = (ray){rec->p, direction};
    return true;
}


double reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
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
