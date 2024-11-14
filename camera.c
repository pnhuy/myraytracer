#include "camera.h"

camera camera_create(double aspect_ratio, int image_width, int samples_per_pixel, int max_depth, double vfov, point3 lookfrom, point3 lookat, vec3 vup, double defocus_angle, double focus_dist) {
    camera c;
    c.aspect_ratio = aspect_ratio;
    c.image_width = image_width;
    c.image_height = (int)(c.image_width / c.aspect_ratio);
    c.image_height = (c.image_height < 1) ? 1 : c.image_height;
    c.vfov = vfov;
    c.lookfrom = lookfrom;
    c.lookat = lookat;
    c.vup = vup;
    c.defocus_angle = defocus_angle;
    c.focus_dist = focus_dist;

    c.samples_per_pixel = samples_per_pixel;
    c.pixel_samples_scale = 1.0 / c.samples_per_pixel;

    c.max_depth = max_depth;

    c.center = c.lookfrom;

    // Determine viewport dimensions
    /* double focal_length = vec3_length(vec3_subtract(c.lookfrom, c.lookat)); */
    double theta = degrees_to_radians(vfov);
    double h = tan(theta/2);
    double viewport_height = 2 * h * c.focus_dist;
    double viewport_width = viewport_height * ((double)(c.image_width) / c.image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    vec3 w = vec3_unit_vector(vec3_subtract(c.lookfrom, c.lookat));
    vec3 u = vec3_unit_vector(vec3_cross(c.vup, w));
    vec3 v = vec3_cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = vec3_scale(u, viewport_width);
    vec3 viewport_v = vec3_scale(vec3_negate(v), viewport_height);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    c.pixel_delta_u = vec3_div(viewport_u, c.image_width);
    c.pixel_delta_v = vec3_div(viewport_v, c.image_height);

    // Calculate the location of the upper left pixel.
    vec3 viewport_upper_left = c.center;
    vec3 focal_vec3 = vec3_scale(w, c.focus_dist);
    viewport_upper_left = vec3_subtract(viewport_upper_left, focal_vec3);
    viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_div(viewport_u, 2.0));
    viewport_upper_left = vec3_subtract(viewport_upper_left, vec3_div(viewport_v, 2.0));
    c.pixel00_loc = vec3_add(viewport_upper_left,
                             vec3_scale(vec3_add(c.pixel_delta_u, c.pixel_delta_v), 0.5));

    double defocus_radius = c.focus_dist * tan(degrees_to_radians(c.defocus_angle / 2));
    c.defocus_disk_u = vec3_scale(u, defocus_radius);
    c.defocus_disk_v = vec3_scale(v, defocus_radius);

    return c;
}

color camera_ray_color(camera *cam, ray *r, int depth, hittable_list *world) {
    if (depth <= 0) {
        return (color){0, 0, 0};
    }

    hit_record rec;
    interval i = {0.001, INFINITY};

    if (hittable_list_hit(world, r, i, &rec)) {
        // fprintf(stderr, "Hit at Depth %d\n", depth);
        ray scattered;
        color attenuation;

        if ((*(rec.scatter))(r, &rec, &attenuation, &scattered)) {
            color c = camera_ray_color(cam, &scattered, depth - 1, world);
            vec3 rc = vec3_hadamard_prod(attenuation, c);
            return rc;
        }
        return (color){0, 0, 0};
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

point3 defocus_disk_sample(camera *cam) {
    // Returns a random point in the camera defocus disk.
    point3 p = random_in_unit_disk();
    point3 x = (cam->center);
    x = vec3_add(x, (vec3_scale(cam->defocus_disk_u, p.x)));
    x = vec3_add(x, (vec3_scale(cam->defocus_disk_v, p.y)));
    return x;
}

ray camera_get_ray(camera *cam, int i, int j) {
    // Construct a camera ray originating from the defocus disk and directed at randomly sampled
    // point around the pixel location i, j.

    vec3 offset = camera_sample_square();
    vec3 pixel_sample = cam->pixel00_loc;
    pixel_sample = vec3_add(pixel_sample, vec3_scale(cam->pixel_delta_u, (i + offset.x)));
    pixel_sample = vec3_add(pixel_sample, vec3_scale(cam->pixel_delta_v, (j + offset.y)));

    point3 ray_origin = (cam->defocus_angle <= 0) ?  cam->center : defocus_disk_sample(cam);
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
            color c = vec3_scale(pixel_color, cam->pixel_samples_scale);
            write_color(c);
        }
    }
    fprintf(stderr, "\rDone.                 \n");
}
