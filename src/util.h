#pragma once
#include "common.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))

void l3GetPoligonOuterRect(int c, l3Vertex* _vertices[], l3Mat21 min, l3Mat21 max);
l3Type l3GetTriangleArea(l3Mat21 p, l3Mat21 a, l3Mat21 b);
l3Vertex* l3CreateVertex(l3Type x, l3Type y, l3Type z, l3RGB* color);
l3Poligon* l3CreatePoligon(l3Vertex* v1, l3Vertex* v2, l3Vertex* v3);
void l3DestructVertices(int vc, l3Vertex* vs[]);
void l3DestructPoligons(int pc, l3Poligon* ps[]);
