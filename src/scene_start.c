//最初のシーン

#include "lib3d.h"

static void transition(l3Environment* env, int frame) {
    l3Object* eatora = l3FindObject(env, "eatora");
    l3Object* b02 = l3FindObject(env, "box02");
    l3Object* ramiel_nomal = l3CreateObject(env, "ramiel");

    {  //運動カメラ
        {  //bezier曲線移動サンプル
            bezier cpoint[4]; //カメラ座標ベジェ
            {
                cpoint[0].x = 100;
                cpoint[0].y = 20;
                cpoint[0].z = -100;
                cpoint[1].x = 120;
                cpoint[1].y = 25;
                cpoint[1].z = -100;
                cpoint[2].x = 150;
                cpoint[2].y = 30;
                cpoint[2].z = -100;
                cpoint[3].x = 200;
                cpoint[3].y = 70;
                cpoint[3].z = -70;
            }
            bezier camera_bezier = l3GetBezierCurve(l3TimeTransition(l3TimeType_EasyEase, frame, 0, 180), 4, cpoint);
            env->camera.coordinate[0] = camera_bezier.x;
            env->camera.coordinate[1] = camera_bezier.y;
            env->camera.coordinate[2] = camera_bezier.z;
        }
        {  
            bezier cpoint[4];//ターゲット座標ベジェ
            {
                cpoint[0].x = 60;
                cpoint[0].y = 20;
                cpoint[0].z = 25;
                cpoint[1].x = 70;
                cpoint[1].y = 30;
                cpoint[1].z = 25;
                cpoint[2].x = 80;
                cpoint[2].y = 35;
                cpoint[2].z = 30;
                cpoint[3].x = -360;
                cpoint[3].y = 505;
                cpoint[3].z = 225;
            }
            bezier cameratarget_bezier = l3GetBezierCurve(l3TimeTransition(l3TimeType_EasyEase, frame, 0, 180), 4, cpoint);
            env->camera.target[0] = cameratarget_bezier.x;
            env->camera.target[1] = cameratarget_bezier.y;
            env->camera.target[2] = cameratarget_bezier.z;
        }
    }
    if (frame >= 180)
    {
        {
            bezier cpoint[4]; //カメラ座標ベジェ
            {
                cpoint[0].x = -500;
                cpoint[0].y = 800;
                cpoint[0].z = -300;
                cpoint[1].x = -300;
                cpoint[1].y = 650;
                cpoint[1].z = -400;
                cpoint[2].x = -50;
                cpoint[2].y = 550;
                cpoint[2].z = -350;
                cpoint[3].x = 50;
                cpoint[3].y = 450;
                cpoint[3].z = 150;
            }
            bezier camera_bezier = l3GetBezierCurve(l3TimeTransition(l3TimeType_EasyEase, frame, 180, 300), 4, cpoint);
            env->camera.coordinate[0] = camera_bezier.x;
            env->camera.coordinate[1] = camera_bezier.y;
            env->camera.coordinate[2] = camera_bezier.z;
        }
        env->camera.target[0] = -310;
        env->camera.target[1] = 505;
        env->camera.target[2] = 225;
    }
}

int scene_start(int argc, const char* argv[], l3Options* options) {
    l3Environment env;//envの情報
    {
        l3RGB red = {255, 0, 0};
        l3RGB green = {0, 255, 0};
        l3RGB blue = {0, 0, 255};
        l3RGB white = {255, 255, 255};
        l3RGB ramiel_blue = {100,100,250};
        l3RGB ramiel_pink = {220,100,100};
        l3InitializeEnvironment(&env);
        env.w = options->w;
        env.h = options->h;
        env.outdir = options->outdir;

        l3Object* box = l3CreateBox();//箱を作る関数
        {
            box->poligons[0]->color.r = 255;
            l3SetTransposeObject(box, 0, 0, 0);
            l3SetScaleObject(box, 0, 0, 0);
            //l3AddObjectToEnvironment(&env, box, "box");
        }
        
        l3Object* b01 = l3CreateBox();
        {
            int i;
            for (i = 0; i < 11; ++i) {
                b01->poligons[i]->color.r = 0;
                b01->poligons[i]->color.g = 0;
                b01->poligons[i]->color.b = 0;
            }
            //テクスチャ読み込み・貼り付け
            for (i = 0; i < 11; ++i) {
                l3Texture texture;
                l3Load2DTexture("assets/building.ppm", &texture);
                l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
                b01->poligons[i]->textureType = l3TextureTypeTiled;
                b01->poligons[i]->textureRepeatX = 4;
                b01->poligons[i]->textureRepeatY = 4;
                b01->poligons[i]->textureCoordinateSystem = l3CoordinateSystemLocal;
                b01->poligons[i]->texture = &texture;
                //b01->poligons[i]->lightType = l3LightTypePoint;
                //b01->poligons[i]->lightIntensity = 4;
            }


            l3SetTransposeObject(b01, 0, 0, 0);
            l3SetScaleObject(b01, 0, 0, 0);
            //l3AddObjectToEnvironment(&env, b01, "box01");
        }
        double i,j;
        for(i = 0; i < 7; ++i){
            double n = (i+1.0)*10.0;
            for(j = 0; j < n; ++j){
                l3Object* b02 = l3CloneObject(b01);//クローン
                {
                    l3SetTransposeObject(b01, (i+5.0)*80*cos(j / n * 2.0 * PI) , 20.0, (i+5.0)*80*sin(j / n * 2.0 * PI));
                    l3SetScaleObject(b01, 20 + (rand()%5)*5, 50 + (rand()%10)*20, 20 + (rand()%5)*5);
                    l3AddObjectToEnvironment(&env, b02, "box02");
                }
            }
        }
        
        l3Object* ramiel_nomal = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(ramiel_nomal,l3CreateVertex(0,0,0,&ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal,l3CreateVertex(0,200,0,&ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal,l3CreateVertex(200,0,0,&ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal,l3CreateVertex(0,0,-200,&ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal,l3CreateVertex(0,0,200,&ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal,l3CreateVertex(-200,0,0,&ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal,l3CreateVertex(0,-200,0,&ramiel_blue)),
            };

            // 右回りが表、数字はオブジェクトごとの頂点のインデックス
            l3Poligon* poligons[] = {
                l3CreatePoligon(1,3,2),
                l3CreatePoligon(1,2,4),
                l3CreatePoligon(1,5,3),
                l3CreatePoligon(1,4,5),
                l3CreatePoligon(6,2,3),
                l3CreatePoligon(6,4,2),
                l3CreatePoligon(6,3,5),
                l3CreatePoligon(6,5,4),
            };

            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->color = ramiel_blue;
            poligons[1]->material = l3PoligonMaterialColor;
            poligons[1]->color = ramiel_blue;
            poligons[2]->material = l3PoligonMaterialColor;
            poligons[2]->color = ramiel_blue;
            poligons[3]->material = l3PoligonMaterialColor;
            poligons[3]->color = ramiel_blue;;
            poligons[4]->material = l3PoligonMaterialColor;
            poligons[4]->color = ramiel_blue;
            poligons[5]->material = l3PoligonMaterialColor;
            poligons[5]->color = ramiel_blue;
            poligons[6]->material = l3PoligonMaterialColor;
            poligons[6]->color = ramiel_blue;
            poligons[7]->material = l3PoligonMaterialColor;
            poligons[7]->color = ramiel_blue;;

            poligons[0]->metalness[0] = poligons[1]->metalness[0] = poligons[2]->metalness[0] = poligons[3]->metalness[0]= poligons[4]->metalness[0]= poligons[5]->metalness[0]= poligons[6]->metalness[0]= poligons[7]->metalness[0] = 0.5;
            poligons[0]->metalness[1] = poligons[1]->metalness[1] = poligons[2]->metalness[1] = poligons[3]->metalness[1]= poligons[4]->metalness[1]= poligons[5]->metalness[1]= poligons[6]->metalness[1]= poligons[7]->metalness[1] = 0.5;
            poligons[0]->metalness[2] = poligons[1]->metalness[2] = poligons[2]->metalness[2] = poligons[3]->metalness[2]= poligons[4]->metalness[2]= poligons[5]->metalness[2]= poligons[6]->metalness[2]= poligons[7]->metalness[2] = 0.0005;

            poligons[0]->transparency = poligons[1]->transparency = poligons[2]->transparency = poligons[3]->transparency= poligons[4]->transparency= poligons[5]->transparency= poligons[6]->transparency= poligons[7]->transparency = 0.05;
            
            // poligons[0]->lightType        = poligons[1]->lightType        = poligons[2]->lightType        = poligons[3]->lightType        = poligons[4]->lightType        = poligons[5]->lightType        = poligons[6]->lightType        = poligons[7]->lightType        = l3LightTypeParallel;
            // poligons[0]->lightIntensity   = poligons[1]->lightIntensity   = poligons[2]->lightIntensity   = poligons[3]->lightIntensity   = poligons[4]->lightIntensity   = poligons[5]->lightIntensity   = poligons[6]->lightIntensity   = poligons[7]->lightIntensity   = 0.01;
            // poligons[0]->lightAttenuation = poligons[1]->lightAttenuation = poligons[2]->lightAttenuation = poligons[3]->lightAttenuation = poligons[4]->lightAttenuation = poligons[5]->lightAttenuation = poligons[6]->lightAttenuation = poligons[7]->lightAttenuation = 0.4;

            // テクスチャ読み込み・貼り付け
            // l3Texture texture;
            // l3Load2DTexture("assets/tex4.ppm", &texture);
            // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
            l3SetPoligonsToObject(ramiel_nomal, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(ramiel_nomal, -360, 505, 225);
            l3SetScaleObject(ramiel_nomal, 1, 1, 1);
            l3AddObjectToEnvironment(&env, ramiel_nomal, "ramiel");


        }

        l3Object* obj01 = l3CreateBox();
            for (int i = 0; i < 12; i++) {
            obj01->poligons[i]->material = l3PoligonMaterialColor;
            obj01->poligons[i]->color.r =245;
            obj01->poligons[i]->color.g =245;
            obj01->poligons[i]->color.b =245;
            obj01->poligons[i]->lightColor.r =245;
            obj01->poligons[i]->lightColor.g =245;
            obj01->poligons[i]->lightColor.b =245;
            }          
        {
            l3SetTransposeObject(obj01, 59, 87.5, 34);
            l3SetScaleObject(obj01, 124, 4, 74);
            l3AddObjectToEnvironment(&env, obj01, "roof");
        }

        l3Object* obj02 = l3CreateBox();
            for (int i = 0; i < 12; i++) {
            obj02->poligons[i]->material = l3PoligonMaterialColor;
            obj02->poligons[i]->color.r =255;
            obj02->poligons[i]->color.g =248;
            obj02->poligons[i]->color.b =220;
            obj02->poligons[i]->lightColor.r =255;
            obj02->poligons[i]->lightColor.g =248;
            obj02->poligons[i]->lightColor.b =220;
            }
        {
            l3SetTransposeObject(obj02, 59, 42.5, 34);
            l3SetScaleObject(obj02, 118, 85, 68);
            l3AddObjectToEnvironment(&env, obj02, "mainbox");
        }

        l3Object* obj03 = l3CreateBox();
            for (int i = 0; i < 12; i++) {
            obj03->poligons[i]->material = l3PoligonMaterialColor;
            obj03->poligons[i]->color.r =245;
            obj03->poligons[i]->color.g =245;
            obj03->poligons[i]->color.b =245;
            obj03->poligons[i]->lightColor.r =245;
            obj03->poligons[i]->lightColor.g =245;
            obj03->poligons[i]->lightColor.b =245;
            }        
        {
            l3SetTransposeObject(obj03, 59, 22.5, -6.5);
            l3SetScaleObject(obj03, 76, 3, 12);
            l3AddObjectToEnvironment(&env, obj03, "1hisasi");
        }

        l3Object* obj04 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj04->poligons[i]->material = l3PoligonMaterialColor;
            obj04->poligons[i]->color.r =224;
            obj04->poligons[i]->color.g =255;
            obj04->poligons[i]->color.b =255;
            obj04->poligons[i]->lightColor.r =224;
            obj04->poligons[i]->lightColor.g =255;
            obj04->poligons[i]->lightColor.b =255;
            }
            for (int i = 0; i < 12; i++) {
            obj04->poligons[i]->transparency = 0.5;
            }
        {
            l3SetTransposeObject(obj04, 59.5, 12,-6);
            l3SetScaleObject(obj04, 18, 18, 11);
            l3AddObjectToEnvironment(&env, obj04, "qube");
        }

        l3Object* obj05 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj05->poligons[i]->material = l3PoligonMaterialColor;
            obj05->poligons[i]->color.r =255;
            obj05->poligons[i]->color.g =240;
            obj05->poligons[i]->color.b =245;
            obj05->poligons[i]->lightColor.r =255;
            obj05->poligons[i]->lightColor.g =240;
            obj05->poligons[i]->lightColor.b =245;
            }
        {
            l3SetTransposeObject(obj05, 59, 2.5, -7);
            l3SetScaleObject(obj05, 72, 1, 13);
            l3AddObjectToEnvironment(&env, obj05, "3dan");
        }

        l3Object* obj06 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj06->poligons[i]->material = l3PoligonMaterialColor;
            obj06->poligons[i]->color.r =255;
            obj06->poligons[i]->color.g =240;
            obj06->poligons[i]->color.b =245;
            obj06->poligons[i]->lightColor.r =255;
            obj06->poligons[i]->lightColor.g =240;
            obj06->poligons[i]->lightColor.b =245;
            }
        {
            l3SetTransposeObject(obj06, 59, 1.5, -7.5);
            l3SetScaleObject(obj06, 72, 1, 14);
            l3AddObjectToEnvironment(&env, obj06, "2dan");
        }

        l3Object* obj07 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj07->poligons[i]->material = l3PoligonMaterialColor;
            obj07->poligons[i]->color.r =255;
            obj07->poligons[i]->color.g =240;
            obj07->poligons[i]->color.b =245;
            obj07->poligons[i]->lightColor.r =255;
            obj07->poligons[i]->lightColor.g =240;
            obj07->poligons[i]->lightColor.b =245;
            }
        {
            l3SetTransposeObject(obj07, 59, 0.5, -8);
            l3SetScaleObject(obj07, 72, 1, 15);
            l3AddObjectToEnvironment(&env, obj07, "1dan");
        }

        l3Object* obj08 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj08->poligons[i]->material = l3PoligonMaterialColor;
            obj08->poligons[i]->color.r =248;
            obj08->poligons[i]->color.g =248;
            obj08->poligons[i]->color.b =255;
            obj08->poligons[i]->lightColor.r =248;
            obj08->poligons[i]->lightColor.g =248;
            obj08->poligons[i]->lightColor.b =255;
            }
        {
            l3SetTransposeObject(obj08, 22.5, 4.5, -9);
            l3SetScaleObject(obj08, 2, 9, 17);
            l3AddObjectToEnvironment(&env, obj08, "rightwall");
        }
        l3Object* obj09 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj09->poligons[i]->material = l3PoligonMaterialColor;
            obj09->poligons[i]->color.r =248;
            obj09->poligons[i]->color.g =248;
            obj09->poligons[i]->color.b =255;
            obj09->poligons[i]->lightColor.r =248;
            obj09->poligons[i]->lightColor.g =248;
            obj09->poligons[i]->lightColor.b =255;
            }
        {
            l3SetTransposeObject(obj09, 96.5, 4, -5);
            l3SetScaleObject(obj09, 2, 9,17);
            l3AddObjectToEnvironment(&env, obj09, "reftwall");
        }
        l3Object* obj10 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj10->poligons[i]->material = l3PoligonMaterialColor;
            obj10->poligons[i]->color.r =224;
            obj10->poligons[i]->color.g =255;
            obj10->poligons[i]->color.b =255;
            obj10->poligons[i]->lightColor.r =224;
            obj10->poligons[i]->lightColor.g =255;
            obj10->poligons[i]->lightColor.b =255;
            }
        {
            l3SetTransposeObject(obj10, 59.5, 73.5, -0.5);
            l3SetScaleObject(obj10, 60, 8, 0.5);
            l3AddObjectToEnvironment(&env, obj10, "4mado");
        }
        l3Object* obj11 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj11->poligons[i]->material = l3PoligonMaterialColor;
            obj11->poligons[i]->color.r =224;
            obj11->poligons[i]->color.g =255;
            obj11->poligons[i]->color.b =255;
            obj11->poligons[i]->lightColor.r =225;
            obj11->poligons[i]->lightColor.g =255;
            obj11->poligons[i]->lightColor.b =255;
            }
        {
            l3SetTransposeObject(obj11, 59.5, 53.5, -0.5);
            l3SetScaleObject(obj11, 60, 8, 0.5);
            l3AddObjectToEnvironment(&env, obj11, "3mado");
        }
        l3Object* obj12 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj12->poligons[i]->material = l3PoligonMaterialColor;
            obj12->poligons[i]->color.r =224;
            obj12->poligons[i]->color.g =255;
            obj12->poligons[i]->color.b =255;
            obj12->poligons[i]->lightColor.r =248;
            obj12->poligons[i]->lightColor.g =255;
            obj12->poligons[i]->lightColor.b =255;
            }
        {
            l3SetTransposeObject(obj12, 59.5, 33.5, -0.5);
            l3SetScaleObject(obj12, 60, 8, 0.5);
            l3AddObjectToEnvironment(&env, obj12, "2mado");
        }
        l3Object* obj13 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj13->poligons[i]->material = l3PoligonMaterialColor;
            obj13->poligons[i]->color.r =224;
            obj13->poligons[i]->color.g =255;
            obj13->poligons[i]->color.b =255;
            obj13->poligons[i]->lightColor.r =248;
            obj13->poligons[i]->lightColor.g =255;
            obj13->poligons[i]->lightColor.b =255;
            }
        {
            l3SetTransposeObject(obj13, 38.5, 11.5, -0.5);
            l3SetScaleObject(obj13, 18, 18, 0.5);
            l3AddObjectToEnvironment(&env, obj13, "1madori");
        }
        l3Object* obj14 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj14->poligons[i]->material = l3PoligonMaterialColor;
            obj14->poligons[i]->color.r =224;
            obj14->poligons[i]->color.g =255;
            obj14->poligons[i]->color.b =255;
            obj14->poligons[i]->lightColor.r =224;
            obj14->poligons[i]->lightColor.g =255;
            obj14->poligons[i]->lightColor.b =255;
            }
        {
            l3SetTransposeObject(obj14, 80.5, 11.5, 0.5);
            l3SetScaleObject(obj14, 18, 18, 0.5);
            l3AddObjectToEnvironment(&env, obj14, "1madolef");
        }
     //エアトラ発射台
        l3Object* _hatudai = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, 1,0, &red)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, 0, 1, &green)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, -1, 0, &blue)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, 0, -1, &white)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(1, 1,0, &red)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(1, 0, 1, &green)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(1, 1, 0, &blue)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(1, 0, -1, &white)),
            };
            // 右回りが表、数字はオブジェクトごとの頂点のインデックス
            l3Poligon* poligons[] = {
                l3CreatePoligon(0, 1, 3),
                l3CreatePoligon(1, 2, 3),
                l3CreatePoligon(4, 0, 7),
                l3CreatePoligon(0, 3, 7),
                l3CreatePoligon(0, 4, 5),
                l3CreatePoligon(0, 5, 1),
                l3CreatePoligon(6, 7, 3),
                l3CreatePoligon(6, 3, 2),
                l3CreatePoligon(1, 5, 6),
                l3CreatePoligon(2, 1, 6),
                l3CreatePoligon(4, 7, 5),
                l3CreatePoligon(5, 7, 6),
            };
            for (int i = 0; i < 12; i++) {
            poligons[i]->material = l3PoligonMaterialColor;
            poligons[i]->color.r =189;
            poligons[i]->color.g =195;
            poligons[i]->color.b =201;
            poligons[i]->lightColor.r =189;
            poligons[i]->lightColor.g =195;
            poligons[i]->lightColor.b =201;
            }
            for (int i = 0; i < 12; i++) {
            poligons[i]->metalness[0] = 0.8;
            poligons[i]->metalness[1] = 0.8;
            poligons[i]->metalness[2] = 0.8;
            }
            l3SetPoligonsToObject(_hatudai, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(_hatudai, 60, 95, 35);
            l3SetScaleObject(_hatudai, 50, 5, 5);
            l3AddObjectToEnvironment(&env, _hatudai, "hatudai");
        }

        //エアトラ
        l3Object* _eatora = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(_eatora, l3CreateVertex(-1, 1,0, &red)),
                l3AddVertexToObject(_eatora, l3CreateVertex(-1, 0, 1, &green)),
                l3AddVertexToObject(_eatora, l3CreateVertex(-1, 0, -1, &white)),
                l3AddVertexToObject(_eatora, l3CreateVertex(1, 1,0, &red)),
                l3AddVertexToObject(_eatora, l3CreateVertex(1, 0, 1, &green)),
                l3AddVertexToObject(_eatora, l3CreateVertex(1, 0, -1, &white)),
                l3AddVertexToObject(_eatora, l3CreateVertex(-1, 2, 0, &green)),
                l3AddVertexToObject(_eatora, l3CreateVertex(1, 2, 0, &white)),
            };
            // 右回りが表、数字はオブジェクトごとの頂点のインデックス
            l3Poligon* poligons[] = {
                l3CreatePoligon(3, 0, 5),
                l3CreatePoligon(5, 0, 2),
                l3CreatePoligon(3, 5, 0),
                l3CreatePoligon(5, 2, 0),
                l3CreatePoligon(0, 3, 4),
                l3CreatePoligon(0, 4, 1),
                l3CreatePoligon(0, 4, 3),
                l3CreatePoligon(0, 1, 4),
                l3CreatePoligon(7, 6, 3),
                l3CreatePoligon(3, 6, 0),
                l3CreatePoligon(3, 6, 7),
                l3CreatePoligon(3, 0, 6),
            };

            for (int i = 0; i < 8; i++) {
            poligons[i]->material = l3PoligonMaterialColor;
            poligons[i]->color.r =255;
            poligons[i]->color.g =255;
            poligons[i]->color.b =255;
            poligons[i]->lightColor.r =255;
            poligons[i]->lightColor.g =255;
            poligons[i]->lightColor.b =255;            
            }
            for (int i = 8; i < 12; i++) {
            poligons[i]->material = l3PoligonMaterialColor;
            poligons[i]->color.r =0;
            poligons[i]->color.g =0;
            poligons[i]->color.b =0;
            poligons[i]->lightColor.r =0;
            poligons[i]->lightColor.g =0;
            poligons[i]->lightColor.b =0;            
            }
            for (int i = 0; i < 8; i++) {
            poligons[i]->transparency = 0.5;
            }
            l3SetPoligonsToObject(_eatora, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(_eatora, 100, 95, 35);
            l3SetScaleObject(_eatora, 10, 10, 10);
            l3AddObjectToEnvironment(&env, _eatora, "eatora");
        }

        l3Object* obj3 = l3CreateObject();//地面
        {
            l3AddVertexToObject(obj3, l3CreateVertex(0, 0, 0, &blue));
            l3Mat31A normal = {0, 1, 0};  // 正しい
            l3SetTransposeObject(obj3, 0, 0, 0);
            l3Poligon* poligons[] = {
                l3CreatePoligonPlane(0, normal),
            };
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->color.r = 60;
            poligons[0]->color.g = 60;
            poligons[0]->color.b = 60;
            poligons[0]->metalness[0] = poligons[0]->metalness[1] = poligons[0]->metalness[2] = 0.1;
            poligons[0]->roughness = radians(10);
            poligons[0]->roughnessSamples = 2;

            l3SetPoligonsToObject(obj3, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3AddObjectToEnvironment(&env, obj3, "plane");
        }
        l3Object* sky = l3CreateObject();
        {
            l3Poligon* poligons[] = {
                l3CreatePoligonSky(),
            };
            poligons[0]->color.r = 20;
            poligons[0]->color.g = 20;
            poligons[0]->color.b = 20;
            poligons[0]->lightColor = white;
            poligons[0]->lightType = l3LightTypeParallel;
            poligons[0]->lightIntensity = 1;
            poligons[0]->normal[0] = 1;
            poligons[0]->normal[1] = -1;
            poligons[0]->normal[2] = 1;
            l3NormarizeVec3(poligons[0]->normal, poligons[0]->normal);

            l3SetPoligonsToObject(sky, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3AddObjectToEnvironment(&env, sky, "sky");
        }

        l3SetCameraInfoToEnvironment(&env, 0, 10, -60,
                                     0, 0, 0,
                                     0, 1, 0,
                                     radians(50), 2, 100000);

        l3MultithreadSequentialRenderer(&env, transition, options);
        l3DestructEnvironment(&env);
    }
    return 0;
}