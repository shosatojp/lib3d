#include "lib3d.h"

int main() {
    int w = 1920, h = 1080;
    rgb red = {255, 0, 0};
    rgb green = {0, 255, 0};
    rgb blue = {0, 0, 255};
    rgb white = {255, 255, 255};
    {
        object _object;
        memset(&_object, 0, sizeof(object));
        vertex* vs[] = {
            l3CreateVertex(5, 5, 5, &red),
            l3CreateVertex(-5, -5, 5, &green),
            l3CreateVertex(5, -5, -5, &blue),
            l3CreateVertex(-5, 5, -5, &white),
        };

        _object.dx = -15;
        _object.dy = -15;
        _object.dz = -15;
        _object.sx = 1;
        _object.sy = 1;
        _object.sz = 1;
        poligon* poligons[] = {
            l3CreatePoligon(vs[0], vs[2], vs[1]),
            l3CreatePoligon(vs[0], vs[3], vs[2]),
            l3CreatePoligon(vs[1], vs[2], vs[3]),
            l3CreatePoligon(vs[0], vs[1], vs[3]),
        };
        _object.poligons = poligons;
        _object.poligon_count = 4;

        vtype camera[4] = {14, 0, -10, 1};
        vtype target[4] = {20, 20, 20, 1};
        vtype upper[4] = {0, 1, 0, 1};
        vtype wc[16] = {0};
        vtype cp[16] = {0};
        vtype ps[16] = {0};
        vtype wcp[16] = {0};
        l3MakeWorldToCameraMat44(camera, target, upper, wc);
        l3MakeCameraToProjectionMat44(120, (double)w / h, 10, 100, cp);
        l3MakeProjectionToScreenMat44(w, h, ps);
        l3MulMat4444(cp, wc, wcp);

        pixel_info* map = l3CreateRasterMap(w, h);
        unsigned char* buf = l3MakeBuffer(w, h, 255);
        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 4; j++) {
                vs[j]->converted = false;
            }
            l3ClearRasterMap(map, w, h);
            l3ClearBuffer(buf, w, h);
            _object.theta_y = i * 5 * 3.14 / 180.0;
            l3ConvertObject(&_object, wcp, ps, map, w, h);

            l3ConvertRasterMapToBuffer(map, buf, w, h);
            char name[20] = {0};
            sprintf(name, "bin/hoge-%03d.ppm", i);
            l3WriteBuffer(buf, w, h, name);
        }

        free(buf);
        free(map);
        l3DestructVertices(sizeof(vs) / sizeof(vertex*), vs);
        l3DestructPoligons(sizeof(poligons) / sizeof(poligon*), poligons);
    }
}