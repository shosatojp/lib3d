#include "array.h"
#include "lib3d.h"

static void transition(l3Environment* env, int frame) {
    l3Object* sphere = array_at(&env->objects, 1);
    // { // イージング処理集
    // if(frame <= 30){sphere->dx = 90- 180 * l3TimeTransition(l3TimeType_linear,frame,0,30);}
    // if(frame >=60 && frame <= 90){sphere->dx = 90- 180 * l3TimeTransition(l3TimeType_EasyEaseIn,frame,60,90);}
    // if(frame >=120 && frame <= 150){sphere->dx = 90- 180 * l3TimeTransition(l3TimeType_EasyEaseOut,frame,120,150);}
    // if(frame >=180 && frame <= 210){sphere->dx = 90- 180 * l3TimeTransition(l3TimeType_EasyEase,frame,180,210);}
    //
    // sphere->dy = 40;
    // }

    {  //bezier曲線移動サンプル
        bezier cpoint[8];
        {
            cpoint[0].x = 0;
            cpoint[0].y = 5;
            cpoint[0].z = -70;
            cpoint[1].x = 80;
            cpoint[1].y = 100;
            cpoint[1].z = 0;
            cpoint[2].x = 0;
            cpoint[2].y = 10;
            cpoint[2].z = -100;
            cpoint[3].x = -70;
            cpoint[3].y = 20;
            cpoint[3].z = 40;
            cpoint[4].x = 0;
            cpoint[4].y = 5;
            cpoint[4].z = -70;
            cpoint[5].x = -100;
            cpoint[5].y = 40;
            cpoint[5].z = 70;
            cpoint[6].x = -80;
            cpoint[6].y = 100;
            cpoint[6].z = -30;
            cpoint[7].x = -100;
            cpoint[7].y = 5;
            cpoint[7].z = 70;
        }

        bezier sphere_bezier = l3GetBezierCurve(l3TimeTransition(l3TimeType_EasyEase, frame, 30, 200), 8, cpoint);

        sphere->dx = sphere_bezier.x;
        sphere->dy = sphere_bezier.y;
        sphere->dz = sphere_bezier.z;
    }

    l3Object* obj = array_at(&env->objects, 0);
    obj->theta_y += 1 / 100.0 * 2 * PI;
    obj->theta_x += 1 / 100.0 * 2 * PI;
    obj->theta_z += 10 / 100.0 * 2 * PI;
    // l3Object* obj2 = array_at(&env->objects, 1);

    //sphere->dy = 50 * fabs(sin(1.0 * frame / 5.0)); //ぴょんぴょん

    {  //運動テスト用固定カメラ
        env->camera.coordinate[0] = 0;
        env->camera.coordinate[1] = 0;
        env->camera.coordinate[2] = -100;
        env->camera.target[1] = 20;
    }
}

int scene_bezier(int argc, const char* argv[],l3Options* options) {
    l3Environment env;
    {
        l3RGB red = {255, 0, 0};
        l3RGB green = {0, 255, 0};
        l3RGB blue = {0, 0, 255};
        l3RGB white = {255, 255, 255};
        l3InitializeEnvironment(&env);
        env.w = options->w;
        env.h = options->h;
        env.outdir = options->outdir;

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
            // poligons[0]->transparency = 0.1;
            // poligons[1]->transparency = 0.1;
            // poligons[2]->transparency = 0.1;
            // poligons[3]->transparency = 0.1;
            poligons[0]->metalness[0] = poligons[1]->metalness[0] = poligons[2]->metalness[0] = poligons[3]->metalness[0] = 0.05;
            poligons[0]->metalness[1] = poligons[1]->metalness[1] = poligons[2]->metalness[1] = poligons[3]->metalness[1] = 0.05;
            poligons[0]->metalness[2] = poligons[1]->metalness[2] = poligons[2]->metalness[2] = poligons[3]->metalness[2] = 0.05;

            // テクスチャ読み込み・貼り付け
            // l3Texture texture;
            // l3Load2DTexture("assets/tex4.ppm", &texture);
            // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
            l3SetPoligonsToObject(_object, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(_object, -5, 10, 0);
            l3SetScaleObject(_object, 2, 2, 2);
            l3AddObjectToEnvironment(&env, _object, "test");
        }
        l3Object* obj2 = l3CreateBox();
        {
            l3SetTransposeObject(obj2, -15, 10, 0);
            l3SetScaleObject(obj2, 20, 20, 20);
            // l3AddObjectToEnvironment(&env, obj2, "box");
        }
        l3Object* sphere = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(sphere, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(sphere, l3CreateVertex(0, 10, 0, &red)),
            };
            l3Poligon* poligons[] = {
                l3CreatePoligonSphere(0, 1, 10),
            };
            poligons[0]->color.r = 255;
            poligons[0]->color.g = 50;
            poligons[0]->color.b = 50;
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->lightType = l3LightTypePoint;
            poligons[0]->lightIntensity = 3;
            // poligons[0]->transparency = 0.5;
            poligons[0]->lightAttenuation = 0.004;

            l3SetPoligonsToObject(sphere, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(sphere, 15, 0, 0);
            l3AddObjectToEnvironment(&env, sphere, "sphere");
        }
        l3Object* sphere2 = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(sphere2, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(sphere2, l3CreateVertex(0, 5, 0, &red)),
            };
            l3Poligon* poligons[] = {
                l3CreatePoligonSphere(0, 1, 10),
            };
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->color.r = 255;
            poligons[0]->color.g = 255;
            poligons[0]->color.b = 255;
            poligons[0]->lightType = l3LightTypePoint;
            poligons[0]->lightIntensity = 0.4;
            // poligons[0]->lightAttenuation = 1;
            poligons[0]->normal[0] = 0;
            poligons[0]->normal[1] = -1;
            poligons[0]->normal[2] = 0;
            poligons[0]->noSize = true;
            // 単位ベクトル化
            l3NormarizeVec3(poligons[0]->normal, poligons[0]->normal);

            l3SetPoligonsToObject(sphere2, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(sphere2, 0, 100, 0);
            l3AddObjectToEnvironment(&env, sphere2, "sphere2");
        }

        l3Object* point1 = l3CloneObject(sphere);
        {
            point1->poligons[0]->color.r = 0;
            point1->poligons[0]->color.g = 0;
            point1->poligons[0]->color.b = 0;
            l3SetTransposeObject(point1, -90, 70, 0);
            l3AddObjectToEnvironment(&env, point1, "point1");
        }

        l3Object* point2 = l3CloneObject(sphere);
        {
            point2->poligons[0]->color.r = 0;
            point2->poligons[0]->color.g = 0;
            point2->poligons[0]->color.b = 0;
            l3SetTransposeObject(point2, 90, 70, 0);
            l3AddObjectToEnvironment(&env, point2, "point2");
        }

        l3Object* obj3 = l3CreateObject();
        {
            l3AddVertexToObject(obj3, l3CreateVertex(0, 0, 0, &blue));
            l3Mat31A normal = {0, 1, 0};  // 正しい
            l3SetTransposeObject(obj3, 0, -10, 0);
            l3Poligon* poligons[] = {
                l3CreatePoligonPlane(0, normal),
            };
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->color.r = 60;
            poligons[0]->color.g = 60;
            poligons[0]->color.b = 60;
            poligons[0]->metalness[0] = poligons[0]->metalness[1] = poligons[0]->metalness[2] = 0.1;
            poligons[0]->roughness = radians(30);
            poligons[0]->roughnessSamples = 2;

            l3SetPoligonsToObject(obj3, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3AddObjectToEnvironment(&env, obj3, "plane");
        }
        l3Object* sky = l3CreateObject();
        {
            l3Poligon* poligons[] = {
                l3CreatePoligonSky(),
            };
            poligons[0]->color.r = 200;
            poligons[0]->color.g = 200;
            poligons[0]->color.b = 200;
            poligons[0]->lightType = l3LightTypeParallel;
            poligons[0]->lightIntensity = 1;
            poligons[0]->normal[0] = 1;
            poligons[0]->normal[1] = -1;
            poligons[0]->normal[2] = 1;
            l3NormarizeVec3(poligons[0]->normal, poligons[0]->normal);

            l3SetPoligonsToObject(sky, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3AddObjectToEnvironment(&env, sky, "sky");
        }

        l3SetCameraInfoToEnvironment(&env, 0, 20, -60,
                                     0, 0, 0,
                                     0, 1, 0,
                                     radians(50), 2, 100000);

        l3MultithreadSequentialRenderer(&env, transition, options);
        // l3MultithreadRenderer(&env, options->renderer, transition, options->frames, options->threads);
        // l3MultithreadSequentialRenderer(&env, l3RasterizingRenderer, transition, 100, 16);

        l3DestructEnvironment(&env);
    }
}