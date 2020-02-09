#include "lib3d.h"

void l3ClearRasterMap(pixel_info* map, int w, int h) {
    memset(map, 0, w * h * sizeof(pixel_info));
}

pixel_info* l3CreateRasterMap(int w, int h) {
    return (pixel_info*)calloc(sizeof(pixel_info), w * h);
}

void l3WriteRasterMap(pixel_info* map, int w, int h, poligon* _poligon) {
    vtype min[2] = {0};
    vtype max[2] = {0};
    l3GetPoligonOuterRect(POLIGON_VERTEX_COUNT, _poligon->vertices, min, max);

    for (int i = min[0]; i < max[0]; ++i) {
        for (int j = min[1]; j < max[1]; ++j) {
            vtype v[2] = {i, j};
            if (l3InsideOfPoligon2D(POLIGON_VERTEX_COUNT, _poligon->vertices, v)) {
                pixel_info* p = &l3RasterMapAt(map, w, h, i, j);
                p->_poligon = _poligon;
                p->activated = true;
                l3FragmentShader(p, v);
            }
        }
    }
}

void l3ConvertRasterMapToBuffer(pixel_info* map, unsigned char* buf, int w, int h) {
    for (int i = 0, len = w * h; i < len; i++) {
        if (map[i].activated) {
            buf[i * 3] = map[i].color.r;
            buf[i * 3 + 1] = map[i].color.g;
            buf[i * 3 + 2] = map[i].color.b;
        }
    }
}

void l3SetMaxZofPoligon(poligon* _poligon) {
    _poligon->max_z = max(max(_poligon->vertices[0]->coordinate2d[2],
                              _poligon->vertices[1]->coordinate2d[2]),
                          _poligon->vertices[2]->coordinate2d[2]);
}

int l3ComparePoligons(const void* p, const void* q) {
    return -((poligon*)p)->max_z + ((poligon*)q)->max_z;
}

void l3SortPoligonsByZ(int c, poligon* _poligons[]) {
    qsort(_poligons, c, sizeof(poligon*), l3ComparePoligons);
}