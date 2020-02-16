#pragma once

#define PI 3.14159265359
#define pow2(a) ((a) * (a))
#define radians(degree) ((degree)*PI / 180)

#include <math.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "array.h"

#define l3POLIGON_VERTEX_COUNT 3

typedef float l3Type;  // floatのが速い
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

typedef enum _l3PoligonType {
    l3PoligonTypeTriangle = 0,
    l3PoligonTypeShpere,
    l3PoligonTypeSky,
    l3PoligonTypePlane,
} l3PoligonType;

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
     * ワールド座標空間の座標
     */
    l3Mat41A coordinateWorld;
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

    // 三角形ポリゴン用のパラメータ
    l3Mat31A e1;
    l3Mat31A e2;
    l3Mat31A normal;

    union {
        /**
         * ソート用
         */
        l3Type max_z;
        /**
         * l3PoligonType == l3PoligonSphereのとき
         */
        l3Type sphere_radius;
    };
    /**
     * ポリゴンを囲う四角形の左上と右下
     */
    l3Mat21A min;
    l3Mat21A max;
    /**
     * テクスチャマッピング用。verticesに対応する順番で
     */
    l3Texture* texture;
    // 三角形ポリゴン用
    l3Mat23 textureVertices;  // heap 解放
    l3Mat33 textureAffineMatInv;  // heap 解放

    l3RGB color; // ベースカラー？？

    /**
     * このポリゴンにどのマテリアルを使うか
     */
    l3PoligonMaterial material;

    // 光の透過率
    l3Type transparency;
    // 金属度(RGB毎に)
    // (これと視線の入射角から拡散反射係数、鏡面反射係数を求める)
    // スカイオブジェクトは0
    // この値(からもとまる拡散反射係数)の比率で色が決まる？？
    l3Type metalness[3];
    // 鏡面反射光の取得範囲(1/光沢度)
    // 鏡面反射光はいくつかの方向のものを(cos theta)^alphaを掛けて足し合わせる
    l3Type roughness;
    // 光源としての強度
    // もしくはcolorの1以上で表現する
    l3Type lightIntensity;

    /**
     * ポリゴンの種類
     * - 三角形ポリゴン
     * - 球体 (座標(vertices[0]),上方向(vertices[1])、半径(max_z))
     */
    l3PoligonType poligonType;
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
    l3Type bounding_radius;
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
    l3Type angle;
    l3Type near;
    l3Type far;
    l3Mat41A cameraZ;
} l3CameraInfo;

struct _l3Environment;
typedef struct _l3Environment l3Environment;
typedef void l3FrameTransitionFunction(l3Environment* env, int frame);
typedef void l3Renderer(l3Environment* env);

typedef enum _l3MultiThreadRenderingType{
    l3MultiThreadRenderingTypeRandom,
    l3MultiThreadRenderingTypeSequential,
}l3MultiThreadRenderingType;

// マルチスレッド時にこれを持ってく
struct _l3Environment {
    int w, h;
    // オブジェクトのポインタの配列
    array objects;  // 中身はheap

    // Textureは状態を保持しないのでそのままでおｋ
    // カメラ情報
    l3CameraInfo camera;
    // 変換行列？
    l3MultiThreadRenderingType renderType;
    int frame_begin;
    int frame_end;
    int thread_count;
    // int frame_per_thread;
    // time
    l3PixelInfo* map;
    const char* outdir;
    l3FrameTransitionFunction* transitionFn;
    array poligons;  // 描画用のポリゴンリスト solve ptrsで作成
};

typedef struct _l3Ray {
    // 着地前の情報
    l3Mat41A rayStartPoint;
    l3Mat41A rayDirection;
    // 着地地点の情報
    l3Mat41A intersection;
    l3Mat21A uv;
    l3Poligon* poligon;
    // 色情報（再帰的に決まる）
    l3RGB color;
} l3Ray;