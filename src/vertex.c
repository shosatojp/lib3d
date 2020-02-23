#include "lib3d.h"

l3Mat41A origin = {0, 0, 0, 1};
l3Mat41A e1 = {1, 0, 0, 1};
l3Mat41A e2 = {0, 1, 0, 1};
l3Mat41A e3 = {0, 0, 1, 1};

void l3MakeRoundXMat44(l3Type theta, l3Mat44 r) {
    r[0] = 1;
    l3Type cos_theta = cos(theta),
           sin_theta = sin(theta);
    r[5] = cos_theta;
    r[6] = sin_theta;
    r[9] = -sin_theta;
    r[10] = cos_theta;
    r[15] = 1;
}
void l3MakeRoundYMat44(l3Type theta, l3Mat44 r) {
    l3Type cos_theta = cos(theta),
           sin_theta = sin(theta);
    r[0] = cos_theta;
    r[2] = -sin_theta;
    r[5] = 1;
    r[8] = sin_theta;
    r[10] = cos_theta;
    r[15] = 1;
}
void l3MakeRoundZMat44(l3Type theta, l3Mat44 r) {
    l3Type cos_theta = cos(theta),
           sin_theta = sin(theta);
    r[0] = cos_theta;
    r[1] = sin_theta;
    r[4] = -sin_theta;
    r[5] = cos_theta;
    r[10] = 1;
    r[15] = 1;
}
void l3MakeRoundMat44(l3Type theta_x, l3Type theta_y, l3Type theta_z, l3Mat44 r) {
    l3Mat44A rx = {0};
    l3MakeRoundXMat44(theta_x, rx);
    l3Mat44A ry = {0};
    l3MakeRoundYMat44(theta_y, ry);
    l3Mat44A rz = {0};
    l3MakeRoundZMat44(theta_z, rz);
    l3Mat44 rmats[] = {rx, ry, rz};
    l3MulMat44s44(3, rmats, r);
}
void l3MakeTransposeMat44(l3Type dx, l3Type dy, l3Type dz, l3Mat44 r) {
    r[0] = 1;
    r[5] = 1;
    r[10] = 1;
    r[12] = dx;
    r[13] = dy;
    r[14] = dz;
    r[15] = 1;
}
void l3MakeScaleMat44(l3Type sx, l3Type sy, l3Type sz, l3Mat44 r) {
    r[0] = sx;
    r[5] = sy;
    r[10] = sz;
    r[15] = 1;
}

void l3MakeLocalToWorldMat44(l3Type dx, l3Type dy, l3Type dz,
                             l3Type sx, l3Type sy, l3Type sz,
                             l3Type theta_x, l3Type theta_y, l3Type theta_z, l3Mat44 r) {
    l3Type s[16] = {0};
    l3Type d[16] = {0};
    l3Type rx[16] = {0};
    l3Type ry[16] = {0};
    l3Type rz[16] = {0};
    l3MakeTransposeMat44(dx, dy, dz, d);
    l3MakeScaleMat44(sx, sy, sz, s);
    l3MakeRoundXMat44(-theta_x, rx);
    l3MakeRoundYMat44(-theta_y, ry);
    l3MakeRoundZMat44(-theta_z, rz);

    l3Mat44 mat44s[] = {d, s, rz, ry, rx};
    l3MulMat44s44(5, mat44s, r);
}

/**
 * camera = target = upper = r
 */
void l3MakeWorldToCameraMat44(l3CameraInfo* camerainfo, l3Mat44 r) {
    l3Type tmp[4] = {0},
           cx[4] = {0},
           cy[4] = {0},
           cz[4] = {0};
    // cz
    l3SubMat(camerainfo->target, camerainfo->coordinate, tmp, 3);
    l3NormarizeVec(tmp, cz, 3);
    // cx
    l3CrossProductVec3(cz, camerainfo->upper, tmp);
    l3NormarizeVec(tmp, cx, 3);
    // cy
    l3CrossProductVec3(cx, cz, tmp);
    l3NormarizeVec(tmp, cy, 3);

    // 後から使うから入れとく
    memcpy(&camerainfo->cameraZ, cz, sizeof(l3Type) * 4);

    r[0] = cx[0];
    r[4] = cx[1];
    r[8] = cx[2];
    r[1] = cy[0];
    r[5] = cy[1];
    r[9] = cy[2];
    r[2] = cz[0];
    r[6] = cz[1];
    r[10] = cz[2];
    r[12] = -l3InnerProductVec(camerainfo->coordinate, cx, 3);
    r[13] = -l3InnerProductVec(camerainfo->coordinate, cy, 3);
    r[14] = -l3InnerProductVec(camerainfo->coordinate, cz, 3);
    r[15] = 1;
}

void l3MakeCameraToProjectionMat44(l3Type angle, l3Type aspect, l3Type near, l3Type far, l3Mat44 r) {
    l3Type tan_angle_2 = tan(angle / 2);
    r[5] = 1 / tan_angle_2;
    r[0] = r[5] / aspect;
    r[10] = far / (far - near);
    r[11] = 1;
    r[14] = -near * r[10];
}

void l3MakeProjectionToScreenMat44(l3Type width, l3Type height, l3Mat44 r) {
    l3Type width_2 = width / 2.0;
    l3Type height_2 = height / 2.0;
    r[0] = width_2;
    r[5] = -height_2;
    r[10] = 1;
    r[12] = width_2;
    r[13] = height_2;
    r[15] = 1;
}

void l3AppendPoligonsFromEnvironment(l3Environment* env, l3Mat44 wcps, int w, int h) {
    for (int i = 0; i < (int)env->objects.length; ++i) {
        l3Object* _object = array_at(&env->objects, i);
        l3Type lw[16] = {0};
        l3MakeLocalToWorldMat44(_object->dx, _object->dy, _object->dz,
                                _object->sx, _object->sy, _object->sz,
                                _object->theta_x, _object->theta_y, _object->theta_z, lw);
        l3Type lwcps[16] = {0};
        l3MulMat4444(wcps, lw, lwcps);

        if (!l3FrustumCulling(_object, lwcps, w, h)) {
            continue;
        }

        l3Type r[4] = {0};
        for (int i = 0; i < _object->poligon_count; i++) {
            l3Poligon* _poligon = _object->poligons[i];

            for (int j = 0; j < _poligon->vertex_count; j++) {
                l3Vertex* _vertex = _poligon->vertices[j];
                if (!_vertex->converted) {
                    l3InitMat(r, 4, 1);
                    // ローカル->ワールド->プロジェクション->スクリーン座標変換
                    l3MulMat4441(lwcps, _vertex->coordinate, r);
                    l3DivMat(r, r[3], r, 4);
                    // 結果格納
                    memcpy(_vertex->coordinate2d, r, sizeof(_vertex->coordinate2d));
                    _vertex->converted = true;
                }
            }

            // ポリゴンに対する諸設定はここで済ます
            l3SetMaxZofPoligon(_poligon);
            l3SetOuterRectPoligon(_poligon);
        }
    }
}

/**
 * 視錐台カリング
 */
bool l3FrustumCulling(l3Object* _object, l3Mat44 lwcps, int w, int h) {
    /**
         * オブジェクトが視錐台に入っていなければカリング
         * オブジェクトに当たり判定用球を作る
         * xyは-1~1, zは0~1
         * 
         * (1,0,0)と(0,0,0)の距離でスケールを適用する
         */
    l3Mat41A center = {0};
    l3MulMat4441(lwcps, origin, center);
    l3DivMat(center, center[3], center, 4);
    l3Mat41A center_1 = {0};
    l3MulMat4441(lwcps, e1, center_1);
    l3DivMat(center_1, center_1[3], center_1, 4);
    l3Mat41A sub;
    l3SubMat(center_1, center, sub, 3);
    l3Type scale = l3VecAbs(sub, 3);
    l3Type r = _object->bounding_radius * scale;
    return (-r < center[0] && center[0] < w + r) &&
           (-r < center[1] && center[1] < h + r) &&
           (-r < center[2] && center[2] < 1 + r);
}
