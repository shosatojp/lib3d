#pragma once

#include "common.h"

#define SET_POINT_RASTER_MAP(map, w, h, x, y, _point) \
    if (0 <= x && x < w && 0 <= y && y < h) {         \
        map[x + y * w] = _point;                      \
    }
void l3ClearRasterMap(pixel_info* map, int w, int h);
pixel_info* l3CreateRasterMap(int w, int h);
#define l3RasterMapAt(map, w, h, x, y) (map)[(x) + (y) * (w)]
void l3WriteRasterMap(pixel_info* map, int w, int h, poligon* _poligon);
void l3ConvertRasterMapToBuffer(pixel_info* map, unsigned char* buf, int w, int h);
void l3SetMaxZofPoligon(poligon* _poligon);
int l3ComparePoligons(const void* p, const void* q);
void l3SortPoligonsByZ(int c, poligon* _poligons[]);