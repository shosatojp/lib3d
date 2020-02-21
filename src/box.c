#include "lib3d.h"

l3Object* l3CreateBox(){
    l3Object* box = l3CreateObject();
    l3RGB white = {255, 255, 255};
    int vs[] = {
        l3AddVertexToObject(box, l3CreateVertex(-0.5, 0.5, -0.5, &white)),
        l3AddVertexToObject(box, l3CreateVertex(-0.5, 0.5, 0.5, &white)),
        l3AddVertexToObject(box, l3CreateVertex(0.5, 0.5, 0.5, &white)),
        l3AddVertexToObject(box, l3CreateVertex(0.5, 0.5, -0.5, &white)),
        l3AddVertexToObject(box, l3CreateVertex(-0.5, -0.5, -0.5, &white)),
        l3AddVertexToObject(box, l3CreateVertex(-0.5, -0.5, 0.5, &white)),
        l3AddVertexToObject(box, l3CreateVertex(0.5, -0.5, 0.5, &white)),
        l3AddVertexToObject(box, l3CreateVertex(0.5, -0.5, -0.5, &white)),
    };

    l3Poligon* poligons[] = {
                l3CreatePoligon(0, 2, 1),
                l3CreatePoligon(2, 0, 3),
                l3CreatePoligon(1, 6, 5),
                l3CreatePoligon(6, 1, 2),
                l3CreatePoligon(2, 7, 6),
                l3CreatePoligon(7, 2, 3),
                l3CreatePoligon(3, 4, 7),
                l3CreatePoligon(4, 3, 0),
                l3CreatePoligon(0, 5, 4),
                l3CreatePoligon(5, 0, 1),
                l3CreatePoligon(5, 7, 4),
                l3CreatePoligon(7, 5, 6),
            };

    poligons[0]->material = l3PoligonMaterialColor;
    poligons[0]->color.r = 255;
    poligons[0]->color.g = 255;
    poligons[0]->color.b = 255;
    poligons[1]->material = l3PoligonMaterialColor;
    poligons[1]->color.r = 255;
    poligons[1]->color.g = 255;
    poligons[1]->color.b = 255;
    poligons[2]->material = l3PoligonMaterialColor;
    poligons[2]->color.r = 55;
    poligons[2]->color.g = 255;
    poligons[2]->color.b = 255;
    poligons[3]->material = l3PoligonMaterialColor;
    poligons[3]->color.r = 55;
    poligons[3]->color.g = 255;
    poligons[3]->color.b = 255;
    poligons[4]->material = l3PoligonMaterialColor;
    poligons[4]->color.r = 255;
    poligons[4]->color.g = 55;
    poligons[4]->color.b = 255;
    poligons[5]->material = l3PoligonMaterialColor;
    poligons[5]->color.r = 255;
    poligons[5]->color.g = 55;
    poligons[5]->color.b = 255;
    poligons[6]->material = l3PoligonMaterialColor;
    poligons[6]->color.r = 255;
    poligons[6]->color.g = 255;
    poligons[6]->color.b = 55;
    poligons[7]->material = l3PoligonMaterialColor;
    poligons[7]->color.r = 255;
    poligons[7]->color.g = 255;
    poligons[7]->color.b = 55;
    poligons[8]->material = l3PoligonMaterialColor;
    poligons[8]->color.r = 0;
    poligons[8]->color.g = 0;
    poligons[8]->color.b = 255;
    poligons[9]->material = l3PoligonMaterialColor;
    poligons[9]->color.r = 0;
    poligons[9]->color.g = 0;
    poligons[9]->color.b = 255;
    poligons[10]->material = l3PoligonMaterialColor;
    poligons[10]->color.r = 255;
    poligons[10]->color.g = 0;
    poligons[10]->color.b = 0;
    poligons[11]->material = l3PoligonMaterialColor;
    poligons[11]->color.r = 255;
    poligons[11]->color.g = 0;
    poligons[11]->color.b = 0;
    
    l3SetPoligonsToObject(box, sizeof(poligons) / sizeof(l3Poligon*), poligons);
    return box;
}