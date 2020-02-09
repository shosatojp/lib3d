#include "lib3d.h"

/**
 * 行列を零行列に初期化する
 */
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
