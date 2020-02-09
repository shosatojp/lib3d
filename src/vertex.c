#include "lib3d.h"

void l3MakeRoundXMat44(vtype theta, mat44 r) {
    r[0] = 1;
    vtype cos_theta = cos(theta),
          sin_theta = sin(theta);
    r[5] = cos_theta;
    r[6] = sin_theta;
    r[9] = -sin_theta;
    r[10] = cos_theta;
    r[15] = 1;
}
void l3MakeRoundYMat44(vtype theta, mat44 r) {
    vtype cos_theta = cos(theta),
          sin_theta = sin(theta);
    r[0] = cos_theta;
    r[2] = -sin_theta;
    r[5] = 1;
    r[8] = sin_theta;
    r[10] = cos_theta;
    r[15] = 1;
}
void l3MakeRoundZMat44(vtype theta, mat44 r) {
    vtype cos_theta = cos(theta),
          sin_theta = sin(theta);
    r[0] = cos_theta;
    r[1] = sin_theta;
    r[4] = -sin_theta;
    r[5] = cos_theta;
    r[10] = 1;
    r[15] = 1;
}
void l3MakeTransposeMat44(vtype dx, vtype dy, vtype dz, mat44 r) {
    r[0] = 1;
    r[5] = 1;
    r[10] = 1;
    r[12] = dx;
    r[13] = dy;
    r[14] = dz;
    r[15] = 1;
}
void l3MakeScaleMat44(vtype sx, vtype sy, vtype sz, mat44 r) {
    r[0] = sx;
    r[5] = sy;
    r[10] = sz;
    r[15] = 1;
}

void l3MakeLocalToWorldMat44(vtype dx, vtype dy, vtype dz,
                             vtype sx, vtype sy, vtype sz,
                             vtype theta_x, vtype theta_y, vtype theta_z, mat44 r) {
    vtype s[16] = {0};
    vtype d[16] = {0};
    vtype rx[16] = {0};
    vtype ry[16] = {0};
    vtype rz[16] = {0};
    l3MakeTransposeMat44(-dx, -dy, -dz, d);
    l3MakeScaleMat44(sx, sy, sz, s);
    l3MakeRoundXMat44(-theta_x, rx);
    l3MakeRoundYMat44(-theta_y, ry);
    l3MakeRoundZMat44(-theta_z, rz);

    vtype* mat44s[] = {rx, ry, rz, d, s};
    l3MulMat44s44(5, (mat44*)mat44s, r);
}

/**
 * camera = target = upper = r
 */
void l3MakeWorldToCameraMat44(mat41 camera, mat41 target, mat41 upper, mat44 r) {
    vtype tmp[4] = {0};
    vtype cx[4] = {0};
    vtype cy[4] = {0};
    vtype cz[4] = {0};
    // cz
    l3SubMat(target, camera, tmp, 3);
    l3NormarizeVec(tmp, cz, 3);
    // cx
    l3CrossProductVec3(cz, upper, tmp);
    l3NormarizeVec(tmp, cx, 3);
    // cy
    l3CrossProductVec3(cx, cz, tmp);
    l3NormarizeVec(tmp, cy, 3);

    r[0] = cx[0];
    r[4] = cx[1];
    r[8] = cx[2];
    r[1] = cy[0];
    r[5] = cy[1];
    r[9] = cy[2];
    r[2] = cz[0];
    r[6] = cz[1];
    r[10] = cz[2];
    r[12] = -l3InnerProductVec(camera, cx, 3);
    r[13] = -l3InnerProductVec(camera, cy, 3);
    r[14] = -l3InnerProductVec(camera, cz, 3);
    r[15] = 1;
}

void l3MakeCameraToProjectionMat44(vtype angle, vtype aspect, vtype near, vtype far, mat44 r) {
    vtype tan_angle_2 = tan(angle / 2);
    r[5] = 1 / tan_angle_2;
    r[0] = r[5] / aspect;
    r[10] = far / (far - near);
    r[11] = 1;
    r[14] = -near * r[10];
}

void l3MakeProjectionToScreenMat44(vtype width, vtype height, mat44 r) {
    vtype width_2 = width / 2.0;
    vtype height_2 = height / 2.0;
    r[0] = width_2;
    r[5] = -height_2;
    r[10] = 1;
    r[12] = width_2;
    r[13] = height_2;
    r[15] = 1;
}

void l3ConvertObject(object* _object, mat44 wcp, mat44 ps, pixel_info* _map, int w, int h) {
    vtype lw[16] = {0};
    l3MakeLocalToWorldMat44(_object->dx, _object->dy, _object->dz,
                            _object->sx, _object->sy, _object->sz,
                            _object->theta_x, _object->theta_y, _object->theta_z, lw);
    vtype lp[16] = {0};
    l3MulMat4444(wcp, lw, lp);

    vtype r[4] = {0},
          r2[4] = {0};
    for (int i = 0; i < _object->poligon_count; i++) {
        poligon* _poligon = _object->poligons[i];
        for (int j = 0; j < l3POLIGON_VERTEX_COUNT; j++) {
            vertex* _vertex = _poligon->vertices[j];
            if (!_vertex->converted) {
                l3InitMat(r, 4, 1);
                // ローカル->ワールド->プロジェクション座標変換
                l3MulMat4441(lp, _vertex->coordinate, r);

                l3DivMat(r, r[3], r, 4);

                l3InitMat(r2, 4, 1);
                // スクリーン座標変換
                l3MulMat4441(ps, r, r2);

                // 結果格納
                memcpy(_vertex->coordinate2d, r2, sizeof(_vertex->coordinate2d));
                _vertex->converted = true;
            }
        }
        // set_max_z(_poligon);
    }
    // sort_by_z(_object->poligon_count, _object->poligons);
    for (int i = 0; i < _object->poligon_count; i++) {
        poligon* _poligon = _object->poligons[i];
        l3WriteRasterMap(_map, w, h, _poligon);
    }
}
