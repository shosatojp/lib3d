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
        }
    }
}

/**
 * a    4*4
 * b    4*4
 * r    4*1
 * 
 * a != r, b != r
 * must initialize r : true
 * 
 */
void mul4444(mat44 a, mat44 b, mat41 r) {
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

vtype unit_y[] = {0, 1, 0};

vtype inner_product_1d(mat a, mat b, int n) {
    vtype sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

// void view_axis(mat41 camera, mat41 target, mat41 up, mat41 cx, mat41 cy, mat41 cz) {
//     // cz
//     vector_sub(target, camera, cz, 3);
//     normarize_vector(cz, cz, 3);
//     cz[3] = 1;

//     // cx
//     vector_product3(&unit_y, cz, cx);
//     normarize_vector(cx, cx, 3);
//     cx[3] = 1;

//     // cy
//     vector_product3(cz, cx, cy);
//     normarize_vector(cy, cy, 3);
//     cy[3] = 1;
// }

/**
 * camera = target = upper = r
 */
void world_to_camera_mat44(mat41 camera, mat41 target, mat41 upper, mat44 r) {
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

void camera_to_projection_mat44(vtype angle, vtype aspect, vtype near, vtype far, mat44 r) {
    vtype tan_angle_2 = tan(angle / 2);
    r[0] = 1 / tan_angle_2;
    r[5] = 1 / (tan_angle_2 * aspect);
    r[10] = far / (far - near);
    r[11] = 1;
    r[14] = -near * r[10];
}

void projection_to_screen_mat44(vtype width, vtype height, mat44 r) {
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

int main() {
    vtype a[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    // vtype b[] = {1, 2, 3, 1};
    vtype r[4] = {0};
    vtype r2[4] = {0};
    int w = 400, h = 200;
    // mul44(a, b, r);

    vtype p_local[] = {10, 10, 10, 1};
    vtype locals[] = {10, 10, 10, 1,
                      10, 20, 10, 1,
                      20, 20, 10, 1,
                      20, 10, 10, 1,
                      10, 10, 20, 1,
                      10, 20, 20, 1,
                      20, 20, 20, 1,
                      20, 10, 20, 1
                      };
    vtype p_global[] = {0};

    vtype s[16] = {0};
    vtype rx[16] = {0};
    vtype ry[16] = {0};
    vtype rz[16] = {0};

    // make_matrix_scale(2, 2, 4, s);
    vtype camera[4] = {14, 20, -10, 1};
    vtype target[4] = {20, 20, 20, 1};
    vtype upper[4] = {0, 1, 0, 1};
    vtype wc[16] = {0};
    world_to_camera_mat44(camera, target, upper, wc);
    vtype cp[16] = {0};
    camera_to_projection_mat44(90, 0.5, 10, 100, cp);
    vtype ps[16] = {0};
    projection_to_screen_mat44(w, h, ps);

    unsigned char* buf = make_buffer(w, h, 255);
    for (int i = 0; i < 8; i++) {
        vtype* p_local = locals + i * 4;
        init_mat(r2, 4, 1);
        init_mat(r, 4, 1);
        mul44(wc, p_local, r2);
        // mat_print(r2, 4, 1);
        mat_print(r2, 4, 1);
        mul44(cp, r2, r);
        // mat_print(r, 4, 1);

        vector_div(r, r[3], r, 4);
        // mat_print(r, 4, 1);
        // mat_print(ps, 4, 4);
        init_mat(r2, 4, 1);
        mul44(ps, r, r2);

        int white[3] = {0, 0, 0};
        SET_BUFFER_RGB(buf, w, h, (int)r2[0], (int)r2[1], white);
    }
    write_buffer(buf, w, h, "hoge.ppm");
}