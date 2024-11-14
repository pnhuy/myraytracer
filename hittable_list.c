#include "hittable_list.h"

hittable_list hittable_list_create() {
    hittable_list hl;
    hl.size = 0;
    return hl;
}

void hittable_list_add(hittable_list *hl, sphere *s) {
    if (hl->size >= MAX_HL) {
        return;
    }

    hl->objects[hl->size] = s;

    hl->size += 1;
}

void hittable_list_clear(hittable_list *hl) {
    hl->size = 0;
}

bool hittable_list_hit(hittable_list *hl, ray *r, interval ray_t, hit_record *rec) {
    hit_record *temp_rec;
    temp_rec = (hit_record *)malloc(sizeof(hit_record));

    if (temp_rec == NULL) {
        return false;
    }

    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    for (int i = 0; i < hl->size; i++) {
        sphere *s = hl->objects[i];
        if (s->hit(s, r, (interval){ray_t.min, closest_so_far}, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec->t;
            *rec = *temp_rec;
        }
    }
    free(temp_rec);
    return hit_anything;
}
