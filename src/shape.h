#include "common.h"

bool l3IntersectRayColumn(l3Mat41 ray_origin, l3Mat41 ray_direction,
                          l3Mat31 v0, l3Mat31 v1, l3Type radius, l3Mat41 r);

bool l3IntersectRayCircle(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Mat41 center, l3Mat41 normal, l3Type radius, l3Mat41 r);