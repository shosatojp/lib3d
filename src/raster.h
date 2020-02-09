#pragma once

#include "common.h"

#define l3RasterMapAt(map, w, h, x, y) (map)[(x) + (y) * (w)]

void l3ClearRasterMap(l3PixelInfo* map, int w, int h);
l3PixelInfo* l3CreateRasterMap(int w, int h);
void l3WriteRasterMap(l3PixelInfo* map, int w, int h, l3Poligon* _poligon);
void l3ConvertRasterMapToBuffer(l3PixelInfo* map, unsigned char* buf, int w, int h);
void l3SetMaxZofPoligon(l3Poligon* _poligon);
int l3ComparePoligons(const void* p, const void* q);
void l3SortPoligonsByZ(int c, l3Poligon* _poligons[]);