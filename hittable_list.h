#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "ray.h"
#include "sphere.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_HL 9999

typedef struct hittable_list {
    int size;
    sphere *objects[MAX_HL];
} hittable_list;

hittable_list hittable_list_create();

void hittable_list_add(hittable_list *hl, sphere *s);

void hittable_list_clear(hittable_list *hl);

bool hittable_list_hit(hittable_list *hl, ray *r, interval ray_t, hit_record *rec);

#endif
