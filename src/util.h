#pragma once
#include "common.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))

void l3GetPoligonOuterRect(int c, vertex* _vertices[], mat21 min, mat21 max);
vtype l3GetTriangleArea(mat21 p, mat21 a, mat21 b);
vertex* l3CreateVertex(vtype x, vtype y, vtype z, rgb* color);
poligon* l3CreatePoligon(vertex* v1, vertex* v2, vertex* v3);
void l3DestructVertices(int vc, vertex* vs[]);
void l3DestructPoligons(int pc, poligon* ps[]);
