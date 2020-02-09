#pragma once

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define l3POLIGON_VERTEX_COUNT 3

typedef double l3Type;
typedef l3Type* l3Mat;
typedef l3Type* l3Mat44;
typedef l3Type* l3Mat41;  // 3次元座標、インデックス3は1
typedef l3Type* l3Mat21;

typedef struct {
    l3Type r, g, b;
} l3RGB;

typedef struct {
    /**
     * ローカル座標空間の座標
     */
    l3Type coordinate[4];
    /**
     * スクリーン座標空間の座標
     */
    l3Type coordinate2d[4];
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