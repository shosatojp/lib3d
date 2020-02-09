#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "lib3d.h"
int white[3] = {0, 0, 0};

void init_mat(mat a, int x, int y) {
    memset(a, 0, sizeof(vtype) * x * y);
}

/**
 * a    4*4
 * b    4*1
 * r    4*1
 * 
 * a != r, b != r
 * must initialize r : true
 */
void mul44(mat44 a, mat41 b, mat41 r) {
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
            r[i] += a[i + 4 * k] * b[k];
            // printf("%f  =  %f  *  %f\n", r[i], a[i + 4 * k], b[k]);
        }
    }
}

/**
 * a    4*4
 * b    4*4
 * r    4*4
 * 
 * a != r, b != r
 * must initialize r : true
 * 
 */
void mul4444(mat44 a, mat44 b, mat44 r) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                r[i + 4 * j] += a[i + k * 4] * b[k + 4 * j];
            }
        }
    }
}

/**
 * a = r, b = r
 * must initialize r : false
 */
void vector_sub(mat a, mat b, mat r, int n) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] - b[i];
    }
}

/**
 * a = r
 * must initialize r : false
 */
void vector_div(mat a, vtype d, mat r, int n) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] / d;
    }
}

void mat_print(mat a, int x, int y) {
    for (int i = 0; i < x; ++i) {
        if (i == 0)
            printf("┌ ");
        else if (i == x - 1)
            printf("└ ");
        else
            printf("│ ");
        for (int j = 0; j < y; ++j) {
            printf("%5.2f ", a[j * 4 + i]);
        }
        if (i == 0)
            printf("┐");
        else if (i == x - 1)
            printf("┘");
        else
            printf("│");
        printf("\n");
    }
}

/**
 * a = r
 */
void normarize_vector(mat a, mat r, int n) {
    vtype sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * a[i];
    }
    vtype s = sqrt(sum);
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] / s;
    }
}

/**
 * a = r, b = r
 */
void vector_product3(mat41 a, mat41 b, mat41 r) {
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
    r[3] = 1;
}

vtype vector_product2(mat21 a, mat21 b) {
    return a[0] * b[1] - a[1] * b[0];
}

vtype inner_product_1d(mat a, mat b, int n) {
    vtype sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

/**
 * camera = target = upper = r
 */
void make_world_to_camera_mat44(mat41 camera, mat41 target, mat41 upper, mat44 r) {
    vtype tmp[4] = {0};
    vtype cx[4] = {0};
    vtype cy[4] = {0};
    vtype cz[4] = {0};
    // cz
    vector_sub(target, camera, tmp, 3);
    normarize_vector(tmp, cz, 3);
    // cx
    vector_product3(cz, upper, tmp);
    normarize_vector(tmp, cx, 3);
    // cy
    vector_product3(cx, cz, tmp);
    normarize_vector(tmp, cy, 3);

    r[0] = cx[0];
    r[4] = cx[1];
    r[8] = cx[2];
    r[1] = cy[0];
    r[5] = cy[1];
    r[9] = cy[2];
    r[2] = cz[0];
    r[6] = cz[1];
    r[10] = cz[2];
    r[12] = -inner_product_1d(camera, cx, 3);
    r[13] = -inner_product_1d(camera, cy, 3);
    r[14] = -inner_product_1d(camera, cz, 3);
    r[15] = 1;
}

void make_camera_to_projection_mat44(vtype angle, vtype aspect, vtype near, vtype far, mat44 r) {
    vtype tan_angle_2 = tan(angle / 2);
    r[5] = 1 / tan_angle_2;
    r[0] = r[5] / aspect;
    r[10] = far / (far - near);
    r[11] = 1;
    r[14] = -near * r[10];
}

void make_projection_to_screen_mat44(vtype width, vtype height, mat44 r) {
    vtype width_2 = width / 2.0;
    vtype height_2 = height / 2.0;
    r[0] = width_2;
    r[5] = -height_2;
    r[10] = 1;
    r[12] = width_2;
    r[13] = height_2;
    r[15] = 1;
}

void make_matrix_round_x(vtype theta, mat44 r) {
    r[0] = 1;
    vtype cos_theta = cos(theta),
          sin_theta = sin(theta);
    r[5] = cos_theta;
    r[6] = sin_theta;
    r[9] = -sin_theta;
    r[10] = cos_theta;
    r[15] = 1;
}
void make_matrix_round_y(vtype theta, mat44 r) {
    vtype cos_theta = cos(theta),
          sin_theta = sin(theta);
    r[0] = cos_theta;
    r[2] = -sin_theta;
    r[5] = 1;
    r[8] = sin_theta;
    r[10] = cos_theta;
    r[15] = 1;
}
void make_matrix_round_z(vtype theta, mat44 r) {
    vtype cos_theta = cos(theta),
          sin_theta = sin(theta);
    r[0] = cos_theta;
    r[1] = sin_theta;
    r[4] = -sin_theta;
    r[5] = cos_theta;
    r[10] = 1;
    r[15] = 1;
}
void make_matrix_transpose(vtype dx, vtype dy, vtype dz, mat44 r) {
    r[0] = 1;
    r[5] = 1;
    r[10] = 1;
    r[12] = dx;
    r[13] = dy;
    r[14] = dz;
    r[15] = 1;
}
void make_matrix_scale(vtype sx, vtype sy, vtype sz, mat44 r) {
    r[0] = sx;
    r[5] = sy;
    r[10] = sz;
    r[15] = 1;
}

void mul44s(int c, mat44* as, mat44 r) {
    if (c <= 1) {
        fprintf(stderr, "invalid matrix count\n");
    }
    switch (c) {
        case 2:
            mul4444(as[1], as[0], r);
        default: {
            vtype tmp0[16] = {0};
            vtype tmp1[16] = {0};
            mul4444(as[c - 1], as[c - 2], tmp0);
            int tmp_num = 0;
            for (int i = c - 3; i > 0; i--) {
                if (tmp_num == 0) {
                    init_mat(tmp1, 4, 4);
                    mul4444(tmp0, as[i], tmp1);
                    tmp_num = 1;
                } else {
                    init_mat(tmp0, 4, 4);
                    mul4444(tmp1, as[i], tmp0);
                    tmp_num = 0;
                }
            }
            if (tmp_num == 0) {
                mul4444(tmp0, as[0], r);
            } else {
                mul4444(tmp1, as[0], r);
            }
        }
    }
}

void make_local_to_world_mat44(vtype dx, vtype dy, vtype dz,
                               vtype sx, vtype sy, vtype sz,
                               vtype theta_x, vtype theta_y, vtype theta_z, mat44 r) {
    vtype s[16] = {0};
    vtype d[16] = {0};
    vtype rx[16] = {0};
    vtype ry[16] = {0};
    vtype rz[16] = {0};
    make_matrix_transpose(-dx, -dy, -dz, d);
    make_matrix_scale(sx, sy, sz, s);
    make_matrix_round_x(-theta_x, rx);
    make_matrix_round_y(-theta_y, ry);
    make_matrix_round_z(-theta_z, rz);

    vtype* mat44s[] = {rx, ry, rz, d, s};
    mul44s(5, (mat44*)mat44s, r);
}
bool is_in_2d_poligon(int c, vertex* _poligon[], mat21 a) {
    for (int i = 0; i < c; i++) {
        int next = i == c - 1 ? 0 : i + 1;
        vtype d[2] = {0};
        vector_sub(_poligon[next]->coordinate2d, _poligon[i]->coordinate2d, d, 2);

        vtype d2[2] = {0};
        vector_sub(a, _poligon[i]->coordinate2d, d2, 2);

        if (vector_product2(d, d2) > 0) {
            return false;
        }
    }
    return true;
}

point_info** make_raster_map(int w, int h) {
    return (point_info**)calloc(sizeof(point_info*), w * h);
}

void clear_raster_map(point_info** map, int w, int h) {
    memset(map, 0, w * h * sizeof(point_info*));
}

void poligon_rect(int c, vertex* _vertices[], mat21 min, mat21 max) {
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

#define SET_POINT_RASTER_MAP(__map, w, h, x, y, _point) \
    if (0 <= x && x < w && 0 <= y && y < h) {           \
        __map[x + y * w] = _point;                      \
    }

vtype triangle_area(mat21 p, mat21 a, mat21 b) {
    return fabs((a[0] - p[0]) * (b[1] - p[1]) - (a[1] - p[1]) * (b[0] - p[0])) / 2;
}

void write_raster_map(point_info** __map, int w, int h, poligon* _poligon) {
    vtype min[2] = {0};
    vtype max[2] = {0};
    poligon_rect(3, _poligon->vertices, min, max);

    for (int i = min[0]; i < max[0]; ++i) {
        for (int j = min[1]; j < max[1]; ++j) {
            vtype v[2] = {i, j};
            if (is_in_2d_poligon(3, _poligon->vertices, v)) {
                point_info* p = (point_info*)calloc(sizeof(point_info), 1);
                vtype area_0 = triangle_area(v, _poligon->vertices[1]->coordinate2d, _poligon->vertices[2]->coordinate2d);
                vtype area_1 = triangle_area(v, _poligon->vertices[2]->coordinate2d, _poligon->vertices[0]->coordinate2d);
                vtype area_2 = triangle_area(v, _poligon->vertices[0]->coordinate2d, _poligon->vertices[1]->coordinate2d);
                vtype area_sum = area_0 + area_1 + area_2;
                p->color[0] = area_0 / area_sum * _poligon->vertices[0]->color[0] +
                              area_1 / area_sum * _poligon->vertices[1]->color[0] +
                              area_2 / area_sum * _poligon->vertices[2]->color[0];
                p->color[1] = area_0 / area_sum * _poligon->vertices[0]->color[1] +
                              area_1 / area_sum * _poligon->vertices[1]->color[1] +
                              area_2 / area_sum * _poligon->vertices[2]->color[1];
                p->color[2] = area_0 / area_sum * _poligon->vertices[0]->color[2] +
                              area_1 / area_sum * _poligon->vertices[1]->color[2] +
                              area_2 / area_sum * _poligon->vertices[2]->color[2];
                p->_poligon = _poligon;
                SET_POINT_RASTER_MAP(__map, w, h, i, j, p)
            }
        }
    }
}

void raster_map_to_buffer(point_info** __map, unsigned char* __buf, int w, int h) {
    for (int i = 0, len = w * h; i < len; i++) {
        if (__map[i]) {
            __buf[i * 3] = __map[i]->color[0];
            __buf[i * 3 + 1] = __map[i]->color[1];
            __buf[i * 3 + 2] = __map[i]->color[2];
        }
    }
}

void convert_object(object* _object, mat44 wcp, mat44 ps, point_info** _map, int w, int h) {
    vtype lw[16] = {0};
    make_local_to_world_mat44(_object->dx, _object->dy, _object->dz,
                              _object->sx, _object->sy, _object->sz,
                              _object->theta_x, _object->theta_y, _object->theta_z, lw);
    vtype lp[16] = {0};
    mul4444(wcp, lw, lp);

    vtype r[4] = {0},
          r2[4] = {0};
    for (int i = 0; i < _object->poligon_count; i++) {
        poligon* _poligon = _object->poligons[i];
        for (int j = 0; j < 3; j++) {
            vertex* _vertex = _poligon->vertices[j];
            if (!_vertex->converted) {
                init_mat(r, 4, 1);
                // ローカル->ワールド->プロジェクション座標変換
                mul44(lp, _vertex->coordinate, r);

                vector_div(r, r[3], r, 4);

                init_mat(r2, 4, 1);
                // スクリーン座標変換
                mul44(ps, r, r2);

                // 結果格納
                memcpy(_vertex->coordinate2d, r2, sizeof(_vertex->coordinate2d));
                _vertex->converted = true;
            }
        }
        write_raster_map(_map, w, h, _poligon);
    }
}

vertex* create_vertex(vtype x, vtype y, vtype z, rgb* color) {
    vertex* _v = (vertex*)calloc(sizeof(vertex), 1);
    _v->coordinate[0] = x;
    _v->coordinate[1] = y;
    _v->coordinate[2] = z;
    _v->coordinate[3] = 1;
    _v->color[0] = color->r;
    _v->color[1] = color->g;
    _v->color[2] = color->b;
    return _v;
}

poligon* create_poligon(vertex* v1, vertex* v2, vertex* v3) {
    poligon* _p = (poligon*)calloc(sizeof(poligon), 1);
    _p->vertices[0] = v1;
    _p->vertices[1] = v2;
    _p->vertices[2] = v3;
    return _p;
}

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
            create_poligon(vs[1], vs[3], vs[2]),
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

        point_info** map = make_raster_map(w, h);
        unsigned char* buf = make_buffer(w, h, 255);
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 4; j++) {
                vs[j]->converted = false;
            }
            clear_raster_map(map, w, h);
            clear_buffer(buf, w, h);
            _object.theta_y = i * 5 * 3.14 / 180.0;
            convert_object(&_object, wcp, ps, map, w, h);

            raster_map_to_buffer(map, buf, w, h);
            char name[16] = {0};
            sprintf(name, "hoge-%d.ppm", i);
            write_buffer(buf, w, h, name);
        }
    }
    exit(0);
    {
        vtype r[4] = {0};
        vtype r2[4] = {0};

        // vtype locals[] = {5, 5, 5, 1,
        //                   5, -5, 5, 1,
        //                   -5, -5, 5, 1,
        //                   -5, 5, 5, 1,
        //                   5, 5, -5, 1,
        //                   5, -5, -5, 1,
        //                   -5, -5, -5, 1,
        //                   -5, 5, -5, 1};
        vtype locals[] = {5, 5, 5, 1,
                          -5, 5, -5, 1,
                          5, -5, -5, 1,
                          -5, 5, 5, 1};

        vtype camera[4] = {14, 0, -10, 1};
        vtype target[4] = {20, 20, 20, 1};
        vtype upper[4] = {0, 1, 0, 1};
        vtype lw[16] = {0};
        vtype wc[16] = {0};
        vtype cp[16] = {0};
        vtype ps[16] = {0};
        make_world_to_camera_mat44(camera, target, upper, wc);
        make_camera_to_projection_mat44(120, (double)w / h, 10, 100, cp);
        make_projection_to_screen_mat44(w, h, ps);

        for (int i = 0; i < 100; i++) {
            init_mat(lw, 4, 4);
            make_local_to_world_mat44(-15, -15, -15, 1, 1, 1, 0, i * 5 * 3.14 / 180.0, 0, lw);
            vtype lp[16] = {0};
            vtype* mat44s[] = {lw, wc, cp};
            mul44s(3, (mat44*)mat44s, lp);

            unsigned char* buf = make_buffer(w, h, 255);
            for (int i = 0; i < 4; i++) {
                vtype* p_local = locals + i * 4;

                init_mat(r, 4, 1);
                mul44(lp, p_local, r);

                vector_div(r, r[3], r, 4);

                init_mat(r2, 4, 1);
                mul44(ps, r, r2);

                // mat_print(r2, 4, 1);

                SET_BUFFER_RGB(buf, w, h, (int)r2[0], (int)r2[1], white);
            }
            char name[16] = {0};
            sprintf(name, "hoge-%d.ppm", i);
            write_buffer(buf, w, h, name);
        }
    }
}