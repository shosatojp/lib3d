#pragma once

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define l3POLIGON_VERTEX_COUNT 3

typedef double vtype;
typedef vtype* mat;
typedef vtype* mat44;
typedef vtype* mat41;  // 3次元座標、インデックス3は1
typedef vtype* mat21;

typedef struct {
    vtype r, g, b;
} rgb;

typedef struct {
    /**
     * ローカル座標空間の座標
     */
    vtype coordinate[4];
    /**
     * スクリーン座標空間の座標
     */
    vtype coordinate2d[4];
    /**
     * RGB値
     */
    rgb color;
    /**
     * 複数ポリゴンで重複して使用している場合に無駄に計算しないように
     * 変換行列が変わったときはリセットする必要あり
     */
    bool converted;
} vertex;

// textureもここで
typedef struct {
    /**
     * ポリゴンに属する頂点、ポリゴンは必ず三角形
     */
    vertex* vertices[3];
    /**
     * ソート用（裏は描画されないからいらなかったりして）
     */
    vtype max_z;
} poligon;

typedef struct {
    poligon** poligons;  // poligonの配列
    int poligon_count;
    vtype dx, dy, dz;
    vtype sx, sy, sz;
    vtype theta_x, theta_y, theta_z;
} object;

typedef struct {
    // ピクセルの色
    rgb color;
    // ポリゴンへのポインタ
    poligon* _poligon;
    // 変更したか
    bool activated;
} pixel_info;