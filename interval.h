#ifndef INTERVAL_H
#define INTERVAL_H

#include <math.h>
#include <stdbool.h>

typedef struct interval {
    double min;
    double max;
} interval;

interval interval_create();
double interval_size(const interval *i);
bool interval_contains(const interval *i, double x);
bool interval_surrounds(const interval *i, double x);
double interval_clamp(interval *i, double x);

static const interval empty = {INFINITY, -INFINITY};
static const interval universe = {-INFINITY, INFINITY};

#endif
