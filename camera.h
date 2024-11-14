#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "rt.h"
#include "vec3.h"
#include <math.h>
#include <stdio.h>

typedef struct camera {
    double aspect_ratio;
    int image_width;
    int samples_per_pixel;
    double pixel_samples_scale;
    int max_depth;
    double vfov;

    point3 lookfrom; // Point camera is looking from
    point3 lookat;   // Point camera is looking at
    vec3   vup;      // Camera-relative "up" direction

    double defocus_angle; // Variation angle of rays through each pixel
    double focus_dist;    // Distance from camera lookfrom point to plane of perfect focus
    
    int image_height;    // Rendered image height
    point3 center;       // Camera center
    point3 pixel00_loc;  // Location of pixel 0, 0
    vec3 pixel_delta_u;  // Offset to pixel to the right
    vec3 pixel_delta_v;  // Offset to pixel below
    vec3 u, v, w;        // Camera frame basis vectors
    vec3 defocus_disk_u; // Defocus disk horizontal radius
    vec3 defocus_disk_v; // Defocus disk vertical radius
} camera;

camera camera_create(double aspect_ratio, int image_width, int samples_per_pixel, int max_depth, double vfov, point3 lookfrom, point3 lookat, vec3 vup, double defocus_angle, double focus_dist);

color camera_ray_color(camera *cam, ray *r, int depth, hittable_list *world);

vec3 camera_sample_square();

point3 defocus_disk_sample(camera *cam);

ray camera_get_ray(camera *cam, int i, int j);

void camera_render(camera *cam, hittable_list *world);

#endif
