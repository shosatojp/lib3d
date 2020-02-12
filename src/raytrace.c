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
bool l3IntersectRayPoligon(l3Mat31 ray_origin, l3Mat31 ray_direction, l3Poligon* poligon, l3Mat31 r, l3Mat21 uv) {
    l3Mat31A e1 = {0};
    l3Mat31A e2 = {0};
    l3Mat31A a_v0 = {0};
    l3SubMat(&poligon->vertices[1]->coordinateWorld, &poligon->vertices[0]->coordinateWorld, e1, 3);
    l3SubMat(&poligon->vertices[2]->coordinateWorld, &poligon->vertices[0]->coordinateWorld, e2, 3);
    l3SubMat(ray_origin, &poligon->vertices[0]->coordinateWorld, a_v0, 3);

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
void l3PoligonNormalWorld(l3Poligon* poligon, l3Mat31 normal) {
    l3Mat41A e1 = {0};
    l3Mat41A e2 = {0};
    l3Mat41A prod = {0};
    l3SubMat(&poligon->vertices[2]->coordinateWorld, &poligon->vertices[0]->coordinateWorld, e1, 3);
    l3SubMat(&poligon->vertices[1]->coordinateWorld, &poligon->vertices[0]->coordinateWorld, e2, 3);

    l3CrossProductVec3(e2, e1, prod);
    l3NormarizeVec(prod, normal, 3);
}

/**
 * レイトレーシングでポリゴンの点(u,v)のテクスチャ座標
 * poligon: 対象のポリゴン
 * uv: ポリゴン内のuv座標
 * (result) r: テクスチャ内でのxy座標
 */
void l3GetRayPoligon2DTextureCoordinate(l3Poligon* poligon, l3Mat21 uv, l3Mat21 r) {
    l3Mat41A e1 = {0};
    l3Mat41A e2 = {0};
    l3SubMat(&poligon->textureVertices[4], &poligon->textureVertices[0], e1, 3);
    l3SubMat(&poligon->textureVertices[2], &poligon->textureVertices[0], e2, 3);
    r[0] = poligon->textureVertices[0] + uv[0] * e1[0] + uv[1] * e2[0];
    r[1] = poligon->textureVertices[1] + uv[0] * e1[1] + uv[1] * e2[1];
}