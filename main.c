#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vec3.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
    // World
    hittable_list world = hittable_list_create();

    color color_ground = {0.8, 0.8, 0.0};
    material material_ground = {lambertian, color_ground};

    color color_center = {0.1, 0.2, 0.5};
    material material_center = {lambertian, color_center};

    color color_left = {0.8, 0.8, 0.8};
    material material_left = {metal, color_left};

    color color_right = {0.8, 0.6, 0.2};
    material material_right = {metal, color_right};    

    point3 point_ground = {0, -100.5, -1.0};
    sphere sphere_ground = sphere_init(point_ground, 100, &material_ground);

    point3 point_center = {0, 0, -1.2};
    sphere sphere_center = sphere_init(point_center, 0.5, &material_center);

    point3 point_left = {-1.0, 0, -1.0};
    sphere sphere_left = sphere_init(point_left, 0.5, &material_left);

    point3 point_right = {1.0, 0, -1.0};
    sphere sphere_right = sphere_init(point_right, 0.5, &material_right);


    hittable_list_add(&world, &sphere_ground);
    hittable_list_add(&world, &sphere_right);
    hittable_list_add(&world, &sphere_center);
    hittable_list_add(&world, &sphere_left);

    // Camera
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_depth = 50;

    camera cam = camera_create(aspect_ratio, image_width, samples_per_pixel, max_depth);
    camera_render(&cam, &world);

    return 0;
}
