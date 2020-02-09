#include "lib3d.h"

void l3GetPoligonOuterRect(int c, vertex* _vertices[], mat21 min, mat21 max) {
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

vtype l3GetTriangleArea(mat21 p, mat21 a, mat21 b) {
    return fabs((a[0] - p[0]) * (b[1] - p[1]) - (a[1] - p[1]) * (b[0] - p[0])) / 2;
}

vertex* l3CreateVertex(vtype x, vtype y, vtype z, rgb* color) {
    vertex* _v = (vertex*)calloc(sizeof(vertex), 1);
    _v->coordinate[0] = x;
    _v->coordinate[1] = y;
    _v->coordinate[2] = z;
    _v->coordinate[3] = 1;
    _v->color.r = color->r;
    _v->color.g = color->g;
    _v->color.b = color->b;
    return _v;
}

void l3DestructVertices(int vc, vertex* vs[]) {
    while (vc)
        free((void*)vs[--vc]);
}

poligon* l3CreatePoligon(vertex* v1, vertex* v2, vertex* v3) {
    poligon* _p = (poligon*)calloc(sizeof(poligon), 1);
    _p->vertices[0] = v1;
    _p->vertices[1] = v2;
    _p->vertices[2] = v3;
    return _p;
}

void l3DestructPoligons(int pc, poligon* ps[]) {
    while (pc)
        free((void*)ps[--pc]);
}