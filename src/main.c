#include "array.h"
#include "lib3d.h"

void transition(l3Environment* env, int frame) {
    l3Object* obj = array_at(&env->objects, 0);
    obj->theta_y = frame * 5 * 3.14 / 180.0;
    // obj->theta_x = frame * 5 * 3.14 / 180.0;
    // l3Object* obj2 = array_at(&env->objects, 1);
    // obj2->theta_z = frame * 5 * 3.14 / 180.0;
}

int main(int argc, const char* argv[]) {
    // default options
    l3Options options;
    options.outdir = "bin";
    options.h = 1080;
    options.w = 1920;
    options.frames = 10;
    options.threads = 1;
    options.renderer = l3RaytracingRenderer;
    if (!l3ParseArgs(argc, argv, &options)) exit(0);

    l3Environment env;
    {
        l3RGB red = {255, 0, 0};
        l3RGB green = {0, 255, 0};
        l3RGB blue = {0, 0, 255};
        l3RGB white = {255, 255, 255};
        l3InitializeEnvironment(&env);
        env.w = options.w;
        env.h = options.h;
        env.outdir = options.outdir;

        // オブジェクト構築
        l3Object* _object = l3CreateObject();
        {
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
            poligons[0]->color.r = 150;
            poligons[1]->material = l3PoligonMaterialColor;
            poligons[1]->color.g = 150;
            poligons[2]->material = l3PoligonMaterialColor;
            poligons[2]->color.b = 150;
            poligons[3]->material = l3PoligonMaterialColor;
            poligons[3]->color.r = 150;
            poligons[3]->color.b = 150;

            // テクスチャ読み込み・貼り付け
            // l3Texture texture;
            // l3Load2DTexture("assets/tex4.ppm", &texture);
            // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
            l3SetPoligonsToObject(_object, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(_object, -5, 0, 0);
            l3SetScaleObject(_object, 2, 2, 2);
            l3AddObjectToEnvironment(&env, _object);
        }
        l3Object* obj2 = l3CloneObject(_object);
        {
            l3SetTransposeObject(obj2, 20, 0, 0);
            // l3AddObjectToEnvironment(&env, obj2);
        }
        l3Object* obj3 = l3CloneObject(_object);
        {
            l3SetTransposeObject(obj3, 0, 10, 0);
            // l3AddObjectToEnvironment(&env, obj3);
        }

        l3Object* sphere = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(sphere, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(sphere, l3CreateVertex(0, 5, 0, &red)),
            };
            l3Poligon* poligons[] = {
                l3CreatePoligonSphere(0, 1),
            };
            poligons[0]->sphere_radius = 10;
            poligons[0]->color.r = 255;
            poligons[0]->color.g = 55;
            poligons[0]->color.b = 55;
            poligons[0]->material = l3PoligonMaterialColor;
            l3SetPoligonsToObject(sphere, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(sphere, 15, 0, 0);
            l3AddObjectToEnvironment(&env, sphere);
        }

        l3SetCameraInfoToEnvironment(&env, 0, 0, -40,
                                     0, 0, 0,
                                     0, 1, 0,
                                     radians(50), 2, 100);
    }

    l3MultithreadRenderer(&env, options.renderer, transition, options.frames, options.threads);
    // l3MultithreadRenderer(&env, l3RasterizingRenderer, transition, 100, 16);

    l3DestructEnvironment(&env);
}
