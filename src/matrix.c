#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "lib3d.h"

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
        vector_sub(_poligon[next]->coordinate, _poligon[i]->coordinate, d, 2);

        vtype d2[2] = {0};
        vector_sub(a, _poligon[i]->coordinate, d2, 2);

        if (vector_product2(d, d2) < 0) {
            return false;
        }
    }
    return true;
}

point** make_raster_map(int w, int h) {
    return (point**)calloc(sizeof(point*), w * h);
}

void poligon_rect(int c, vertex* _poligon[], mat21 min, mat21 max) {
    int maxx = _poligon[0]->coordinate[0],
        maxy = _poligon[0]->coordinate[1],
        minx = _poligon[0]->coordinate[0],
        miny = _poligon[0]->coordinate[1];

    for (int i = 1; i < c; i++) {
        if (_poligon[i]->coordinate[0] > maxx) maxx = _poligon[i]->coordinate[0];
        if (_poligon[i]->coordinate[1] > maxy) maxy = _poligon[i]->coordinate[1];
        if (_poligon[i]->coordinate[0] < minx) minx = _poligon[i]->coordinate[0];
        if (_poligon[i]->coordinate[1] < miny) miny = _poligon[i]->coordinate[1];
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

void write_raster_map(point** __map, int w, int h, int c, vertex* _poligon[]) {
    vtype min[2] = {0};
    vtype max[2] = {0};
    poligon_rect(c, _poligon, min, max);

    for (int i = min[0]; i < max[0]; ++i) {
        for (int j = min[1]; j < max[1]; ++j) {
            vtype v[2] = {i, j};
            if (is_in_2d_poligon(c, _poligon, v)) {
                point* p = (point*)calloc(sizeof(point), 1);
                p->color[0] = 0;
                p->color[1] = 0;
                p->color[2] = 0;
                SET_POINT_RASTER_MAP(__map, w, h, i, j, p)
            }
        }
    }
}

void raster_map_to_buffer(point** __map, unsigned char* __buf, int w, int h) {
    for (int i = 0, len = w * h; i < len; i++) {
        if (__map[i]) {
            __buf[i * 3] = __map[i]->color[0];
            __buf[i * 3 + 1] = __map[i]->color[1];
            __buf[i * 3 + 2] = __map[i]->color[2];
        }
    }
}

int main() {
    int w = 400, h = 200;
    vtype a[] = {100, 100};
    vtype b[] = {200, 100};
    vtype c[] = {100, 200};

    vertex va;
    vertex vb;
    vertex vc;

    va.coordinate = a;
    vb.coordinate = b;
    vc.coordinate = c;

    vertex* p[] = {&va, &vb, &vc};
    vertex** map = make_raster_map(w, h);
    write_raster_map(map, w, h, 3, p);

    unsigned char* buf = make_buffer(w, h, 255);
    raster_map_to_buffer(map, buf, w, h);
    write_buffer(buf, w, h, "test.ppm");

    exit(0);
    vtype r[4] = {0};
    vtype r2[4] = {0};

    vtype p_local[] = {10, 10, 10, 1};
    vtype locals[] = {5, 5, 5, 1,
                      5, -5, 5, 1,
                      -5, -5, 5, 1,
                      -5, 5, 5, 1,
                      5, 5, -5, 1,
                      5, -5, -5, 1,
                      -5, -5, -5, 1,
                      -5, 5, -5, 1};

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
        int white[3] = {0, 0, 0};

        unsigned char* buf = make_buffer(w, h, 255);
        for (int i = 0; i < 8; i++) {
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