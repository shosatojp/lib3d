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

typedef enum _l3PoligonMaterial {
    l3PoligonMaterialVertex = 0,  // default
    l3PoligonMaterialColor,
    l3PoligonMaterialTexture,
} l3PoligonMaterial;

typedef struct _l3RGB {
    l3Type r, g, b;
} l3RGB;

typedef struct _l3Texture {
    unsigned char* buffer;
    int w, h;
} l3Texture;

typedef struct _l3Vertex {
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
typedef struct _l3Poligon {
    /**
     * ポリゴンに属する頂点のインデックス（基準はEnv）、ポリゴンは必ず三角形
     */
    l3Vertex* vertices[l3POLIGON_VERTEX_COUNT];  // 解放の必要なし
    int vertex_indices[l3POLIGON_VERTEX_COUNT];  // 解放の必要なし
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
    l3Mat23 textureVertices;  // heap 解放
    l3Texture* texture;
    l3Mat33 textureAffineMatInv;  // heap 解放

    l3RGB color;

    /**
     * このポリゴンにどのマテリアルを使うか
     */
    l3PoligonMaterial material;
} l3Poligon;

/**
 * オブジェクトごとに頂点とポリゴンを管理する
 * -> オブジェクトのDestructと同時にどちらも解放して良い
 * ポリゴンは直接ポインタ格納、
 * 頂点はリストを格納
 */
typedef struct _l3Object {
    l3Poligon** poligons;  // poligonのポインタの配列のインデックスの配列 // heap 中身開放する
    int poligon_count;

    array vertices;

    l3Type dx, dy, dz;
    l3Type sx, sy, sz;
    l3Type theta_x, theta_y, theta_z;
} l3Object;

typedef struct _l3PixelInfo {
    // ピクセルの色
    l3RGB color;
    // ポリゴンへのポインタ
    l3Poligon* _poligon;
    // 変更したか
    bool activated;
} l3PixelInfo;

typedef struct _l3CameraInfo {
    l3Mat41A coordinate;
    l3Mat41A target;
    l3Mat41A upper;
} l3CameraInfo;

struct _l3Environment;
typedef struct _l3Environment l3Environment;
typedef void l3FrameTransitionFunction(l3Environment* env, int frame);

// マルチスレッド時にこれを持ってく
struct _l3Environment {
    int w, h;
    // オブジェクトのポインタの配列
    array objects;  // 中身はheap

    // Textureは状態を保持しないのでそのままでおｋ
    // カメラ情報
    l3CameraInfo camera;
    // 変換行列？

    int frame_begin;
    int frame_end;
    // time
    l3FrameTransitionFunction* transitionFn;
};