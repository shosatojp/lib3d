#include "lib3d.h"

static void transition(l3Environment *env, int frame)
{
    l3Object *obj = l3FindObject(env, "box");
    obj->theta_y += radians(360 / 100);
    obj->theta_x += radians(360 / 100);
    obj->theta_z += radians(360 / 100);
    obj->dx = 15 * cos(frame / 100.0 * 2 * PI + PI);
    obj->dz = 15 * sin(frame / 100.0 * 2 * PI + PI);

    // l3Vertex *v0 = array_at(&obj->vertices, 0);
    // v0->coordinate[0] = frame;
    // v0->coordinate[1] = frame*2;
    // v0->coordinate[2]  = frame*3;
    // v0->coordinate[3] = 1;

    l3Object *sphere = l3FindObject(env, "sphere");
    sphere->theta_y += radians(360 / 100);
    sphere->dy = 50 * fabs(sin(1.0 * frame / 5.0));
    sphere->dx = 15 * cos(frame / 100.0 * 2 * PI);
    sphere->dz = 15 * sin(frame / 100.0 * 2 * PI);

    l3Object *ramiel_trans = l3FindObject(env, "ramiel_trans");
    ramiel_trans->theta_x = (frame / 10.0 * 2 * PI)*l3TimeTransition(l3TimeType_EasyEaseIn,frame,0,100);

    // sphere->poligons[0]->textureRotate += radians(1);

    env->camera.coordinate[0] = 400 * cos(-(0 + 1) / 100.0 * 2 * PI);
    env->camera.coordinate[1] = 250;
    env->camera.coordinate[2] = 400 * sin(-(0 + 1) / 100.0 * 2 * PI);
    env->camera.target[0] = 0;
    env->camera.target[1] = 250;
    env->camera.target[2] = 0;
}

int scene_core(int argc, const char *argv[], l3Options *options)
{
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
        l3Object *_object = l3CreateObject();

        // テクスチャ読み込み
        l3Texture texture;
        //l3Load2DTexture("assets/manbou.ppm", &texture);

        {
            int vs[] = {
                l3AddVertexToObject(_object, l3CreateVertex(5, 5, 5, &red)),
                l3AddVertexToObject(_object, l3CreateVertex(-5, -5, 5, &green)),
                l3AddVertexToObject(_object, l3CreateVertex(5, -5, -5, &blue)),
                l3AddVertexToObject(_object, l3CreateVertex(-5, 5, -5, &white)),
            };
            // 右回りが表、数字はオブジェクトごとの頂点のインデックス
            l3Poligon *poligons[] = {
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
            poligons[2]->color.r = 255;
            poligons[2]->color.b = 255;
            poligons[2]->color.g = 255;
            poligons[3]->material = l3PoligonMaterialColor;
            poligons[3]->color.r = 150;
            poligons[3]->color.b = 150;
            poligons[0]->transparency = 0.1;
            // poligons[1]->transparency = 0.1;
            poligons[2]->transparency = 0.1;
            // poligons[3]->transparency = 0.1;
            poligons[0]->metalness[0] = poligons[1]->metalness[0] = poligons[2]->metalness[0] = poligons[3]->metalness[0] = 0.04;
            poligons[0]->metalness[1] = poligons[1]->metalness[1] = poligons[2]->metalness[1] = poligons[3]->metalness[1] = 0.04;
            poligons[0]->metalness[2] = poligons[1]->metalness[2] = poligons[2]->metalness[2] = poligons[3]->metalness[2] = 0.9;

            // テクスチャ読み込み・貼り付け
            for (int i = 0; i < 2; i++)
            {
                poligons[i]->textureType = l3TextureTypeTiled;
                poligons[i]->textureRepeatX = 10;
                poligons[i]->textureRepeatY = 10;
                poligons[i]->textureCoordinateSystem = l3CoordinateSystemLocal;
                poligons[i]->texture = &texture;
                // poligons[i]->lightType = l3LightTypePoint;
                // poligons[i]->lightIntensity = 0.4;
            }
            l3Mat32A texture_vertices = {0.5, 0, 0, 0.8, 1, 0.8};
            for (int i = 2; i < 4; i++)
            {
                l3SetUVTexturePoligon(poligons[i], &texture, texture_vertices);
            }

            l3SetPoligonsToObject(_object, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3SetTransposeObject(_object, -15, 10, 0);
            l3SetScaleObject(_object, 2, 2, 2);
            l3AddObjectToEnvironment(&env, _object, "test");
        }
        l3Object *obj2 = l3CreateBox();
        {
            l3SetTransposeObject(obj2, 0, 0, 0);
            l3SetScaleObject(obj2, 10, 10, 10);
            // l3AddObjectToEnvironment(&env, obj2, "box1");
        }
        l3Object *obj5 = l3CreateBox();
        {
            l3SetTransposeObject(obj5, -20, 0, 0);
            l3SetScaleObject(obj5, 10, 10, 10);
            // l3AddObjectToEnvironment(&env, obj5, "box2");
        }
        l3Object *obj4 = l3CreateBox();
        {
            l3SetTransposeObject(obj4, 20, 0, 0);
            l3SetScaleObject(obj4, 10, 10, 10);
            // l3AddObjectToEnvironment(&env, obj4, "boxddd3");
        }

        l3Object *ramiel_trans = l3CreateObject();
        {
            int i;
            l3RGB ramiel_blue = {100, 100, 250};
            l3RGB ramiel_pink = {220, 100, 100};

            l3Mat41A p[36] = {0};
            l3PolarToMat41A(p[0], 0, 0, 0);

            for (i = 1; i <= 5; i++)
            {
                l3PolarToMat41A(p[1 + 7 * (i - 1)], 30, PI, PI / 2 + 2 * PI / 5 * (i - 1));
                l3PolarToMat41A(p[2 + 7 * (i - 1)], 100, 5 * PI / 6, PI / 2 + 2 * PI / 5 * (i - 1));
                l3PolarToMat41A(p[3 + 7 * (i - 1)], 300, PI / 2 - PI / 6, PI / 2 + 2 * PI / 5 * (i - 1));
                l3PolarToMat41A(p[4 + 7 * (i - 1)], 100, PI / 2 - PI / 12, PI / 2 - PI / 10 + 2 * PI / 5 * (i - 1));
                l3PolarToMat41A(p[5 + 7 * (i - 1)], 80, PI / 2 - PI / 20, PI / 2 - PI / 10 + 2 * PI / 5 * (i - 1));
                l3PolarToMat41A(p[6 + 7 * (i - 1)], 30, PI / 6, PI / 2 + 2 * PI / 5 * (i - 1));
                l3PolarToMat41A(p[7 + 7 * (i - 1)], 30, PI / 4, PI / 2 - PI / 10 + 2 * PI / 5 * (i - 1));
            }

            int vs[] = {

                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[0][0], p[0][1], p[0][2], &ramiel_blue)), //0
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[1][0], p[1][1], p[1][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[2][0], p[2][1], p[2][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[3][0], p[3][1], p[3][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[4][0], p[4][1], p[4][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[5][0], p[5][1], p[5][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[6][0], p[6][1], p[6][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[7][0], p[7][1], p[7][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[8][0], p[8][1], p[8][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[9][0], p[9][1], p[9][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[10][0], p[10][1], p[10][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[11][0], p[11][1], p[11][2], &ramiel_blue)), //0
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[12][0], p[12][1], p[12][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[13][0], p[13][1], p[13][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[14][0], p[14][1], p[14][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[15][0], p[15][1], p[15][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[16][0], p[16][1], p[16][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[17][0], p[17][1], p[17][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[18][0], p[18][1], p[18][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[19][0], p[19][1], p[19][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[20][0], p[20][1], p[20][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[21][0], p[21][1], p[21][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[22][0], p[22][1], p[22][2], &ramiel_blue)), //0
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[23][0], p[23][1], p[23][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[24][0], p[24][1], p[24][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[25][0], p[25][1], p[25][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[26][0], p[26][1], p[26][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[27][0], p[27][1], p[27][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[28][0], p[28][1], p[28][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[29][0], p[29][1], p[29][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[30][0], p[30][1], p[30][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[31][0], p[31][1], p[31][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[32][0], p[32][1], p[32][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[33][0], p[33][1], p[33][2], &ramiel_blue)), //0
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[34][0], p[34][1], p[34][2], &ramiel_blue)),
                l3AddVertexToObject(ramiel_trans, l3CreateVertex(p[35][0], p[35][1], p[35][2], &ramiel_blue)),
            };

            // 右回りが表、数字はオブジェクトごとの頂点のインデックス

            l3Poligon *poligons[] = {

                l3CreatePoligon(3 + 7 * 0, 4 + 7 * 0, 6 + 7 * 0), //0
                l3CreatePoligon(6 + 7 * 0, 4 + 7 * 0, 7 + 7 * 0),
                l3CreatePoligon(0 + 7 * 0, 6 + 7 * 0, 7 + 7 * 0),
                l3CreatePoligon(7 + 7 * 0, 4 + 7 * 0, 6 + 7 * (0 + 1)),
                l3CreatePoligon(0 + 7 * 0, 7 + 7 * 0, 6 + 7 * (0 + 1)),
                l3CreatePoligon(4 + 7 * 0, 3 + 7 * (0 + 1), 6 + 7 * (0 + 1)),
                l3CreatePoligon(2 + 7 * 0, 4 + 7 * 0, 3 + 7 * 0),
                l3CreatePoligon(2 + 7 * 0, 5 + 7 * 0, 4 + 7 * 0),
                l3CreatePoligon(4 + 7 * 0, 5 + 7 * 0, 2 + 7 * (0 + 1)),
                l3CreatePoligon(3 + 7 * (0 + 1), 4 + 7 * 0, 2 + 7 * (0 + 1)), //9

                l3CreatePoligon(3 + 7 * 1, 4 + 7 * 1, 6 + 7 * 1), //0
                l3CreatePoligon(6 + 7 * 1, 4 + 7 * 1, 7 + 7 * 1),
                l3CreatePoligon(0 + 7 * 1, 6 + 7 * 1, 7 + 7 * 1),
                l3CreatePoligon(7 + 7 * 1, 4 + 7 * 1, 6 + 7 * (1 + 1)),
                l3CreatePoligon(0 + 7 * 1, 7 + 7 * 1, 6 + 7 * (1 + 1)),
                l3CreatePoligon(4 + 7 * 1, 3 + 7 * (1 + 1), 6 + 7 * (1 + 1)),
                l3CreatePoligon(2 + 7 * 1, 4 + 7 * 1, 3 + 7 * 1),
                l3CreatePoligon(2 + 7 * 1, 5 + 7 * 1, 4 + 7 * 1),
                l3CreatePoligon(4 + 7 * 1, 5 + 7 * 1, 2 + 7 * (1 + 1)),
                l3CreatePoligon(3 + 7 * (1 + 1), 4 + 7 * 1, 2 + 7 * (1 + 1)), //9

                l3CreatePoligon(3 + 7 * 2, 4 + 7 * 2, 6 + 7 * 2), //0
                l3CreatePoligon(6 + 7 * 2, 4 + 7 * 2, 7 + 7 * 2),
                l3CreatePoligon(0 + 7 * 2, 6 + 7 * 2, 7 + 7 * 2),
                l3CreatePoligon(7 + 7 * 2, 4 + 7 * 2, 6 + 7 * (2 + 1)),
                l3CreatePoligon(0 + 7 * 2, 7 + 7 * 2, 6 + 7 * (2 + 1)),
                l3CreatePoligon(4 + 7 * 2, 3 + 7 * (2 + 1), 6 + 7 * (2 + 1)),
                l3CreatePoligon(2 + 7 * 2, 4 + 7 * 2, 3 + 7 * 2),
                l3CreatePoligon(2 + 7 * 2, 5 + 7 * 2, 4 + 7 * 2),
                l3CreatePoligon(4 + 7 * 2, 5 + 7 * 2, 2 + 7 * (2 + 1)),
                l3CreatePoligon(3 + 7 * (2 + 1), 4 + 7 * 2, 2 + 7 * (2 + 1)), //9

                l3CreatePoligon(3 + 7 * 3, 4 + 7 * 3, 6 + 7 * 3), //0
                l3CreatePoligon(6 + 7 * 3, 4 + 7 * 3, 7 + 7 * 3),
                l3CreatePoligon(0 + 7 * 3, 6 + 7 * 3, 7 + 7 * 3),
                l3CreatePoligon(7 + 7 * 3, 4 + 7 * 3, 6 + 7 * (3 + 1)),
                l3CreatePoligon(0 + 7 * 3, 7 + 7 * 3, 6 + 7 * (3 + 1)),
                l3CreatePoligon(4 + 7 * 3, 3 + 7 * (3 + 1), 6 + 7 * (3 + 1)),
                l3CreatePoligon(2 + 7 * 3, 4 + 7 * 3, 3 + 7 * 3),
                l3CreatePoligon(2 + 7 * 3, 5 + 7 * 3, 4 + 7 * 3),
                l3CreatePoligon(4 + 7 * 3, 5 + 7 * 3, 2 + 7 * (3 + 1)),
                l3CreatePoligon(3 + 7 * (3 + 1), 4 + 7 * 3, 2 + 7 * (3 + 1)), //9

                l3CreatePoligon(3 + 7 * 4, 4 + 7 * 4, 6 + 7 * 4), //0
                l3CreatePoligon(6 + 7 * 4, 4 + 7 * 4, 7 + 7 * 4),
                l3CreatePoligon(0 + 7 * 4, 6 + 7 * 4, 7 + 7 * 4),
                l3CreatePoligon(7 + 7 * 4, 4 + 7 * 4, 6 + 7 * 0),
                l3CreatePoligon(0 + 7 * 4, 7 + 7 * 4, 6 + 7 * 0),
                l3CreatePoligon(4 + 7 * 4, 3 + 7 * 0, 6 + 7 * 0),
                l3CreatePoligon(2 + 7 * 4, 4 + 7 * 4, 3 + 7 * 4),
                l3CreatePoligon(2 + 7 * 4, 5 + 7 * 4, 4 + 7 * 4),
                l3CreatePoligon(4 + 7 * 4, 5 + 7 * 4, 2 + 7 * 0),
                l3CreatePoligon(3 + 7 * 0, 4 + 7 * 4, 2 + 7 * 0), //9

            };

            for (i = 0; i <= 4; i++)
            {
                poligons[i * 10]->material = l3PoligonMaterialColor;
                poligons[i * 10]->color = ramiel_blue;

                poligons[1 + i * 10]->material = l3PoligonMaterialColor;
                poligons[1 + i * 10]->color = ramiel_pink;

                poligons[2 + i * 10]->material = l3PoligonMaterialColor;
                poligons[2 + i * 10]->color = ramiel_pink;

                poligons[3 + i * 10]->material = l3PoligonMaterialColor;
                poligons[3 + i * 10]->color = ramiel_pink;

                poligons[4 + i * 10]->material = l3PoligonMaterialColor;
                poligons[4 + i * 10]->color = ramiel_pink;

                poligons[5 + i * 10]->material = l3PoligonMaterialColor;
                poligons[5 + i * 10]->color = ramiel_blue;

                poligons[6 + i * 10]->material = l3PoligonMaterialColor;
                poligons[6 + i * 10]->color = ramiel_blue;

                poligons[7 + i * 10]->material = l3PoligonMaterialColor;
                poligons[7 + i * 10]->color = ramiel_blue;

                poligons[8 + i * 10]->material = l3PoligonMaterialColor;
                poligons[8 + i * 10]->color = ramiel_blue;

                poligons[9 + i * 10]->material = l3PoligonMaterialColor;
                poligons[9 + i * 10]->color = ramiel_blue;
            }

            for (i = 0; i <= 36; i++)
            {
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
            l3SetPoligonsToObject(ramiel_trans, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3SetTransposeObject(ramiel_trans, 0, 250, 0);
            l3SetScaleObject(ramiel_trans, 1, 1, 1);
            l3AddObjectToEnvironment(&env, ramiel_trans, "ramiel_trans");
        }

        l3Object *column = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(column, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(column, l3CreateVertex(0, 20, 0, &red)),
            };
            l3Poligon *poligons[] = {
                l3CreatePoligonColumn(0, 1, 10),
            };
            poligons[0]->color.r = 255;
            poligons[0]->color.g = 250;
            poligons[0]->color.b = 50;
            poligons[0]->material = l3PoligonMaterialColor;

            l3SetPoligonsToObject(column, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3SetTransposeObject(column, 0, 0, 0);
            // l3AddObjectToEnvironment(&env, column, "column");
        }
        l3Texture texture2;
        // l3Load2DTexture("assets/star.ppm", &texture2);

        l3Object *sphere = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(sphere, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(sphere, l3CreateVertex(0, 10, 0, &red)),
            };
            l3Poligon *poligons[] = {
                l3CreatePoligonSphere(0, 1, 10),
            };
            poligons[0]->color.r = 255;
            poligons[0]->color.g = 50;
            poligons[0]->color.b = 50;
            poligons[0]->lightColor.r = 255;
            poligons[0]->lightColor.g = 50;
            poligons[0]->lightColor.b = 50;
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->lightType = l3LightTypePoint;
            poligons[0]->lightIntensity = 3;
            // poligons[0]->transparency = 0.5;
            poligons[0]->lightAttenuation = 0.004;
            // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
            // poligons[0]->textureType = l3TextureTypeTiled;
            // poligons[0]->textureRepeatX = 10;
            // poligons[0]->textureRepeatY = 10;
            // poligons[0]->textureCoordinateSystem = l3CoordinateSystemLocal;
            // poligons[0]->texture = &texture2;
            // poligons[0]->normal[0] = 0;
            // poligons[0]->normal[1] = 1;
            // poligons[0]->normal[2] = 0;

            l3SetPoligonsToObject(sphere, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3SetTransposeObject(sphere, 15, 0, 0);
            l3AddObjectToEnvironment(&env, sphere, "sphere");
        }
        l3Object *sphere2 = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(sphere2, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(sphere2, l3CreateVertex(0, 5, 0, &red)),
            };
            l3Poligon *poligons[] = {
                l3CreatePoligonSphere(0, 1, 10),
            };
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->color = white;
            poligons[0]->lightColor = white;
            poligons[0]->lightType = l3LightTypePoint;
            poligons[0]->lightIntensity = 0.4;
            // poligons[0]->lightAttenuation = 1;
            poligons[0]->normal[0] = 0;
            poligons[0]->normal[1] = -1;
            poligons[0]->normal[2] = 0;
            poligons[0]->noSize = true;
            // 単位ベクトル化
            l3NormarizeVec3(poligons[0]->normal, poligons[0]->normal);

            l3SetPoligonsToObject(sphere2, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3SetTransposeObject(sphere2, 0, 100, 0);
            l3AddObjectToEnvironment(&env, sphere2, "sphere2");
        }

        l3Object *obj3 = l3CreateObject();
        {
            l3AddVertexToObject(obj3, l3CreateVertex(0, 0, 0, &blue));
            l3Mat31A normal = {0, 1, 0}; // 正しい
            l3SetTransposeObject(obj3, 0, 0, 0);
            l3Poligon *poligons[] = {
                l3CreatePoligonPlane(0, normal),
            };
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->color.r = 60;
            poligons[0]->color.g = 60;
            poligons[0]->color.b = 60;
            poligons[0]->metalness[0] = poligons[0]->metalness[1] = poligons[0]->metalness[2] = 0.1;
            poligons[0]->roughness = radians(10);
            poligons[0]->roughnessSamples = 2;

            l3SetPoligonsToObject(obj3, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3AddObjectToEnvironment(&env, obj3, "plane");
        }
        l3Object *sky = l3CreateObject();
        {
            l3Poligon *poligons[] = {
                l3CreatePoligonSky(),
            };
            poligons[0]->color.r = 176;
            poligons[0]->color.g = 213;
            poligons[0]->color.b = 255;
            poligons[0]->lightColor = white;
            poligons[0]->lightType = l3LightTypeParallel;
            poligons[0]->lightIntensity = 1;
            poligons[0]->normal[0] = 1;
            poligons[0]->normal[1] = -1;
            poligons[0]->normal[2] = 1;
            l3NormarizeVec3(poligons[0]->normal, poligons[0]->normal);

            l3SetPoligonsToObject(sky, sizeof(poligons) / sizeof(l3Poligon *), poligons);
            l3AddObjectToEnvironment(&env, sky, "sky");
        }

        l3SetCameraInfoToEnvironment(&env, 0, 10, -60,
                                     0, 0, 0,
                                     0, 1, 0,
                                     radians(50), 2, 100000);

        env.environmentColor = white;
        env.environmentLightRate = 0.1;
        env.environmentLightIntensity = 2;

        l3MultithreadSequentialRenderer(&env, transition, options);
        l3DestructEnvironment(&env);
    }
    return 0;
}