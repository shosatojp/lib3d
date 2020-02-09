#include "lib3d.h"

void l3GetPoligonOuterRect(int c, l3Vertex* _vertices[], l3Mat21 min, l3Mat21 max) {
    int maxx = _vertices[0]->coordinate2d[0],
        maxy = _vertices[0]->coordinate2d[1],
        minx = _vertices[0]->coordinate2d[0],
        miny = _vertices[0]->coordinate2d[1];

    for (int i = 1; i < c; i++) {
        if (_vertices[i]->coordinate2d[0] > maxx) maxx = _vertices[i]->coordinate2d[0];
        if (_vertices[i]->coordinate2d[1] > maxy) maxy = _vertices[i]->coordinate2d[1];
        if (_vertices[i]->coordinate2d[0] < minx) minx = _vertices[i]->coordinate2d[0];
        if (_vertices[i]->coordinate2d[1] < miny) miny = _vertices[i]->coordinate2d[1];
    }

    max[0] = maxx;
    max[1] = maxy;
    min[0] = minx;
    min[1] = miny;
}

l3Type l3GetTriangleArea(l3Mat21 p, l3Mat21 a, l3Mat21 b) {
    return fabs((a[0] - p[0]) * (b[1] - p[1]) - (a[1] - p[1]) * (b[0] - p[0])) / 2;
}

l3Vertex* l3CreateVertex(l3Type x, l3Type y, l3Type z, l3RGB* color) {
    l3Vertex* _v = (l3Vertex*)calloc(sizeof(l3Vertex), 1);
    _v->coordinate[0] = x;
    _v->coordinate[1] = y;
    _v->coordinate[2] = z;
    _v->coordinate[3] = 1;
    _v->color.r = color->r;
    _v->color.g = color->g;
    _v->color.b = color->b;
    return _v;
}

void l3DestructVertices(int vc, l3Vertex* vs[]) {
    while (vc)
        free((void*)vs[--vc]);
}

l3Poligon* l3CreatePoligon(l3Vertex* v1, l3Vertex* v2, l3Vertex* v3) {
    l3Poligon* _p = (l3Poligon*)calloc(sizeof(l3Poligon), 1);
    _p->vertices[0] = v1;
    _p->vertices[1] = v2;
    _p->vertices[2] = v3;
    return _p;
}

void l3DestructPoligons(int pc, l3Poligon* ps[]) {
    while (pc)
        free((void*)ps[--pc]);
}