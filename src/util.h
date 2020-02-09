#pragma once
#include "common.h"
#define max(a, b) (((a) > (b)) ? (a) : (b))

void poligon_rect(int c, vertex* _vertices[], mat21 min, mat21 max);

vtype triangle_area(mat21 p, mat21 a, mat21 b);

vertex* create_vertex(vtype x, vtype y, vtype z, rgb* color);

poligon* create_poligon(vertex* v1, vertex* v2, vertex* v3);