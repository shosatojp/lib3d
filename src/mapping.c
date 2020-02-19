#include "lib3d.h"

void l3GetPoligonPlane2DTextureCoordinate(l3Poligon *poligon, l3Mat21 uv, l3Mat21 r) {
    // u方向単位ベクトル
    l3Mat41A e1 = {0};
    l3Mat41A axis_x = {1, 0, 0, 1};
    l3CrossProductVec3(poligon->normal, axis_x, e1);
    l3NormarizeVec3(e1, e1);

    // v方向単位ベクトル
    l3Mat41A e2 = {0};
    l3CrossProductVec3(e1, poligon->normal, e2);
    
}