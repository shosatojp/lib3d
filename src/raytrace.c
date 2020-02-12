#include "lib3d.h"

/**
 * レイとポリゴンの交点判定（ワールド座標）
 * ray_origin: Rayの始点
 * ray_direction: Rayの方向ベクトル（単位ベクトル）
 * poligon: 判定対象のポリゴン
 * (result) r: return trueの場合、交点。return falseの場合何もしない
 * (result) uv: return trueの場合、ポリゴン内のuv座標< b - a, c - a >。return falseの場合何もしない
 * return: 交点がある場合true
 */
bool l3IntersectRayPoligon(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Poligon* poligon, l3Mat41 r, l3Mat21 uv) {
    l3Mat41A e1 = {0};
    l3Mat41A e2 = {0};
    l3Mat41A a_v0 = {0};
    l3SubMat(poligon->vertices[1]->coordinateWorld, poligon->vertices[0]->coordinateWorld, e1, 3);
    l3SubMat(poligon->vertices[2]->coordinateWorld, poligon->vertices[0]->coordinateWorld, e2, 3);
    l3SubMat(ray_origin, poligon->vertices[0]->coordinateWorld, a_v0, 3);

    l3Type tmp[12] = {0};
    tmp[0] = -ray_direction[0];
    tmp[1] = -ray_direction[1];
    tmp[2] = -ray_direction[2];
    memcpy(&tmp[3], e1, sizeof(l3Type) * 3);
    memcpy(&tmp[6], e2, sizeof(l3Type) * 3);
    memcpy(&tmp[9], a_v0, sizeof(l3Type) * 3);

    l3SimplificateMat(tmp, 3, 4);

    l3Type t = tmp[9];
    l3Type u = tmp[10];
    l3Type v = tmp[11];

    if (0 <= t && 0 <= u && u <= 1 && 0 <= v && v <= 1 && u + v <= 1) {
        r[0] = ray_origin[0] + t * ray_direction[0];
        r[1] = ray_origin[1] + t * ray_direction[1];
        r[2] = ray_origin[2] + t * ray_direction[2];
        uv[0] = u;
        uv[1] = v;
        return true;
    } else {
        return false;
    }
}

/**
 * ポリゴンの法線ベクトル（ワールド座標）
 * poligon: 対象のポリゴン
 * (result) normal: 法線ベクトル（ワールド座標）
 */
void l3GetPoligonNormal(l3Poligon* poligon, l3Mat41 normal) {
    l3Mat41A e1 = {0};
    l3Mat41A e2 = {0};
    l3Mat41A prod = {0};
    l3SubMat(poligon->vertices[2]->coordinateWorld, poligon->vertices[0]->coordinateWorld, e1, 3);
    l3SubMat(poligon->vertices[1]->coordinateWorld, poligon->vertices[0]->coordinateWorld, e2, 3);
    l3CrossProductVec3(e2, e1, prod);
    l3NormarizeVec(prod, normal, 3);
}

/**
 * レイトレーシングでポリゴンの点(u,v)のテクスチャ座標
 * poligon: 対象のポリゴン
 * uv: ポリゴン内のuv座標
 * (result) r: テクスチャ内でのxy座標（0-1）
 */
void l3GetRayPoligon2DTextureCoordinate(l3Poligon* poligon, l3Mat21 uv, l3Mat21 r) {
    l3Mat21A e1 = {0};
    l3Mat21A e2 = {0};
    l3SubMat(&poligon->textureVertices[4], &poligon->textureVertices[0], e1, 2);
    l3SubMat(&poligon->textureVertices[2], &poligon->textureVertices[0], e2, 2);
    r[0] = poligon->textureVertices[0] + uv[0] * e1[0] + uv[1] * e2[0];
    r[1] = poligon->textureVertices[1] + uv[0] * e1[1] + uv[1] * e2[1];
}

/**
 * レイと球の交点判定（ワールド座標）
 * ray_origin: Rayの始点
 * ray_direction: Rayの方向ベクトル（単位ベクトル）
 * sphere_center: 判定対象の球の中心座標
 * sphere_radius: 判定対象の球の半径
 * (result) r: return trueの場合、交点。return falseの場合何もしない
 * return: 交点がある場合true
 */
bool l3IntersectRaySphere(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Mat41 sphere_center, l3Type sphere_radius, l3Mat41 r) {
    l3Mat41A s_pc;
    l3SubMat(ray_origin, sphere_center, s_pc, 3);
    l3Type b = l3InnerProductVec(s_pc, ray_direction, 3);
    l3Type c = l3InnerProductVec(ray_origin, ray_origin, 3) +
               l3InnerProductVec(sphere_center, sphere_center, 3) -
               sphere_radius * sphere_radius - 2 * l3InnerProductVec(ray_origin, sphere_center, 3);
    l3Type d = b * b - c;
    if (d > 0) {
        l3Type t = -b - sqrt(d);
        r[0] = ray_origin[0] + t * ray_direction[0];
        r[1] = ray_origin[1] + t * ray_direction[1];
        r[2] = ray_origin[2] + t * ray_direction[2];
        return true;
    } else {
        return false;
    }
}

/**
 * レイトレーシングで球上の点のテクスチャ座標
 * sphere_local: 球のローカル座標空間での点の位置
 * (result) r: テクスチャ内でのr座標（0-1）
 */
void l3GetRaySphere2DTextureCoordinate(l3Mat41 sphere_local, l3Mat21 r) {
    l3Type theta = acos(sphere_local[0] / sqrt(pow2(sphere_local[0]) + pow2(sphere_local[2])));
    l3Type phi = acos(sphere_local[1] / sqrt(pow2(sphere_local[0]) + pow2(sphere_local[1])));
    if (sphere_local[2] < 0) {
        theta = 2 * PI - theta;
    }
    r[0] = theta / (2 * PI);
    r[1] = phi / PI;
}

/**
 * 球の法線ベクトル
 * sphere_local: 球のローカル座標空間での点の位置
 * (result) normal: 法線ベクトル
 */
void l3GetSphereNormal(l3Mat41 sphere_local, l3Mat41 r) {
    l3NormarizeVec(sphere_local, r, 3);
}

/**
 * 反射ベクトル
 * incident_vec: 入射ベクトル
 * normal: 法線ベクトル
 * (result) r: 反射ベクトル
 */
void l3GetReflectedVec(l3Mat41 incident_vec, l3Mat41 normal, l3Mat41 r) {
    l3Mat41A tmp;
    l3ScalarMulMat(normal, -2 * l3InnerProductVec(normal, incident_vec, 3), tmp, 3);
    l3SubMat(incident_vec, tmp, r, 3);
}

// rgbもmat31で表すのがいいかも
void l3MultiplyColor(l3RGB* a, l3RGB* b, l3RGB* r) {
    r->r = a->r * b->r / 255;
    r->g = a->g * b->g / 255;
    r->b = a->b * b->b / 255;
}