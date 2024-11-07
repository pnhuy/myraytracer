#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "rt.h"
#include "vec3.h"
#include <stdio.h>

typedef struct camera {
    double aspect_ratio;
    int image_width;
    int samples_per_pixel;
    double pixel_samples_scale;
    int max_depth;
    int image_height;   // Rendered image height
    point3 center;      // Camera center
    point3 pixel00_loc; // Location of pixel 0, 0
    vec3 pixel_delta_u; // Offset to pixel to the right
    vec3 pixel_delta_v; // Offset to pixel below
} camera;

camera camera_create(double aspect_ratio, int image_width, int samples_per_pixel, int max_depth) {
    camera c;
    c.aspect_ratio = aspect_ratio;
    c.image_width = image_width;
    c.image_height = (int)(c.image_width / c.aspect_ratio);
    c.image_height = (c.image_height < 1) ? 1 : c.image_height;

    c.samples_per_pixel = samples_per_pixel;
    c.pixel_samples_scale = 1.0 / c.samples_per_pixel;

    c.max_depth = max_depth;

    point3 center = {0, 0, 0};
    c.center = center;

    // Determine viewport dimensions
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)(c.image_width) / c.image_height);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = {viewport_width, 0, 0};
    vec3 viewport_v = {0, -viewport_height, 0};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    c.pixel_delta_u = vec3_div(viewport_u, c.image_width);
    c.pixel_delta_v = vec3_div(viewport_v, c.image_height);

    // Calculate the location of the upper left pixel.
    vec3 viewport_upper_left = c.center;
    vec3 focal_vec3 = {0, 0, focal_length};
    viewport_upper_left = vec3_subtract(viewport_upper_left, focal_vec3);
    viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_div(viewport_u, 2.0));
    viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_div(viewport_v, 2.0));
    c.pixel00_loc = vec3_add(viewport_upper_left,
                             vec3_scale(vec3_add(c.pixel_delta_u, c.pixel_delta_v), 0.5));

    return c;
}

color camera_ray_color(camera *cam, ray *r, int depth, hittable_list *world) {
    color black = {0, 0, 0};
    color white = {1, 1, 1};
    if (depth <= 0) {
        return black;
    }

    hit_record rec;
    color c = {1, 1, 1};
    interval i = {0.001, INFINITY};
    if (hittable_list_hit(world, r, i, &rec)) {
        ray scattered;
        color attenuation;

        if ((*(rec.scatter))(r, &rec, &attenuation, &scattered)) {
            vec3 r = vec3_hadamard_prod(attenuation, camera_ray_color(cam, &scattered, depth - 1, world));
	    return r;
        }
        return white;
    }

    vec3 unit_direction = vec3_unit_vector(r->direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    color c1 = {1.0, 1.0, 1.0};
    color c2 = {0.5, 0.7, 1.0};

    return vec3_add(vec3_scale(c1, 1.0 - a), vec3_scale(c2, a));
}

vec3 camera_sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    vec3 sam = {random_double() - 0.5, random_double() - 0.5, 0};
    return sam;
}

ray camera_get_ray(camera *cam, int i, int j) {
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location i, j.

    vec3 offset = camera_sample_square();
    vec3 pixel_sample = cam->pixel00_loc;
    pixel_sample = vec3_add(pixel_sample, vec3_scale(cam->pixel_delta_u, (i + offset.x)));
    pixel_sample = vec3_add(pixel_sample, vec3_scale(cam->pixel_delta_v, (j + offset.y)));

    point3 ray_origin = cam->center;
    vec3 ray_direction = vec3_subtract(pixel_sample, ray_origin);

    ray r = {ray_origin, ray_direction};
    return r;
}

void camera_render(camera *cam, hittable_list *world) {
    printf("P3\n%d %d \n255\n", cam->image_width, cam->image_height);

    for (int j = 0; j < cam->image_height; j++) {
        fprintf(stderr, "\rScanlines remaining: %d ", cam->image_height - j);
        fflush(stderr);
        for (int i = 0; i < cam->image_width; i++) {
            color pixel_color = {0, 0, 0};
            for (int sample = 0; sample < cam->samples_per_pixel; sample++) {
                ray r = camera_get_ray(cam, i, j);
                pixel_color = vec3_add(pixel_color, camera_ray_color(cam, &r, cam->max_depth, world));
            }
            write_color(vec3_scale(pixel_color, cam->pixel_samples_scale));
        }
    }
    fprintf(stderr, "\rDone.                 \n");
}

#endif
