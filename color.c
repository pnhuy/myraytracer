#include "color.h"
#include "interval.h"
#include <stdio.h>

double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return sqrt(linear_component);
    return 0;
}

void write_color(color pixel_color) {
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;

    r = linear_to_gamma(r);
    b = linear_to_gamma(b);
    g = linear_to_gamma(g);

    static interval intensity = {0.000, 0.999};
    int rbyte = (int)(256 * interval_clamp(&intensity, r));
    int gbyte = (int)(256 * interval_clamp(&intensity, g));
    int bbyte = (int)(256 * interval_clamp(&intensity, b));

    printf("%d %d %d\n", rbyte, gbyte, bbyte);
}
