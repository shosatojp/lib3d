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
    l3Mat41A a_v0 = {0};
    l3SubMat(ray_origin, poligon->vertices[0]->coordinateWorld, a_v0, 3);

    // クラメールの公式利用
    l3Mat31A cross_prod_r_d;
    l3CrossProductVec3(a_v0, ray_direction, cross_prod_r_d);
    l3Type e = -l3InnerProductVec(ray_direction, poligon->cross_prod_e1_e2, 3);
    l3Type t = l3InnerProductVec(a_v0, poligon->cross_prod_e1_e2, 3) / e;
    l3Type u = l3InnerProductVec(poligon->e2, cross_prod_r_d, 3) / e;
    l3Mat31A minus_cross_prod_r_d;
    l3ScalarMulMat(cross_prod_r_d, -1, minus_cross_prod_r_d, 3);
    l3Type v = l3InnerProductVec(poligon->e1, minus_cross_prod_r_d, 3) / e;

    // 簡約化で解く
    // l3Type tmp[12] = {0};
    // tmp[0] = -ray_direction[0];
    // tmp[1] = -ray_direction[1];
    // tmp[2] = -ray_direction[2];
    // memcpy(&tmp[3], poligon->e1, sizeof(l3Type) * 3);
    // memcpy(&tmp[6], poligon->e2, sizeof(l3Type) * 3);
    // memcpy(&tmp[9], a_v0, sizeof(l3Type) * 3);

    // l3SimplificateMat(tmp, 3, 4);

    // l3Type t = tmp[9];
    // l3Type u = tmp[10];
    // l3Type v = tmp[11];

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
    l3AddMat(incident_vec, tmp, r, 3);
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
void l3MakeWorldToCameraBasisChangeMat33(l3CameraInfo* camerainfo, l3Mat33 r) {
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
 * Ray始点での方向ベクトル
 */
void l3GetRayStartPointAndDirection(l3Mat33 p_world_to_camera, l3Mat31 camera_pos, l3Type near,
                                    l3Type w, l3Type h,
                                    l3Type u, l3Type v, l3Mat31 start_point, l3Mat31 direction) {
    l3Mat31A ray_pos_c = {(2 * u - w) / h,
                          1 - 2 * v / h,
                          near};
    l3Mat31A ray_pos_w = {0};
    l3MulMat(p_world_to_camera, ray_pos_c, ray_pos_w, 3, 3, 1);
    l3NormarizeVec(ray_pos_w, direction, 3);

    l3AddMat(ray_pos_w, camera_pos, start_point, 3);
}

/**
 * Rayの着地点を見つける
 */
bool l3FindRayCrossPoint(l3Ray* ray, l3Environment* env) {
    l3Mat41A min_intersection = {0};
    l3Mat21A min_uv = {0};
    l3Poligon* min_poligon = NULL;
    l3Type min_length = 0;
    bool found = false;

    for (size_t i = 0; i < env->poligons.length; i++) {
        l3Poligon* p = array_at(&env->poligons, i);
        switch (p->poligonType) {
            case l3PoligonTypeTriangle: {
                l3Mat41A intersection;
                l3Mat21A uv;
                if (!l3IntersectRayPoligon(ray->rayStartPoint, ray->rayDirection, p, intersection, uv)) {
                    break;
                }
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
                if (!l3IntersectRaySphere(ray->rayStartPoint, ray->rayDirection,
                                          p->vertices[0]->coordinateWorld,
                                          p->sphere_radius, intersection)) {
                    break;
                }
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

    if (found) {
        memcpy(ray->uv, min_uv, sizeof(l3Type) * 2);
        memcpy(ray->intersection, min_intersection, sizeof(l3Type) * 3);
        ray->poligon = min_poligon;
    }
    return found;
}

void l3GetNormal(l3Poligon* poligon, l3Mat41 intersection, l3Mat41 normal) {
    switch (poligon->poligonType) {
        case l3PoligonTypeTriangle:
            l3NormarizeVec(poligon->cross_prod_e1_e2, normal, 3);
            break;
        case l3PoligonTypeShpere:
            break;
        default:
            break;
    }
}

/**
 * 始点と方向が決まっているRayから、
 * 交点を見つけ、
 * その地点の色を求める
 * 交点がなかった場合にはそこで再帰終了、MAXDEPTHを超えた場合にも
 * Depthの定義：たどる交点の数
 */
bool l3TraceRay(l3Ray* ray, l3Environment* env, int depth) {
    if (l3FindRayCrossPoint(ray, env)) {
        // l3RGB sumcolor = {100, 100, 100};
        l3RGB sumcolor = {ray->poligon->color.r,
                          ray->poligon->color.g,
                          ray->poligon->color.b};
        l3Mat41A normal = {0};
        l3GetNormal(ray->poligon, ray->intersection, normal);

        // 環境光
        l3RGB envcolor = {255, 255, 255};
        l3Type brightness = 60;
        sumcolor.r += 0.21 * brightness;
        sumcolor.g += 0.72 * brightness;
        sumcolor.b += 0.07 * brightness;

        // 拡散反射Ray
        l3Ray diffuse = {0};
        l3Mat31A light = {6, 20, 30};
        l3NormarizeVec(light, light, 3);
        l3Type l_d = max(min(1, l3InnerProductVec(normal, light, 3)), 0.3);
        l3ScalarMulColor(sumcolor, l_d);

        if (depth <= l3RAY_TRACE_MAX_DEPTH) {

            // 鏡面反射Ray
            l3Ray specular = {0};
            l3GetReflectedVec(ray->rayDirection, normal, specular.rayDirection);
            l3AddMat(ray->intersection, specular.rayDirection, specular.rayStartPoint, 3);
            if (l3TraceRay(&specular, env, depth + 1)) {
                l3MulColor(sumcolor, specular.color);
            }
        }
        // l3SummarizeColor();
        ray->color.r = sumcolor.r;
        ray->color.g = sumcolor.g;
        ray->color.b = sumcolor.b;
        return true;
    } else {
        return false;
    }
}

void l3SetWorldCoordinate(l3Environment* env) {
    for (int i = 0; i < env->objects.length; ++i) {
        l3Object* _object = array_at(&env->objects, i);
        l3Type lw[16] = {0};
        l3MakeLocalToWorldMat44(_object->dx, _object->dy, _object->dz,
                                _object->sx, _object->sy, _object->sz,
                                _object->theta_x, _object->theta_y, _object->theta_z, lw);
        for (int i = 0; i < _object->poligon_count; i++) {
            l3Poligon* _poligon = _object->poligons[i];

            for (int j = 0; j < l3POLIGON_VERTEX_COUNT; j++) {
                l3Vertex* _vertex = _poligon->vertices[j];
                if (!_vertex->converted) {
                    // ローカル->ワールド
                    l3InitMat(_vertex->coordinateWorld, 4, 1);
                    l3MulMat4441(lw, _vertex->coordinate, _vertex->coordinateWorld);
                    _vertex->converted = true;
                }
            }

            // 前処理
            l3SubMat(_poligon->vertices[1]->coordinateWorld, _poligon->vertices[0]->coordinateWorld, _poligon->e1, 3);
            l3SubMat(_poligon->vertices[2]->coordinateWorld, _poligon->vertices[0]->coordinateWorld, _poligon->e2, 3);
            l3CrossProductVec3(_poligon->e1, _poligon->e2, _poligon->cross_prod_e1_e2);
        }
    }
}