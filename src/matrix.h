#pragma once

#include "common.h"

#define l3MatAt(a, h, i, j) (a)[(j) * (h) + (i)]

#define l3CopyMat(a, r, n) \
    memcpy(r, a, sizeof(l3Type) * n)

l3Mat l3CloneMat(l3Mat a, int h, int w);

/**
 * 行列を零行列に初期化する
 */
void l3InitMat(l3Mat a, int x, int y);

/**
 * a    4*4
 * b    4*1
 * r    4*1
 * 
 * a != r, b != r
 * must initialize r : true
 */
void l3MulMat4441(l3Mat44 a, l3Mat41 b, l3Mat41 r);

/**
 * a    4*4
 * b    4*4
 * r    4*4
 * 
 * a != r, b != r
 * must initialize r : true
 * 
 */
void l3MulMat4444(l3Mat44 a, l3Mat44 b, l3Mat44 r);
void l3MulMat(l3Mat a, l3Mat b, l3Mat r, int s, int t, int u);

/**
 * a = r, b = r
 * must initialize r : false
 */
// void l3SubMat(l3Mat a, l3Mat b, l3Mat r, int n);
#define l3SubMat(a, b, r, n)        \
    for (int i = 0; i < (n); ++i) { \
        (r)[i] = (a)[i] - (b)[i];   \
    }
#define l3SubMat2(a, b, r)    \
    (r)[0] = (a)[0] - (b)[0]; \
    (r)[1] = (a)[1] - (b)[1];
#define l3SubMat3(a, b, r)    \
    (r)[0] = (a)[0] - (b)[0]; \
    (r)[1] = (a)[1] - (b)[1]; \
    (r)[2] = (a)[2] - (b)[2];

// void l3AddMat(l3Mat a, l3Mat b, l3Mat r, int n);
#define l3AddMat(a, b, r, n)        \
    for (int i = 0; i < (n); ++i) { \
        (r)[i] = (a)[i] + (b)[i];   \
    }
#define l3AddMat3(a, b, r)    \
    (r)[0] = (a)[0] + (b)[0]; \
    (r)[1] = (a)[1] + (b)[1]; \
    (r)[2] = (a)[2] + (b)[2];
// void l3ScalarMulMat(l3Mat a, l3Type s, l3Mat r, int n);
#define l3ScalarMulMat(a, s, r, n)  \
    for (int i = 0; i < (n); ++i) { \
        (r)[i] = (a)[i] * (s);      \
    }
#define l3ScalarMulMat3(a, s, r) \
    (r)[0] = (a)[0] * s;         \
    (r)[1] = (a)[1] * s;         \
    (r)[2] = (a)[2] * s;
l3Type l3DistanceVec(l3Mat31 a, l3Mat31 b);
/**
 * a = r
 * must initialize r : false
 */
// void l3DivMat(l3Mat a, l3Type d, l3Mat r, int n);
#define l3DivMat(a, d, r, n)            \
    {                                   \
        for (int i = 0; i < (n); ++i) { \
            (r)[i] = (a)[i] / (d);      \
        }                               \
    }

void l3PrintMat(l3Mat a, int h, int w);

/**
 * a = r
 */
void l3NormarizeVec(l3Mat a, l3Mat r, int n);
/**
 * a = r, b = r
 */
// void l3CrossProductVec3(l3Mat31 a, l3Mat31 b, l3Mat31 r);
#define l3CrossProductVec3(a, b, r)             \
    (r)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1]; \
    (r)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2]; \
    (r)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0];
// (r)[3] = 1;

l3Type l3CrossProductVec2(l3Mat21 a, l3Mat21 b);
// l3Type l3InnerProductVec(l3Mat a, l3Mat b, int n);
#define l3InnerProductVec(a, b, n) ({ \
    l3Type sum = 0.0;                 \
    for (int i = 0; i < (n); ++i) {   \
        sum += (a)[i] * (b)[i];       \
    }                                 \
    sum;                              \
})
#define l3InnerProductVec3(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
void l3MulMat44s44(int c, l3Mat44* as, l3Mat44 r);
bool l3InsideOfPoligon2D(int c, l3Vertex* _poligon[], l3Mat21 a);
void l3InverseMat(int n, l3Mat44 a, l3Mat44 r);
void l3SimplificateMat(l3Mat a, int h, int w);
void l3GetAffineTransformMat33(l3Mat21 src[3], l3Mat21 dst[3], l3Mat33 r);
void l3SwapRow(l3Mat a, int h, int w, int p, int q);
void l3SwapRows(l3Mat a, int h, int w);
l3Type l3VecAbs(l3Mat a, int n);

#define l3VecAbs3(a) (sqrt((a)[0] * (a)[0] + (a)[1] * (a)[1] + (a)[2] * (a)[2]))
#define l3DistanceVec3(a, b) ({ \
    l3Mat31A tmp;               \
    l3SubMat3((a), (b), tmp);   \
    l3VecAbs3(tmp);             \
})

#define l3NormarizeVec3(a, r)    \
    {                            \
        l3Type s = l3VecAbs3(a); \
        r[0] = a[0] / s;         \
        r[1] = a[1] / s;         \
        r[2] = a[2] / s;         \
    }
