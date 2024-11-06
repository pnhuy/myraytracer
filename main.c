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

    point3 c1 = {0, 0, -1};
    sphere s1 = sphere_init(c1, 0.5);

    point3 c2 = {0, -100.5, -1};
    sphere s2 = sphere_init(c2, 100);

    hittable_list_add(&world, &s2);
    hittable_list_add(&world, &s1);

    // Camera
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;

    camera cam = camera_create(aspect_ratio, image_width, samples_per_pixel);
    camera_render(&cam, &world);

    return 0;
}
