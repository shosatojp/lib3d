#include "lib3d.h"

// void l3GetPoligonPlane2DTextureCoordinate(l3Poligon *poligon, l3Mat21 uv, l3Mat21 r) {
//     // u方向単位ベクトル
//     l3Mat41A e1 = {0};
//     l3Mat41A axis_x = {1, 0, 0, 1};
//     l3CrossProductVec3(poligon->normal, axis_x, e1);
//     l3NormarizeVec3(e1, e1);

//     // v方向単位ベクトル
//     l3Mat41A e2 = {0};
//     l3CrossProductVec3(e1, poligon->normal, e2);

// }

void l3Poligon2DTexturePreProcessTriangle(l3Poligon* poligon, l3Mat44 lw) {
    // テクスチャ用の基底変換行列作成
    l3Mat41A e1_prime, e2_prime;
    l3Mat41A axis_x = {0};
    l3Mat41A axis_x_l = {1, 0, 0, 1};
    switch (poligon->textureCoordinateSystem) {
        case l3CoordinateSystemLocal: {
            l3Mat41A zero_l = {0, 0, 0, 1};
            l3Mat41A zero_w = {0};
            l3MulMat4441(lw, axis_x_l, axis_x);
            l3MulMat4441(lw, zero_l, zero_w);
            l3SubMat3(axis_x, zero_w, axis_x);
            l3NormarizeVec3(axis_x, axis_x);
        } break;
        case l3CoordinateSystemWorld: {
            l3CopyMat(axis_x_l, axis_x, 4);
        } break;
    }
    l3CrossProductVec3(poligon->normal, axis_x, e1_prime);
    l3CrossProductVec3(e1_prime, poligon->normal, e2_prime);
    l3NormarizeVec3(e1_prime, e1_prime);
    l3NormarizeVec3(e2_prime, e2_prime);

    l3Mat31A e1_norm, e2_norm;
    l3NormarizeVec3(poligon->e1, e1_norm);
    l3NormarizeVec3(poligon->e2, e2_norm);

    l3Type tmp[18];
    memcpy(tmp + 0, e1_prime, sizeof(l3Type) * 3);
    memcpy(tmp + 3, e2_prime, sizeof(l3Type) * 3);
    memcpy(tmp + 6, poligon->normal, sizeof(l3Type) * 3);
    memcpy(tmp + 9, e1_norm, sizeof(l3Type) * 3);
    memcpy(tmp + 12, e2_norm, sizeof(l3Type) * 3);
    memcpy(tmp + 15, poligon->normal, sizeof(l3Type) * 3);

    l3SimplificateMat(tmp, 3, 6);

    switch (poligon->textureType) {
        case l3TextureTypeTiled: {
            l3Mat22A p = {tmp[9], tmp[10], tmp[12], tmp[13]};
            // 回転行列
            l3Type cos_theta = cos(poligon->textureRotate),
                   sin_theta = sin(poligon->textureRotate);
            l3Mat22A r = {cos_theta, -sin_theta,
                          sin_theta, cos_theta};
            // スケール行列
            l3Mat22A s = {poligon->texture->w * poligon->textureRepeatX, 0,
                          0, poligon->texture->h * poligon->textureRepeatY};

            l3Mat22A transform = {0};
            l3MulMat(s, r, transform, 2, 2, 2);
            poligon->texturePuv = malloc(sizeof(l3Type) * 4);
            l3MulMat(transform, p, poligon->texturePuv, 2, 2, 2);
        } break;
        case l3TextureTypeUV: {
            l3Mat33 p = &tmp[9];
            l3Mat33A uvsrc = {
                0, 0, 1,
                1, 0, 1,
                0, 1, 1};
            l3Mat33A src_tmp = {0};
            l3MulMat(p, uvsrc, src_tmp, 3, 3, 3);
            l3Mat31 dst[] = {&src_tmp[0],
                             &src_tmp[3],
                             &src_tmp[6]};
            l3Mat21 src[] = {
                &poligon->textureVertices[0],
                &poligon->textureVertices[2],
                &poligon->textureVertices[4]};
            l3Mat33A affinemat = {0};
            l3GetAffineTransformMat33(src, dst, affinemat);
            // l3PrintMat(affinemat, 3, 3);
            // {
            //     l3Mat33A dst = {
            //         src_tmp[0], src_tmp[1], 1,
            //         src_tmp[3], src_tmp[4], 1,
            //         src_tmp[6], src_tmp[7], 1};
            //     l3Mat33A src = {
            //         poligon->textureVertices[0], poligon->textureVertices[1], 1,
            //         poligon->textureVertices[2], poligon->textureVertices[3], 1,
            //         poligon->textureVertices[4], poligon->textureVertices[5], 1};
            //     l3Type tmp[18];
            //     memcpy(tmp + 0, &src[0], sizeof(l3Type) * 3);
            //     memcpy(tmp + 3, &src[3], sizeof(l3Type) * 3);
            //     memcpy(tmp + 6, &src[6], sizeof(l3Type) * 3);
            //     memcpy(tmp + 9, &dst[0], sizeof(l3Type) * 3);
            //     memcpy(tmp + 12, &dst[3], sizeof(l3Type) * 3);
            //     memcpy(tmp + 15, &dst[6], sizeof(l3Type) * 3);
            //     l3SimplificateMat(tmp, 3, 6);
            //     l3PrintMat(&tmp[9], 3, 3);
            // }
            l3Mat33A affinematinv = {0};
            l3InverseMat(3, affinemat, affinematinv);
            poligon->textureAffineMatInv = (l3Mat33)calloc(sizeof(l3Type), 9);
            l3MulMat(affinematinv, p, poligon->textureAffineMatInv, 3, 3, 3);
        } break;
        default:
            break;
    }
}