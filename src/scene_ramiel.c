#include "array.h"
#include "lib3d.h"

static void transition(l3Environment *env, int frame)
{

    int mode = 1; //0は普通、１は変形

    l3Object *ramiel_normal = l3FindObject(env, "ramiel_normal");
    l3Vertex *v0 = array_at(&ramiel_normal->vertices, 0);
    l3Vertex *v1 = array_at(&ramiel_normal->vertices, 1);
    l3Vertex *v2 = array_at(&ramiel_normal->vertices, 2);
    l3Vertex *v3 = array_at(&ramiel_normal->vertices, 3);
    l3Vertex *v4 = array_at(&ramiel_normal->vertices, 4);
    l3Vertex *v5 = array_at(&ramiel_normal->vertices, 5);
    l3Vertex *v6 = array_at(&ramiel_normal->vertices, 6);
    l3Vertex *v7 = array_at(&ramiel_normal->vertices, 7);
    l3Vertex *v8 = array_at(&ramiel_normal->vertices, 8);
    l3Vertex *v9 = array_at(&ramiel_normal->vertices, 9);
    l3Vertex *v10 = array_at(&ramiel_normal->vertices, 10);

    if (mode == 0)
    {
        ramiel_normal->theta_y += radians(360 / 100);
    }

    if (mode == 1)
    {

        ramiel_normal->theta_y = -frame / 360.0*2.0*PI;

        int keyframe1 = 30, keyframe2 = 90, keyframe3 = 120;

        if (frame < keyframe1)
        {

            ramiel_normal->dy = 800 - 600 * l3TimeTransition(l3TimeType_linear, frame, 0, keyframe1);
        }

        if (frame >= keyframe1 && frame < keyframe2)
        {

            bezier in[2], l1[2], l2[2], l3[2], l4[2];
            {
                l3SetBezierPoint(&in[0], 0, 0, 200);
                l3SetBezierPoint(&in[1], 0, 0, 0);

                l3SetBezierPoint(&l1[0], 0, 0, 200);
                l3SetBezierPoint(&l1[1], 200, 0, 0);
                l3SetBezierPoint(&l2[0], 0, 0, 200);
                l3SetBezierPoint(&l2[1], -200, 0, 0);
                l3SetBezierPoint(&l3[0], 0, 0, 200);
                l3SetBezierPoint(&l3[1], 0, 200, 0);
                l3SetBezierPoint(&l4[0], 0, 0, 200);
                l3SetBezierPoint(&l4[1], 0, -200, 0);
            }

            l3BezierToMat41A(v4->coordinate, l3GetBezierCurve(l3TimeTransition(l3TimeType_EasyEaseOut, frame, keyframe1, keyframe2), 2, in));
            l3BezierToMat41A(v7->coordinate, l3GetBezierCurve(l3TimeTransition(l3TimeType_linear, frame, keyframe1, keyframe2), 2, l3));
            l3BezierToMat41A(v8->coordinate, l3GetBezierCurve(l3TimeTransition(l3TimeType_linear, frame, keyframe1, keyframe2), 2, l1));
            l3BezierToMat41A(v9->coordinate, l3GetBezierCurve(l3TimeTransition(l3TimeType_linear, frame, keyframe1, keyframe2), 2, l4));
            l3BezierToMat41A(v10->coordinate, l3GetBezierCurve(l3TimeTransition(l3TimeType_linear, frame, keyframe1, keyframe2), 2, l2));

            l3PrintMat(v4->coordinate , 4, 1);
            l3PrintMat(v7->coordinate , 4, 1);
            l3PrintMat(v8->coordinate , 4, 1);
            l3PrintMat(v9->coordinate , 4, 1);
            l3PrintMat(v10->coordinate , 4, 1);
        }
    }

    l3Object *light1 = l3FindObject(env, "light1");
    light1->dx = 800 * cos(-67 / 100.0 * 2.0 * PI);
    light1->dz = 800 * sin(-67 / 100.0 * 2.0 * PI);
}

int scene_ramiel(int argc, const char *argv[], l3Options *options)
{
    l3Environment env;
    {
        int i;
        l3RGB red = {255, 0, 0};
        l3RGB green = {0, 255, 0};
        l3RGB blue = {0, 0, 255};
        l3RGB white = {255, 255, 255};
        l3RGB ramiel_blue = {100, 100, 250};
        l3RGB ramiel_pink = {220, 100, 100};
        l3InitializeEnvironment(&env);
        env.w = options->w;
        env.h = options->h;
        env.outdir = options->outdir;

        // オブジェクト構築

        l3Object *ramiel_normal = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 0, 0, &ramiel_blue)), //0
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 200, 0, &ramiel_blue)),
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(200, 0, 0, &ramiel_blue)),
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 0, -200, &ramiel_blue)),
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 0, 200, &ramiel_blue)),
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(-200, 0, 0, &ramiel_blue)),
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, -200, 0, &ramiel_blue)), //6

                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 0, 200, &ramiel_blue)), //7 開くよう
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 0, 200, &ramiel_blue)),
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 0, 200, &ramiel_blue)),
                l3AddVertexToObject(ramiel_normal, l3CreateVertex(0, 0, 200, &ramiel_blue)),
            };

            // 右回りが表、数字はオブジェクトごとの頂点のインデックス
            l3Poligon *poligons[] = {
                l3CreatePoligon(1, 3, 2),
                l3CreatePoligon(1, 5, 3),
                l3CreatePoligon(6, 2, 3),
                l3CreatePoligon(6, 3, 5),

                l3CreatePoligon(4, 7, 8),
                l3CreatePoligon(4, 8, 9),
                l3CreatePoligon(4, 9, 10),
                l3CreatePoligon(4, 10, 7),

                l3CreatePoligon(5, 1, 10),
                l3CreatePoligon(1, 7, 10),
                l3CreatePoligon(1, 2, 7),
                l3CreatePoligon(2, 8, 7),
                l3CreatePoligon(2, 6, 8),
                l3CreatePoligon(6, 9, 8),
                l3CreatePoligon(5, 9, 6),
                l3CreatePoligon(5, 10, 9),
                

            };

            for (i = 0; i < 16; i++)
            {
                poligons[i]->material = l3PoligonMaterialColor;
                poligons[i]->color = ramiel_blue;

                poligons[i]->metalness[0] = 0.5;
                poligons[i]->metalness[1] = 0.5;
                poligons[i]->metalness[2] = 0.0005;
                poligons[i]->transparency = 0.05;
            }

            // poligons[0]->lightType        = poligons[1]->lightType        = poligons[2]->lightType        = poligons[3]->lightType        = poligons[4]->lightType        = poligons[5]->lightType        = poligons[6]->lightType        = poligons[7]->lightType        = l3LightTypeParallel;
            // poligons[0]->lightIntensity   = poligons[1]->lightIntensity   = poligons[2]->lightIntensity   = poligons[3]->lightIntensity   = poligons[4]->lightIntensity   = poligons[5]->lightIntensity   = poligons[6]->lightIntensity   = poligons[7]->lightIntensity   = 0.01;
            // poligons[0]->lightAttenuation = poligons[1]->lightAttenuation = poligons[2]->lightAttenuation = poligons[3]->lightAttenuation = poligons[4]->lightAttenuation = poligons[5]->lightAttenuation = poligons[6]->lightAttenuation = poligons[7]->lightAttenuation = 0.4;

            // テクスチャ読み込み・貼り付け
            // l3Texture texture;
            // l3Load2DTexture("assets/tex4.ppm", &texture);
            // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
            l3SetPoligonsToObject(ramiel_normal, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3SetTransposeObject(ramiel_normal, 0, 250, 0);
            l3SetScaleObject(ramiel_normal, 1, 1, 1);
            l3AddObjectToEnvironment(&env, ramiel_normal, "ramiel_normal");
        }

        // l3Object *ramiel_trans = l3CreateObject();
        // {
        //     int vs[] = {
        //         l3AddVertexToObject(ramiel_trans, l3CreateVertex(0, 0, 0, &ramiel_blue)), //0

        //     };

        //     // 右回りが表、数字はオブジェクトごとの頂点のインデックス
        //     l3Poligon *poligons[] = {
        //         l3CreatePoligon(1, 3, 2),
        //     };

        //     poligons[0]->material = l3PoligonMaterialColor;
        //     poligons[0]->color = ramiel_blue;

        //     poligons[0]->metalness[0] = poligons[1]->metalness[0] = poligons[2]->metalness[0] = poligons[3]->metalness[0] = poligons[4]->metalness[0] = poligons[5]->metalness[0] = poligons[6]->metalness[0] = poligons[7]->metalness[0] = poligons[8]->metalness[0] = poligons[9]->metalness[0] = poligons[10]->metalness[0] = poligons[11]->metalness[0] = 0.5;
        //     poligons[0]->metalness[1] = poligons[1]->metalness[1] = poligons[2]->metalness[1] = poligons[3]->metalness[1] = poligons[4]->metalness[1] = poligons[5]->metalness[1] = poligons[6]->metalness[1] = poligons[7]->metalness[1] = poligons[8]->metalness[1] = poligons[9]->metalness[1] = poligons[10]->metalness[1] = poligons[11]->metalness[1] = 0.5;
        //     poligons[0]->metalness[2] = poligons[1]->metalness[2] = poligons[2]->metalness[2] = poligons[3]->metalness[2] = poligons[4]->metalness[2] = poligons[5]->metalness[2] = poligons[6]->metalness[2] = poligons[7]->metalness[2] = poligons[8]->metalness[2] = poligons[9]->metalness[2] = poligons[10]->metalness[2] = poligons[11]->metalness[2] = 0.0005;

        //     poligons[0]->transparency = poligons[1]->transparency = poligons[2]->transparency = poligons[3]->transparency = poligons[4]->transparency = poligons[5]->transparency = poligons[6]->transparency = poligons[7]->transparency = poligons[8]->transparency = poligons[9]->transparency = poligons[10]->transparency = poligons[11]->transparency = 0.05;

        //     // poligons[0]->lightType        = poligons[1]->lightType        = poligons[2]->lightType        = poligons[3]->lightType        = poligons[4]->lightType        = poligons[5]->lightType        = poligons[6]->lightType        = poligons[7]->lightType        = l3LightTypeParallel;
        //     // poligons[0]->lightIntensity   = poligons[1]->lightIntensity   = poligons[2]->lightIntensity   = poligons[3]->lightIntensity   = poligons[4]->lightIntensity   = poligons[5]->lightIntensity   = poligons[6]->lightIntensity   = poligons[7]->lightIntensity   = 0.01;
        //     // poligons[0]->lightAttenuation = poligons[1]->lightAttenuation = poligons[2]->lightAttenuation = poligons[3]->lightAttenuation = poligons[4]->lightAttenuation = poligons[5]->lightAttenuation = poligons[6]->lightAttenuation = poligons[7]->lightAttenuation = 0.4;

        //     // テクスチャ読み込み・貼り付け
        //     // l3Texture texture;
        //     // l3Load2DTexture("assets/tex4.ppm", &texture);
        //     // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
        //     l3SetPoligonsToObject(ramiel_trans, sizeof(poligons) / sizeof(l3Poligon *), poligons);
        //     l3SetTransposeObject(ramiel_trans, 0, 250, 0);
        //     l3SetScaleObject(ramiel_trans, 1, 1, 1);
        //     l3AddObjectToEnvironment(&env, ramiel_trans, "ramiel_trans");
        // }

        l3Object *light1 = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(light1, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(light1, l3CreateVertex(0, 100, 0, &red)),
            };
            l3Poligon *poligons[] = {
                l3CreatePoligonSphere(0, 1, 100),
            };
            poligons[0]->color.r = 20;
            poligons[0]->color.g = 20;
            poligons[0]->color.b = 250;
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->lightType = l3LightTypePoint;
            poligons[0]->lightIntensity = 1000;
            // poligons[0]->transparency = 0.5;
            poligons[0]->lightAttenuation = 0.0004;

            l3SetPoligonsToObject(light1, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3SetTransposeObject(light1, -3000, 200, -3000);
            l3AddObjectToEnvironment(&env, light1, "light1");
        }

        l3Object *light2 = l3CloneObject(light1);
        {
            light2->poligons[0]->color.r = 200;
            light2->poligons[0]->color.g = 20;
            light2->poligons[0]->color.b = 120;
            light2->poligons[0]->transparency = 0.01;

            light2->poligons[0]->lightIntensity = 1000;

            l3SetTransposeObject(light2, 800, 400, 300);
            l3AddObjectToEnvironment(&env, light2, "light2");
        }

        l3Object *light3 = l3CloneObject(light1);
        {
            light3->poligons[0]->color.r = 40;
            light3->poligons[0]->color.g = 40;
            light3->poligons[0]->color.b = 255;
            light3->poligons[0]->transparency = 0.01;

            light3->poligons[0]->lightIntensity = 100;

            l3SetTransposeObject(light3, 50, 1200, 100);
            l3AddObjectToEnvironment(&env, light3, "light3");
        }

        l3Object *sky = l3CreateObject();
        {
            l3Poligon *poligons[] = {
                l3CreatePoligonSky(),
            };
            poligons[0]->color.r = 20;
            poligons[0]->color.g = 20;
            poligons[0]->color.b = 20;
            poligons[0]->lightType = l3LightTypeParallel;
            poligons[0]->lightIntensity = 1;
            poligons[0]->normal[0] = 1;
            poligons[0]->normal[1] = -1;
            poligons[0]->normal[2] = 1;
            l3NormarizeVec3(poligons[0]->normal, poligons[0]->normal);

            l3SetPoligonsToObject(sky, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3AddObjectToEnvironment(&env, sky, "sky");
        }

        l3SetCameraInfoToEnvironment(&env, 500, 0, 700,
                                     0, 250, 0,
                                     0, 1, 0,
                                     radians(50), 2, 100000);

        l3MultithreadSequentialRenderer(&env, transition, options);
        // l3MultithreadRenderer(&env, options->renderer, transition, options->frames, options->threads);
        // l3MultithreadSequentialRenderer(&env, l3RasterizingRenderer, transition, 100, 16);

        l3DestructEnvironment(&env);
    }
}