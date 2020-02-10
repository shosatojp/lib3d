#include "lib3d.h"

/**
 * 行列を零行列に初期化する
 */
void l3InitMat(l3Mat a, int x, int y) {
    memset(a, 0, sizeof(l3Type) * x * y);
}

/**
 * a    4*4
 * b    4*1
 * r    4*1
 * 
 * a != r, b != r
 * must initialize r : true
 */
void l3MulMat4441(l3Mat44 a, l3Mat41 b, l3Mat41 r) {
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
void l3MulMat4444(l3Mat44 a, l3Mat44 b, l3Mat44 r) {
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
void l3SubMat(l3Mat a, l3Mat b, l3Mat r, int n) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] - b[i];
    }
}

/**
 * a = r
 * must initialize r : false
 */
void l3DivMat(l3Mat a, l3Type d, l3Mat r, int n) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] / d;
    }
}

void l3PrintMat(l3Mat a, int h, int w) {
    for (int i = 0; i < h; ++i) {
        if (i == 0)
            printf("┌ ");
        else if (i == h - 1)
            printf("└ ");
        else
            printf("│ ");
        for (int j = 0; j < w; ++j) {
            printf("%5.2f ", l3MatAt(a, h, i, j));
        }
        if (i == 0)
            printf("┐");
        else if (i == h - 1)
            printf("┘");
        else
            printf("│");
        printf("\n");
    }
}

/**
 * a = r
 */
void l3NormarizeVec(l3Mat a, l3Mat r, int n) {
    l3Type sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * a[i];
    }
    l3Type s = sqrt(sum);
    for (int i = 0; i < n; ++i) {
        r[i] = a[i] / s;
    }
}

/**
 * a = r, b = r
 */
void l3CrossProductVec3(l3Mat41 a, l3Mat41 b, l3Mat41 r) {
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
    r[3] = 1;
}

l3Type l3CrossProductVec2(l3Mat21 a, l3Mat21 b) {
    return a[0] * b[1] - a[1] * b[0];
}

l3Type l3InnerProductVec(l3Mat a, l3Mat b, int n) {
    l3Type sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

void l3MulMat44s44(int c, l3Mat44 as[], l3Mat44 r) {
    if (c <= 1) {
        fprintf(stderr, "invalid matrix count\n");
    }
    switch (c) {
        case 2:
            l3MulMat4444(as[0], as[1], r);
        default: {
            l3Type tmp0[16] = {0};
            l3Type tmp1[16] = {0};
            l3MulMat4444(as[c - 2], as[c - 1], tmp0);
            int tmp_num = 0;
            for (int i = c - 3; i > 0; i--) {
                if (tmp_num == 0) {
                    l3InitMat(tmp1, 4, 4);
                    l3MulMat4444(as[i], tmp0, tmp1);
                    tmp_num = 1;
                } else {
                    l3InitMat(tmp0, 4, 4);
                    l3MulMat4444(as[i], tmp1, tmp0);
                    tmp_num = 0;
                }
            }
            if (tmp_num == 0) {
                l3MulMat4444(as[0], tmp0, r);
            } else {
                l3MulMat4444(as[0], tmp1, r);
            }
        }
    }
}

bool l3InsideOfPoligon2D(int c, l3Vertex* _poligon[], l3Mat21 a) {
    for (int i = 0; i < c; i++) {
        int next = i == c - 1 ? 0 : i + 1;
        l3Type d[2] = {0};
        l3SubMat(_poligon[next]->coordinate2d, _poligon[i]->coordinate2d, d, 2);

        l3Type d2[2] = {0};
        l3SubMat(a, _poligon[i]->coordinate2d, d2, 2);

        if (l3CrossProductVec2(d, d2) > 0) {
            return false;
        }
    }
    return true;
}

void l3GetAffineTransformMat33(l3Mat21 src[3], l3Mat21 dst[3], l3Mat33 r) {
    l3Mat a = (l3Mat)calloc(sizeof(l3Type), 6 * 7);
    l3Mat b = a + 6 * 6;

    for (int i = 0; i < 3; i++) {
        l3MatAt(a, 6, i * 2, 0) = l3MatAt(a, 6, i * 2 + 1, 3) = src[i][0];
        l3MatAt(a, 6, i * 2, 1) = l3MatAt(a, 6, i * 2 + 1, 4) = src[i][1];
        l3MatAt(a, 6, i * 2, 2) = l3MatAt(a, 6, i * 2 + 1, 5) = 1;
        l3MatAt(b, 6, i * 2, 0) = dst[i][0];
        l3MatAt(b, 6, i * 2 + 1, 0) = dst[i][1];
    }
    l3PrintMat(a, 6, 7);

    l3SimplificateMat(a, 6, 7);
    l3PrintMat(a, 6, 7);

    l3MatAt(r, 3, 0, 0) = b[0];
    l3MatAt(r, 3, 0, 1) = b[1];
    l3MatAt(r, 3, 0, 2) = b[2];
    l3MatAt(r, 3, 1, 0) = b[3];
    l3MatAt(r, 3, 1, 1) = b[4];
    l3MatAt(r, 3, 1, 2) = b[5];
    l3MatAt(r, 3, 2, 0) = 1;
    l3MatAt(r, 3, 2, 1) = 1;
    l3MatAt(r, 3, 2, 2) = 1;
}

void l3InverseMat(int n, l3Mat a, l3Mat r) {
    l3Mat tmp = (l3Mat)malloc(sizeof(l3Type) * n * n * 2);
    memcpy(tmp, a, sizeof(l3Type) * n * n);
    for (int k = 0; k < n; k++)
        l3MatAt(tmp, n, k, n + k) = 1.0;
    l3PrintMat(tmp, n, n * 2);
    l3SimplificateMat(tmp, n, n * 2);
    memcpy(r, tmp + n * n, sizeof(l3Type) * n * n);
    free(tmp);
}

void l3SwapRow(l3Mat a, int h, int w, int p, int q) {
    for (int j = 0; j < w; j++) {
        l3Type tmp = l3MatAt(a, h, p, j);
        l3MatAt(a, h, p, j) = l3MatAt(a, h, q, j);
        l3MatAt(a, h, q, j) = tmp;
    }
}

void l3SwapRows(l3Mat a, int h, int w) {
    for (int r = 0; r < h - 1; r++) {
        if (l3MatAt(a, h, r, r) == 0.0) {
            for (int t = r + 1; t < h; t++) {  // find non zero row
                if (l3MatAt(a, h, t, r) != 0) {
                    l3SwapRow(a, h, w, t, r);
                    break;
                }
            }
        }
    }
}

void l3SimplificateMat(l3Mat a, int h, int w) {
    l3Type piv, t;
    int i, j, k;

    l3SwapRows(a, h, w);

    for (k = 0; k < h; k++) {
        piv = l3MatAt(a, h, k, k);
        for (j = k; j < w; j++) {
            l3MatAt(a, h, k, j) /= piv;
        }
        for (i = 0; i < h; i++) {
            if (i != k) {
                t = l3MatAt(a, h, i, k);
                for (j = k; j < w; j++) {
                    l3MatAt(a, h, i, j) -= t * l3MatAt(a, h, k, j);
                }
            }
        }
    }
}