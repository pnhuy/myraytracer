#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "rt.h"
#include "sphere.h"
#include "vec3.h"
#include <math.h>

int three_sphere() {
    // World
    hittable_list world = hittable_list_create();

    color color_ground = {0.8, 0.8, 0.0};
    material material_ground = {lambertian, color_ground};

    color color_center = {0.1, 0.2, 0.5};
    material material_center = {lambertian, color_center};

    color color_left = {0.8, 0.8, 0.8};
    material material_left = {dielectric, color_left, 1.50, 1.50};
    material material_bubble = {dielectric, color_left, 1.50, 1.00 / 1.50};

    color color_right = {0.8, 0.6, 0.2};
    material material_right = {metal, color_right, 0.8};

    point3 point_ground = {0, -100.5, -1.0};
    sphere sphere_ground = sphere_init(point_ground, 100, &material_ground);

    point3 point_center = {0, 0, -1.2};
    sphere sphere_center = sphere_init(point_center, 0.5, &material_center);

    point3 point_left = {-1.0, 0, -1.0};
    sphere sphere_left = sphere_init(point_left, 0.5, &material_left);
    sphere sphere_bubble = sphere_init(point_left, 0.4, &material_bubble);

    point3 point_right = {1.0, 0, -1.0};
    sphere sphere_right = sphere_init(point_right, 0.5, &material_right);

    hittable_list_add(&world, &sphere_ground);
    hittable_list_add(&world, &sphere_center);
    hittable_list_add(&world, &sphere_left);
    hittable_list_add(&world, &sphere_bubble);
    hittable_list_add(&world, &sphere_right);

    // Camera
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_depth = 50;
    double vfov = 20;
    point3 lookfrom = {-2, 2, 1};
    point3 lookat = {0, 0, -1};
    vec3 vup = {0, 1, 0};
    double defocus_angle = 10.0;
    double focus_dist = 3.4;

    camera cam = camera_create(aspect_ratio, image_width, samples_per_pixel, max_depth, vfov, lookfrom, lookat, vup, defocus_angle, focus_dist);
    camera_render(&cam, &world);

    return 0;
}

int wide_angle_camera() {
    double R = cos(pi / 4);

    hittable_list world = hittable_list_create();

    material material_left = {lambertian, (color){0, 0, 1}};
    material material_right = {lambertian, (color){1, 0, 0}};

    sphere sphere_left = sphere_init((point3){-R, 0, -1}, R, &material_left);
    sphere sphere_right = sphere_init((point3){R, 0, -1}, R, &material_right);

    hittable_list_add(&world, &sphere_left);
    hittable_list_add(&world, &sphere_right);

    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_depth = 50;
    double vfov = 90;
    point3 lookfrom = (point3){0, 0, 0};
    point3 lookat = (point3){0, 0, -1};
    vec3 vup = (vec3){0, 1, 0};
    double defocus_angle = 10.0;
    double focus_dist = 3.4;

    camera cam = camera_create(aspect_ratio, image_width, samples_per_pixel, max_depth, vfov, lookfrom, lookat, vup, defocus_angle, focus_dist);
    camera_render(&cam, &world);

    return 0;
}

int random_bubbles() {
    hittable_list world = hittable_list_create();

    material ground_material = (material){lambertian, (color){0.5, 0.5, 0.5}};
    sphere ground_sphere = sphere_init((point3){0, -1000, 0}, 1000, &ground_material);
    hittable_list_add(&world, &ground_sphere);

    int N = 500;
    int limit = 11;

    /* color albedo[N]; */
    material sphere_material[N];
    sphere spheres[N];

    int n = 0;
    for (int a = -limit; a < limit; a++) {
        for (int b = -limit; b < limit; b++) {
            double choose_mat = random_double();
            point3 center = (point3){a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()};

            if (vec3_length(vec3_add(center, (point3){0.4, 0.2, 0})) > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    color albedo = vec3_hadamard_prod(vec3_random(), vec3_random());
                    sphere_material[n] = (material){lambertian, albedo};
                    spheres[n] = sphere_init(center, 0.2, &sphere_material[n]);
                    hittable_list_add(&world, &spheres[n]);
                } else if (choose_mat < 0.95) {
                    // metal
                    color albedo = vec3_random_range(0.5, 1.0);
                    double fuzz = random_double_range(0, 0.5);
                    sphere_material[n] = (material){metal, albedo, fuzz};
                    spheres[n] = sphere_init(center, 0.2, &sphere_material[n]);
                    hittable_list_add(&world, &spheres[n]);
                } else {
                    // glass
                    sphere_material[n] = (material){dielectric, vec3_random(), random_double(), 1.5};
                    spheres[n] = sphere_init(center, 0.2, &sphere_material[n]);
                    hittable_list_add(&world, &spheres[n]);
                }
                n++;
            }
        }
    }

    material material1 = {dielectric, vec3_random(), random_double(), 1.5};
    sphere sphere1 = sphere_init((point3){0, 1, 0}, 1.0, &material1);

    material material2 = {lambertian, (color){0.4, 0.2, 0.1}};
    sphere sphere2 = sphere_init((point3){-4, 1, 0}, 1.0, &material2);

    material material3 = {metal, (color){0.7, 0.6, 0.5}, 0.0};
    sphere sphere3 = sphere_init((point3){4, 1, 0}, 1.0, &material3);

    hittable_list_add(&world, &sphere1);
    hittable_list_add(&world, &sphere2);
    hittable_list_add(&world, &sphere3);

    // Camera
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 1200;
    int samples_per_pixel = 500;
    int max_depth = 50;

    double vfov = 20;
    point3 lookfrom = {13, 2, 3};
    point3 lookat = {0, 0, 0};
    vec3 vup = {0, 1, 0};

    double defocus_angle = 0.6;
    double focus_dist = 10.0;

    camera cam = camera_create(aspect_ratio, image_width, samples_per_pixel, max_depth, vfov, lookfrom, lookat, vup, defocus_angle, focus_dist);
    camera_render(&cam, &world);

    return 0;
}

int main() {
    return random_bubbles();
}
