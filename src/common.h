#pragma once

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

#define l3POLIGON_VERTEX_COUNT 3

typedef double l3Type;
typedef l3Type* l3Mat;
typedef l3Type* l3Mat44;
typedef l3Type* l3Mat41;  // 3次元座標、インデックス3は1
typedef l3Type* l3Mat21;
typedef l3Type* l3Mat33;  // Affine
typedef l3Type* l3Mat32;
typedef l3Type* l3Mat31;
typedef l3Type* l3Mat23;
typedef l3Type l3Mat44A[16];
typedef l3Type l3Mat41A[4];
typedef l3Type l3Mat21A[2];
typedef l3Type l3Mat33A[9];
typedef l3Type l3Mat32A[6];
typedef l3Type l3Mat31A[3];
typedef l3Type l3Mat23A[6];

typedef enum {
    l3PoligonMaterialVertex = 0,  // default
    l3PoligonMaterialColor,
    l3PoligonMaterialTexture,
} l3PoligonMaterial;

typedef struct {
    l3Type r, g, b;
} l3RGB;

typedef struct {
    unsigned char* buffer;
    int w, h;
} l3Texture;

typedef struct {
    /**
     * ローカル座標空間の座標
     */
    l3Mat41A coordinate;
    /**
     * スクリーン座標空間の座標
     */
    l3Mat41A coordinate2d;
    /**
     * RGB値
     */
    l3RGB color;
    /**
     * 複数ポリゴンで重複して使用している場合に無駄に計算しないように
     * 変換行列が変わったときはリセットする必要あり
     */
    bool converted;
} l3Vertex;

// textureもここで
typedef struct {
    /**
     * ポリゴンに属する頂点、ポリゴンは必ず三角形
     */
    l3Vertex* vertices[3];
    /**
     * ソート用（裏は描画されないからいらなかったりして）
     */
    l3Type max_z;
    /**
     * ポリゴンを囲う四角形の左上と右下
     */
    l3Mat21A min;
    l3Mat21A max;
    /**
     * テクスチャマッピング用。verticesに対応する順番で
     */
    l3Mat23 textureVertices;
    l3Texture* texture;
    l3Mat33 textureAffineMatInv;

    l3RGB color;

    /**
     * このポリゴンにどのマテリアルを使うか
     */
    l3PoligonMaterial material;
} l3Poligon;

typedef struct {
    l3Poligon** poligons;  // poligonの配列
    int poligon_count;
    l3Type dx, dy, dz;
    l3Type sx, sy, sz;
    l3Type theta_x, theta_y, theta_z;
} l3Object;

typedef struct {
    // ピクセルの色
    l3RGB color;
    // ポリゴンへのポインタ
    l3Poligon* _poligon;
    // 変更したか
    bool activated;
} l3PixelInfo;
