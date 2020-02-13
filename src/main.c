#include "array.h"
#include "lib3d.h"

void transition(l3Environment* env, int frame) {
    l3Object* obj = array_at(&env->objects, 0);
    obj->theta_y = frame * 5 * 3.14 / 180.0;
    obj->theta_x = frame * 5 * 3.14 / 180.0;
    l3Object* obj1 = array_at(&env->objects, 1);
    obj1->theta_y = frame * 5 * 3.14 / 180.0;

    l3Object* obj2 = array_at(&env->objects, 2);
    obj2->theta_z = frame * 5 * 3.14 / 180.0;
    obj2->dx = frame;
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
        env.outdir = "/mnt/f/hoge";

        // オブジェクト構築
        l3Object* _object = l3CreateObject();
        int vs[] = {
            l3AddVertexToObject(_object, l3CreateVertex(5, 5, 5, &red)),
            l3AddVertexToObject(_object, l3CreateVertex(-5, -5, 5, &green)),
            l3AddVertexToObject(_object, l3CreateVertex(5, -5, -5, &blue)),
            l3AddVertexToObject(_object, l3CreateVertex(-5, 5, -5, &white)),
        };
        // 右回りが表、数字はオブジェクトごとの頂点のインデックス
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
        l3Load2DTexture("assets/tex4.ppm", &texture);
        l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
        l3SetPoligonsToObject(_object, sizeof(poligons) / sizeof(l3Poligon*), poligons);
        l3SetTransposeObject(_object, 15, 15, 15);
        l3AddObjectToEnvironment(&env, _object);

        l3Object* o2 = l3CloneObject(_object);
        l3Object* o3 = l3CloneObject(_object);
        _object->poligons[0]->transparency = 0.5;
        _object->poligons[1]->transparency = 0.5;
        _object->poligons[2]->transparency = 0.5;
        _object->poligons[3]->transparency = 0.5;
        l3SetTransposeObject(o2, 15, 30, 50);
        o2->poligons[0]->material = l3PoligonMaterialVertex;
        o2->poligons[1]->material = l3PoligonMaterialVertex;
        l3SetTexturePoligon(o2->poligons[1],
                            &texture, texture_vertices);
        l3AddObjectToEnvironment(&env, o2);

        l3SetTransposeObject(o3, 40, 20, 30);
        l3AddObjectToEnvironment(&env, o3);

        l3SetCameraInfoToEnvironment(&env, 14, 0, -10,
                                     20, 20, 20,
                                     0, 1, 0,
                                     120, 10, 100);
        // {
        //     // RayTracingDebug
        //     l3SolvePtrsEnvironment(&env);
        //     l3Mat44A lw = {0};
        //     l3MakeLocalToWorldMat44(15, 15, 15, 1, 1, 1, 0, 0, 0, lw);
        //     for (int i = 0; i < l3POLIGON_VERTEX_COUNT; i++) {
        //         l3MulMat4441(lw, poligons[0]->vertices[i]->coordinate,
        //                      poligons[0]->vertices[i]->coordinateWorld);
        //     }
        //     l3Mat41A r = {0};
        //     l3Mat21A uv = {0};
        //     l3Mat41A ray_origin = {0, 0, 0};
        //     l3Mat41A sphere_center = {20, 20, 20};
        //     l3Mat41A ray_direction = {20, 15, 20};
        //     l3NormarizeVec(ray_direction, ray_direction, 3);
        //     // l3Mat41A ray_direction = {20, 15, 20};
        //     bool result = l3IntersectRayPoligon(ray_origin, ray_direction, poligons[0], r, uv);
        //     bool result2 = l3IntersectRaySphere(ray_origin, ray_direction, sphere_center, 5, r);
        //     printf("%d %d\n", result, result2);
        //     l3Mat41A norm = {0};
        //     l3GetPoligonNormal(poligons[0], norm);
        //     l3PrintMat(norm, 3, 1);
        //     exit(0);
        // }
    }

    l3MultithreadRenderer(&env, transition, 30, 8);

    l3DestructEnvironment(&env);
}