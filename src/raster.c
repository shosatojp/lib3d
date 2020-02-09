#include "lib3d.h"

void clear_raster_map(pixel_info* map, int w, int h) {
    memset(map, 0, w * h * sizeof(pixel_info));
}


pixel_info* create_raster_map(int w, int h) {
    return (pixel_info*)calloc(sizeof(pixel_info), w * h);
}

#define RASTER_MAP_AT(map, w, h, x, y) (map)[(x) + (y) * (w)]

void write_raster_map(pixel_info* __map, int w, int h, poligon* _poligon) {
    vtype min[2] = {0};
    vtype max[2] = {0};
    poligon_rect(POLIGON_VERTEX_COUNT, _poligon->vertices, min, max);

    for (int i = min[0]; i < max[0]; ++i) {
        for (int j = min[1]; j < max[1]; ++j) {
            vtype v[2] = {i, j};
            if (is_in_2d_poligon(POLIGON_VERTEX_COUNT, _poligon->vertices, v)) {
                pixel_info* p = &RASTER_MAP_AT(__map, w, h, i, j);
                p->_poligon = _poligon;
                p->activated = true;
                fragment_shader(p, v);
            }
        }
    }
}

void raster_map_to_buffer(pixel_info* __map, unsigned char* __buf, int w, int h) {
    for (int i = 0, len = w * h; i < len; i++) {
        if (__map[i].activated) {
            __buf[i * 3] = __map[i].color.r;
            __buf[i * 3 + 1] = __map[i].color.g;
            __buf[i * 3 + 2] = __map[i].color.b;
        }
    }
}

void set_max_z(poligon* _poligon) {
    _poligon->max_z = max(max(_poligon->vertices[0]->coordinate2d[2],
                              _poligon->vertices[1]->coordinate2d[2]),
                          _poligon->vertices[2]->coordinate2d[2]);
}

int compare_poligons(const void* p, const void* q) {
    return -((poligon*)p)->max_z + ((poligon*)q)->max_z;
}

void sort_by_z(int c, poligon* _poligons[]) {
    qsort(_poligons, c, sizeof(poligon*), compare_poligons);
}