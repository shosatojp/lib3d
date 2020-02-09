#include "lib3d.h"
/**
 * フラグメントシェーダ
 */
void l3FragmentShader(pixel_info* p, mat21 v) {
    poligon* _poligon = p->_poligon;

    vtype area_0 = l3GetTriangleArea(v, _poligon->vertices[1]->coordinate2d, _poligon->vertices[2]->coordinate2d);
    vtype area_1 = l3GetTriangleArea(v, _poligon->vertices[2]->coordinate2d, _poligon->vertices[0]->coordinate2d);
    vtype area_2 = l3GetTriangleArea(v, _poligon->vertices[0]->coordinate2d, _poligon->vertices[1]->coordinate2d);
    vtype area_sum = area_0 + area_1 + area_2;
    p->color.r = area_0 / area_sum * _poligon->vertices[0]->color.r +
                 area_1 / area_sum * _poligon->vertices[1]->color.r +
                 area_2 / area_sum * _poligon->vertices[2]->color.r;
    p->color.g = area_0 / area_sum * _poligon->vertices[0]->color.g +
                 area_1 / area_sum * _poligon->vertices[1]->color.g +
                 area_2 / area_sum * _poligon->vertices[2]->color.g;
    p->color.b = area_0 / area_sum * _poligon->vertices[0]->color.b +
                 area_1 / area_sum * _poligon->vertices[1]->color.b +
                 area_2 / area_sum * _poligon->vertices[2]->color.b;
}