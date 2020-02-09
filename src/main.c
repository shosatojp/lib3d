#include "lib3d.h"

int main() {
    int w = 400, h = 200;
    rgb red = {255, 0, 0};
    rgb green = {0, 255, 0};
    rgb blue = {0, 0, 255};
    rgb black = {0, 0, 0};
    {
        object _object;
        memset(&_object, 0, sizeof(object));
        vertex* vs[] = {
            create_vertex(5, 5, 5, &red),
            create_vertex(-5, -5, 5, &green),
            create_vertex(5, -5, -5, &blue),
            create_vertex(-5, 5, -5, &black),
        };

        _object.dx = -15;
        _object.dy = -15;
        _object.dz = -15;
        _object.sx = 1;
        _object.sy = 1;
        _object.sz = 1;
        poligon* poligons[] = {
            create_poligon(vs[0], vs[2], vs[1]),
            create_poligon(vs[0], vs[3], vs[2]),
            create_poligon(vs[1], vs[2], vs[3]),
            create_poligon(vs[0], vs[1], vs[3]),
        };
        _object.poligons = poligons;
        _object.poligon_count = 4;

        vtype camera[4] = {14, 0, -10, 1};
        vtype target[4] = {20, 20, 20, 1};
        vtype upper[4] = {0, 1, 0, 1};
        vtype wc[16] = {0};
        vtype cp[16] = {0};
        vtype ps[16] = {0};
        make_world_to_camera_mat44(camera, target, upper, wc);
        make_camera_to_projection_mat44(120, (double)w / h, 10, 100, cp);
        make_projection_to_screen_mat44(w, h, ps);
        vtype wcp[16] = {0};
        mul4444(cp, wc, wcp);

        pixel_info* map = create_raster_map(w, h);
        unsigned char* buf = make_buffer(w, h, 255);
        for (int i = 0; i < 300; i++) {
            for (int j = 0; j < 4; j++) {
                vs[j]->converted = false;
            }
            clear_raster_map(map, w, h);
            clear_buffer(buf, w, h);
            _object.theta_y = i * 5 * 3.14 / 180.0;
            convert_object(&_object, wcp, ps, map, w, h);

            raster_map_to_buffer(map, buf, w, h);
            char name[16] = {0};
            sprintf(name, "hoge-%03d.ppm", i);
            write_buffer(buf, w, h, name);
        }
    }
}