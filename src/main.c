#include "array.h"
#include "lib3d.h"

void transition(l3Environment* env, int frame) {
    l3Object* obj = array_at(&env->objects, 0);
    obj->theta_y = frame * 5 * 3.14 / 180.0;
}

int main() {
    l3Environment env;
    {
        l3RGB red = {255, 0, 0};
        l3RGB green = {0, 255, 0};
        l3RGB blue = {0, 0, 255};
        l3RGB white = {255, 255, 255};
        l3InitializeEnvironment(&env);
        env.w = 1920;
        env.h = 1080;
        env.outdir = "bin";

        // オブジェクト構築
        l3Object* _object = l3CreateObject();
        int vs[] = {
            l3AddVertexToObject(_object, l3CreateVertex(5, 5, 5, &red)),
            l3AddVertexToObject(_object, l3CreateVertex(-5, -5, 5, &green)),
            l3AddVertexToObject(_object, l3CreateVertex(5, -5, -5, &blue)),
            l3AddVertexToObject(_object, l3CreateVertex(-5, 5, -5, &white)),
        };
        l3Poligon* poligons[] = {
            l3CreatePoligon(0, 2, 1),
            l3CreatePoligon(0, 3, 2),
            l3CreatePoligon(1, 2, 3),
            l3CreatePoligon(0, 1, 3),
        };

        poligons[0]->material = l3PoligonMaterialColor;
        poligons[0]->color.r = 255;

        // テクスチャ読み込み・貼り付け
        l3Texture texture;
        l3Load2DTexture("tex4.ppm", &texture);
        l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
        l3SetTexturePoligon(poligons[1],
                            &texture, texture_vertices);

        l3SetPoligonsToObject(_object,
                              sizeof(poligons) / sizeof(l3Poligon*), poligons);
        l3SetTransposeObject(_object, 15, 15, 15);
        l3AddObjectToEnvironment(&env, _object);

        l3SetCameraInfoToEnvironment(&env, 14, 0, -10, 20, 20, 20, 0, 1, 0);
    }

    l3MultithreadRenderer(&env, transition, 1000, 16);

    l3DestructEnvironment(&env);
}