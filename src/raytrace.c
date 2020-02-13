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

/**
 * ワールド座標空間からカメラ座標空間への基底変換行列
 */
void l3MakeWorldToCameraBasisChangeMat44(l3CameraInfo* camerainfo, l3Mat33 r) {
    l3Mat41A tmp = {0};
    l3Mat41A cx = {0};
    l3Mat41A cy = {0};
    l3Mat41A cz = {0};

    // cz
    l3SubMat(camerainfo->target, camerainfo->coordinate, tmp, 3);
    l3NormarizeVec(tmp, cz, 3);
    // cx
    l3CrossProductVec3(cz, camerainfo->upper, tmp);
    l3NormarizeVec(tmp, cx, 3);
    // cy
    l3CrossProductVec3(cx, cz, tmp);
    l3NormarizeVec(tmp, cy, 3);

    memcpy(&r[0], cx, sizeof(l3Type) * 3);
    memcpy(&r[3], cy, sizeof(l3Type) * 3);
    memcpy(&r[6], cz, sizeof(l3Type) * 3);
}

/**
 * Rayの始点
 */
void l3GetRayStartPoint(l3Mat33 p_world_to_camera, l3Type angle, l3Type w, l3Type h, l3Type u, l3Type v, l3Mat31 r) {
    l3Mat31A ray_dir = {(2 * u - w) / h,
                        1 - 2 * v / h,
                        1 / tan(angle / 2)};
    l3MulMat(p_world_to_camera, ray_dir, r, 3, 3, 1);
}

/**
 * Ray始点での方向ベクトル
 */
void l3GetRayDirection(l3Mat31 ray_start_point, l3Mat31 camera_pos, l3Mat31 r) {
    l3Mat31A tmp;
    l3SubMat(ray_start_point, camera_pos, tmp, 3);
    l3NormarizeVec(tmp, tmp, 3);
}

/**
 * Rayの着地点を見つける
 */
bool l3FindRayCrossPoint(l3Ray* ray, int poligon_count, l3Poligon* poligons[]) {
    l3Mat41A min_intersection = {0};
    l3Mat21A min_uv = {0};
    l3Poligon* min_poligon = NULL;
    bool found = false;

    for (size_t i = 0; i < poligon_count; i++) {
        l3Poligon* p = poligons[i];
        switch (p->poligonType) {
            case l3PoligonTypeTriangle: {
                l3Mat41A intersection;
                l3Mat21A uv;
                l3IntersectRayPoligon(ray->rayStartPoint, ray->rayDirection, p, intersection, uv);
                l3Type distance = l3DistanceVec(ray->rayStartPoint, intersection);
                if (!found || min_length > distance) {
                    memcpy(min_intersection, intersection, sizeof(l3Type) * 4);
                    memcpy(min_uv, uv, sizeof(l3Type) * 2);
                    min_length = distance;
                    min_poligon = p;
                    found = true;
                }
            } break;
            case l3PoligonTypeShpere: {
                l3Mat41A intersection;
                l3IntersectRaySphere(ray->rayStartPoint, ray->rayDirection,
                                     p->vertices[0]->coordinateWorld,
                                     p->sphere_radius, intersection);
                l3Type distance = l3DistanceVec(ray->rayStartPoint, intersection);
                if (!found || min_length > distance) {
                    memcpy(min_intersection, intersection, sizeof(l3Type) * 4);
                    min_length = distance;
                    min_poligon = p;
                    found = true;
                }
            } break;
            default:
                break;
        }
    }

    memcpy(ray->uv, min_intersection, sizeof(l3Type) * 4);
    memcpy(ray->intersection, min_uv, sizeof(l3Type) * 2);
    ray->poligon = min_poligon;
    return found;
}

// void l3TraceRay()

void l3RayIrradiationer(l3Environment* env) {
    l3Mat33A p_wtoc = {0};
    l3MakeWorldToCameraBasisChangeMat44(env->camera, p_wtoc);

    // ポリゴンの配列を作る
    array* poligons = array_new(sizeof(l3Poligon*), true, 100);

    for (size_t j = 0; j < env->h; j++) {
        for (size_t i = 0; i < env->w; i++) {
            l3Ray ray;
            memset(&ray, 0, sizeof(l3Ray));
            l3GetRayStartPoint(p_wtoc, env->camera.angle, env->w, env->h, i, j, ray.ray_start_point);
            l3GetRayDirection(ray.ray_start_point, env->camera.coordinate, ray.ray_direction);

            // Rayの交差点を見つける
            l3FindRayCrossPoint(&ray, poligons->length, poligons->data);
            
            // 再帰的にRayをたどる

            // バッファ(i,j)に出力
        }
    }

    array_clear(poligons);
    free(poligons);
}