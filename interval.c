#include "interval.h"

interval interval_create() {
    interval x;
    x.min = INFINITY;
    x.max = -INFINITY;
    return x;
}

double interval_size(const interval *i) {
    return i->max - i->min;
}

bool interval_contains(const interval *i, double x) {
    return i->min <= x && x <= i->max;
}

bool interval_surrounds(const interval *i, double x) {
    return i->min < x && x < i->max;
}

double interval_clamp(interval *i, double x) {
    if (x < i->min) return i->min;
    if (x > i->max) return i->max;
    return x;
}