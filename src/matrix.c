#include "lib3d.h"

/**
 * 行列を零行列に初期化する
 */
void l3InitMat(mat a, int x, int y) {
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
void l3MulMat4441(mat44 a, mat41 b, mat41 r) {
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
void l3MulMat4444(mat44 a, mat44 b, mat44 r) {
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
void l3SubMat(mat a, mat b, mat r, int n) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] - b[i];
    }
}

/**
 * a = r
 * must initialize r : false
 */
void l3DivMat(mat a, vtype d, mat r, int n) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] / d;
    }
}

void l3PrintMat(mat a, int x, int y) {
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
void l3NormarizeVec(mat a, mat r, int n) {
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
void l3CrossProductVec3(mat41 a, mat41 b, mat41 r) {
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
    r[3] = 1;
}

vtype l3CrossProductVec2(mat21 a, mat21 b) {
    return a[0] * b[1] - a[1] * b[0];
}

vtype l3InnerProductVec(mat a, mat b, int n) {
    vtype sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

void l3MulMat44s44(int c, mat44* as, mat44 r) {
    if (c <= 1) {
        fprintf(stderr, "invalid matrix count\n");
    }
    switch (c) {
        case 2:
            l3MulMat4444(as[1], as[0], r);
        default: {
            vtype tmp0[16] = {0};
            vtype tmp1[16] = {0};
            l3MulMat4444(as[c - 1], as[c - 2], tmp0);
            int tmp_num = 0;
            for (int i = c - 3; i > 0; i--) {
                if (tmp_num == 0) {
                    l3InitMat(tmp1, 4, 4);
                    l3MulMat4444(tmp0, as[i], tmp1);
                    tmp_num = 1;
                } else {
                    l3InitMat(tmp0, 4, 4);
                    l3MulMat4444(tmp1, as[i], tmp0);
                    tmp_num = 0;
                }
            }
            if (tmp_num == 0) {
                l3MulMat4444(tmp0, as[0], r);
            } else {
                l3MulMat4444(tmp1, as[0], r);
            }
        }
    }
}

bool l3InsideOfPoligon2D(int c, vertex* _poligon[], mat21 a) {
    for (int i = 0; i < c; i++) {
        int next = i == c - 1 ? 0 : i + 1;
        vtype d[2] = {0};
        l3SubMat(_poligon[next]->coordinate2d, _poligon[i]->coordinate2d, d, 2);

        vtype d2[2] = {0};
        l3SubMat(a, _poligon[i]->coordinate2d, d2, 2);

        if (l3CrossProductVec2(d, d2) > 0) {
            return false;
        }
    }
    return true;
}
