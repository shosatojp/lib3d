#include "lib3d.h"

void l3ClearRasterMap(l3PixelInfo* map, int w, int h) {
    memset(map, 0, w * h * sizeof(l3PixelInfo));
}

/**
 * freeする必要あり
 */
l3PixelInfo* l3CreateRasterMap(int w, int h) {
    return (l3PixelInfo*)calloc(sizeof(l3PixelInfo), w * h);
}

/**
 * 背面カリング
 * cameraはカメラのZ軸方向単位ベクトル
 */
bool l3BackCulling(l3Poligon* _poligon, l3Mat41 camera) {
    // 表裏はプロジェクション座標空間で求めるべきだがスクリーン座標空間でもできる
    l3Mat41A ab = {0};
    l3SubMat(_poligon->vertices[1]->coordinate2d, _poligon->vertices[0]->coordinate2d, ab, 3);
    l3Mat41A bc = {0};
    l3SubMat(_poligon->vertices[2]->coordinate2d, _poligon->vertices[1]->coordinate2d, bc, 3);
    l3Mat41A norm = {0};
    l3CrossProductVec3(ab, bc, norm);
    l3Type ip = l3InnerProductVec(norm, camera, 3);
    return ip > 0;
}

void l3WriteRasterMap(l3Environment* env, int w, int h, l3Poligon* _poligon) {
    if (l3BackCulling(_poligon, &env->camera.cameraZ)) {
        return;
    }
    if (_poligon->material == l3PoligonMaterialTexture) {
        // 変換行列の逆行列を生成
        l3SetTextureMatInv(_poligon);
    }
    for (int i = max(0, _poligon->min[0]); i < min(w, _poligon->max[0]); ++i) {
        for (int j = max(0, _poligon->min[1]); j < min(h, _poligon->max[1]); ++j) {
            l3Mat31A v = {i, j, 1};
            if (l3InsideOfPoligon2D(l3POLIGON_VERTEX_COUNT, _poligon->vertices, v)) {
                l3PixelInfo* p = &l3RasterMapAt(env->map, w, h, i, j);
                p->_poligon = _poligon;
                p->activated = true;
                l3FragmentShader(p, v);
            }
        }
    }
}

void l3ConvertRasterMapToBuffer(l3PixelInfo* map, unsigned char* buf, int w, int h) {
    for (int i = 0, len = w * h; i < len; i++) {
        if (map[i].activated) {
            buf[i * 3] = (unsigned char)map[i].color.r;
            buf[i * 3 + 1] = (unsigned char)map[i].color.g;
            buf[i * 3 + 2] = (unsigned char)map[i].color.b;
        }
    }
}

void l3SetMaxZofPoligon(l3Poligon* _poligon) {
    _poligon->max_z = max(max(_poligon->vertices[0]->coordinate2d[2],
                              _poligon->vertices[1]->coordinate2d[2]),
                          _poligon->vertices[2]->coordinate2d[2]);
}

int l3ComparePoligons(const void* p, const void* q) {
    return -((l3Poligon*)p)->max_z + ((l3Poligon*)q)->max_z;
}

void l3SortPoligonsByMaxZ(int c, l3Poligon* _poligons[]) {
    qsort(_poligons, c, sizeof(l3Poligon*), l3ComparePoligons);
}