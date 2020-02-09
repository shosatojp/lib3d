#pragma once
#include "common.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))

void l3SetOuterRectPoligon(l3Poligon* poligon);
l3Type l3GetTriangleArea(l3Mat21 p, l3Mat21 a, l3Mat21 b);
l3Vertex* l3CreateVertex(l3Type x, l3Type y, l3Type z, l3RGB* color);
l3Poligon* l3CreatePoligon(l3Vertex* v1, l3Vertex* v2, l3Vertex* v3);
void l3DestructVertices(int vc, l3Vertex* vs[]);
void l3DestructPoligons(int pc, l3Poligon* ps[]);
void l3InitializeObject(l3Object* o);
void l3SetTransposeObject(l3Object* o, l3Type dx, l3Type dy, l3Type dz);
void l3SetScaleObject(l3Object* o, l3Type sx, l3Type sy, l3Type sz);
void l3SetPoligonsObject(l3Object* o, int count, l3Poligon* ps[]);