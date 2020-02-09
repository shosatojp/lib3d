#pragma once

#include "common.h"

#define SET_POINT_RASTER_MAP(__map, w, h, x, y, _point) \
    if (0 <= x && x < w && 0 <= y && y < h) {           \
        __map[x + y * w] = _point;                      \
    }
void clear_raster_map(pixel_info* map, int w, int h);

pixel_info* create_raster_map(int w, int h);

#define RASTER_MAP_AT(map, w, h, x, y) (map)[(x) + (y) * (w)]

void write_raster_map(pixel_info* __map, int w, int h, poligon* _poligon);

void raster_map_to_buffer(pixel_info* __map, unsigned char* __buf, int w, int h);

void set_max_z(poligon* _poligon);
int compare_poligons(const void* p, const void* q);
void sort_by_z(int c, poligon* _poligons[]);