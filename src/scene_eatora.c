#include "array.h"
#include "lib3d.h"

 static void transition(l3Environment* env, int frame) {
     l3Object* eatora = l3FindObject(env, "eatora");

    {  // イージング処理集
        if (frame >= 50 && frame <= 150) {
            eatora->dx = 100 - 360 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 50, 150);
        }
        if (frame >= 110 && frame <= 150) {
            eatora->dy = 95 + 360 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 110, 150);
        }
        if (frame >= 130 && frame <= 150) {
            eatora->dz = 35 + 140 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 130, 150);
        }

        if (frame >= 150 && frame <= 200) {
            eatora->dx = -260;
        }
        if (frame >= 150 && frame <= 200) {
            eatora->dy = 455;
        }
        if (frame >= 150 && frame <= 200) {
            eatora->dz = 175;
        }

        if (frame > 180) {
            eatora->ignore = true;
        }
    }
    {
         l3Object* atfield = l3FindObject(env, "atfield");
        if (frame <= 150) {
            for (int i = 0; i < 6; i++) {
                atfield->poligons[i]->transparency = 0.99;
            }
        }
        if (frame >= 150 && frame <= 160) {
            for (int i = 0; i < 6; i++) {
                atfield->poligons[i]->transparency = 0.5 - (frame - 150) / 20.0;
            }
        }
        if (frame >= 180) {
            for (int i = 0; i < 6; i++) {
                atfield->poligons[i]->transparency = max(0, (frame - 180) / 40.0);
            }
        }
    }

    {                          //運動カメラ
        {                      //bezier曲線移動サンプル
             bezier cpoint[4];  //カメラ座標ベジェ
            {
                cpoint[0].x = -100;
                cpoint[0].y = 20;
                cpoint[0].z = -100;
                cpoint[1].x = 125;
                cpoint[1].y = 20;
                cpoint[1].z = -100;
                cpoint[2].x = 125;
                cpoint[2].y = 20;
                cpoint[2].z = -100;
                cpoint[3].x = 175;
                cpoint[3].y = 100;
                cpoint[3].z = 100;
            }

             bezier camera_bezier = l3GetBezierCurve(l3TimeTransition(l3TimeType_EasyEase, frame, 0, 50), 4, cpoint);

            env->camera.coordinate[0] = camera_bezier.x;
            env->camera.coordinate[1] = camera_bezier.y;
            env->camera.coordinate[2] = camera_bezier.z;
        }

        {
             bezier cpoint[4];  //ターゲット座標ベジェ
            {
                cpoint[0].x = 60;
                cpoint[0].y = 40;
                cpoint[0].z = 25;
                cpoint[1].x = 70;
                cpoint[1].y = 60;
                cpoint[1].z = 25;
                cpoint[2].x = 80;
                cpoint[2].y = 95;
                cpoint[2].z = 30;
                cpoint[3].x = 100;
                cpoint[3].y = 95;
                cpoint[3].z = 35;
            }

             bezier cameratarget_bezier = l3GetBezierCurve(l3TimeTransition(l3TimeType_EasyEase, frame, 0, 50), 4, cpoint);

            env->camera.target[0] = cameratarget_bezier.x;
            env->camera.target[1] = cameratarget_bezier.y;
            env->camera.target[2] = cameratarget_bezier.z;
        }

        if (frame >= 50 && frame <= 80) {
            env->camera.coordinate[0] = 175;
        }
        if (frame >= 80 && frame <= 150) {
            env->camera.coordinate[0] = 175 - 360 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 80, 150);
        }
        if (frame >= 150 && frame <= 180) {
            env->camera.coordinate[0] = -185;
        }
        if (frame >= 180 && frame <= 200) {
            env->camera.coordinate[0] = -185 + 300 * l3TimeTransition(l3TimeType_linear, frame, 180, 200);
        }
        if (frame >= 50 && frame <= 110) {
            env->camera.coordinate[1] = 100;
        }
        if (frame >= 110 && frame <= 150) {
            env->camera.coordinate[1] = 100 + 360 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 110, 150);
        }
        if (frame >= 150 && frame <= 200) {
            env->camera.coordinate[1] = 460;
        }
        if (frame >= 50 && frame <= 130) {
            env->camera.coordinate[2] = 100;
        }
        if (frame >= 130 && frame <= 150) {
            env->camera.coordinate[2] = 100 + 140 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 130, 150);
        }
        if (frame >= 150 && frame <= 180) {
            env->camera.coordinate[2] = 240;
        }
        if (frame >= 180 && frame <= 200) {
            env->camera.coordinate[2] = 240 - 200 * l3TimeTransition(l3TimeType_linear, frame, 180, 200);
        }

        if (frame >= 50 && frame <= 150) {
            env->camera.target[0] = 100 - 360 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 50, 150);
        }
        if (frame >= 150 && frame <= 200) {
            env->camera.target[0] = -260;
        }
        if (frame >= 50 && frame <= 110) {
            env->camera.target[1] = 95;
        }
        if (frame >= 110 && frame <= 150) {
            env->camera.target[1] = 95 + 360 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 110, 150);
        }
        if (frame >= 150 && frame <= 200) {
            env->camera.target[1] = 455;
        }
        if (frame >= 50 && frame <= 130) {
            env->camera.target[2] = 35;
        }
        if (frame >= 130 && frame <= 150) {
            env->camera.target[2] = 35 + 140 * l3TimeTransition(l3TimeType_EasyEaseIn, frame, 130, 150);
        }
        if (frame >= 150 && frame <= 200) {
            env->camera.target[2] = 175;
        }
    }

    //テスト用固定カメラ
    // env->camera.coordinate[0] = 100;
    // env->camera.coordinate[1] = 100;
    // env->camera.coordinate[2] = -50;
    // env->camera.target[0] = 50;
    // env->camera.target[1] = 90;
    // env->camera.target[2] = 35;
}

int scene_eatora(int argc, const char* argv[], l3Options* options) {
    l3Environment env;
    {
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

        l3Object* obj01 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj01->poligons[i]->material = l3PoligonMaterialColor;
            obj01->poligons[i]->color.r = 245;
            obj01->poligons[i]->color.g = 245;
            obj01->poligons[i]->color.b = 245;
            obj01->poligons[i]->lightColor.r = 245;
            obj01->poligons[i]->lightColor.g = 245;
            obj01->poligons[i]->lightColor.b = 245;
        }
        {
            l3SetTransposeObject(obj01, 59, 87.5, 34);
            l3SetScaleObject(obj01, 124, 4, 74);
            l3AddObjectToEnvironment(&env, obj01, "roof");
        }

        l3Object* obj02 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj02->poligons[i]->material = l3PoligonMaterialColor;
            obj02->poligons[i]->color.r = 255;
            obj02->poligons[i]->color.g = 248;
            obj02->poligons[i]->color.b = 220;
            obj02->poligons[i]->lightColor.r = 255;
            obj02->poligons[i]->lightColor.g = 248;
            obj02->poligons[i]->lightColor.b = 220;
        }
        {
            l3SetTransposeObject(obj02, 59, 42.5, 34);
            l3SetScaleObject(obj02, 118, 85, 68);
            l3AddObjectToEnvironment(&env, obj02, "mainbox");
        }

        l3Object* obj03 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj03->poligons[i]->material = l3PoligonMaterialColor;
            obj03->poligons[i]->color.r = 245;
            obj03->poligons[i]->color.g = 245;
            obj03->poligons[i]->color.b = 245;
            obj03->poligons[i]->lightColor.r = 245;
            obj03->poligons[i]->lightColor.g = 245;
            obj03->poligons[i]->lightColor.b = 245;
        }
        {
            l3SetTransposeObject(obj03, 59, 22.5, -6.5);
            l3SetScaleObject(obj03, 76, 3, 12);
            l3AddObjectToEnvironment(&env, obj03, "1hisasi");
        }

        l3Object* obj04 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj04->poligons[i]->material = l3PoligonMaterialColor;
            obj04->poligons[i]->color.r = 224;
            obj04->poligons[i]->color.g = 255;
            obj04->poligons[i]->color.b = 255;
            obj04->poligons[i]->lightColor.r = 224;
            obj04->poligons[i]->lightColor.g = 255;
            obj04->poligons[i]->lightColor.b = 255;
        }
        for (int i = 0; i < 12; i++) {
            obj04->poligons[i]->transparency = 0.5;
        }
        {
            l3SetTransposeObject(obj04, 59.5, 12, -6);
            l3SetScaleObject(obj04, 18, 18, 11);
            l3AddObjectToEnvironment(&env, obj04, "qube");
        }

        l3Object* obj05 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj05->poligons[i]->material = l3PoligonMaterialColor;
            obj05->poligons[i]->color.r = 255;
            obj05->poligons[i]->color.g = 240;
            obj05->poligons[i]->color.b = 245;
            obj05->poligons[i]->lightColor.r = 255;
            obj05->poligons[i]->lightColor.g = 240;
            obj05->poligons[i]->lightColor.b = 245;
        }
        {
            l3SetTransposeObject(obj05, 59, 2.5, -7);
            l3SetScaleObject(obj05, 72, 1, 13);
            l3AddObjectToEnvironment(&env, obj05, "3dan");
        }

        l3Object* obj06 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj06->poligons[i]->material = l3PoligonMaterialColor;
            obj06->poligons[i]->color.r = 255;
            obj06->poligons[i]->color.g = 240;
            obj06->poligons[i]->color.b = 245;
            obj06->poligons[i]->lightColor.r = 255;
            obj06->poligons[i]->lightColor.g = 240;
            obj06->poligons[i]->lightColor.b = 245;
        }
        {
            l3SetTransposeObject(obj06, 59, 1.5, -7.5);
            l3SetScaleObject(obj06, 72, 1, 14);
            l3AddObjectToEnvironment(&env, obj06, "2dan");
        }

        l3Object* obj07 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj07->poligons[i]->material = l3PoligonMaterialColor;
            obj07->poligons[i]->color.r = 255;
            obj07->poligons[i]->color.g = 240;
            obj07->poligons[i]->color.b = 245;
            obj07->poligons[i]->lightColor.r = 255;
            obj07->poligons[i]->lightColor.g = 240;
            obj07->poligons[i]->lightColor.b = 245;
        }
        {
            l3SetTransposeObject(obj07, 59, 0.5, -8);
            l3SetScaleObject(obj07, 72, 1, 15);
            l3AddObjectToEnvironment(&env, obj07, "1dan");
        }

        l3Object* obj08 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj08->poligons[i]->material = l3PoligonMaterialColor;
            obj08->poligons[i]->color.r = 248;
            obj08->poligons[i]->color.g = 248;
            obj08->poligons[i]->color.b = 255;
            obj08->poligons[i]->lightColor.r = 248;
            obj08->poligons[i]->lightColor.g = 248;
            obj08->poligons[i]->lightColor.b = 255;
        }
        {
            l3SetTransposeObject(obj08, 22.5, 4.5, -9);
            l3SetScaleObject(obj08, 2, 9, 17);
            l3AddObjectToEnvironment(&env, obj08, "rightwall");
        }
        l3Object* obj09 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj09->poligons[i]->material = l3PoligonMaterialColor;
            obj09->poligons[i]->color.r = 248;
            obj09->poligons[i]->color.g = 248;
            obj09->poligons[i]->color.b = 255;
            obj09->poligons[i]->lightColor.r = 248;
            obj09->poligons[i]->lightColor.g = 248;
            obj09->poligons[i]->lightColor.b = 255;
        }
        {
            l3SetTransposeObject(obj09, 96.5, 4, -5);
            l3SetScaleObject(obj09, 2, 9, 17);
            l3AddObjectToEnvironment(&env, obj09, "reftwall");
        }
        l3Object* obj10 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj10->poligons[i]->material = l3PoligonMaterialColor;
            obj10->poligons[i]->color.r = 224;
            obj10->poligons[i]->color.g = 255;
            obj10->poligons[i]->color.b = 255;
            obj10->poligons[i]->lightColor.r = 224;
            obj10->poligons[i]->lightColor.g = 255;
            obj10->poligons[i]->lightColor.b = 255;
        }
        {
            l3SetTransposeObject(obj10, 59.5, 73.5, -0.5);
            l3SetScaleObject(obj10, 60, 8, 0.5);
            l3AddObjectToEnvironment(&env, obj10, "4mado");
        }
        l3Object* obj11 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj11->poligons[i]->material = l3PoligonMaterialColor;
            obj11->poligons[i]->color.r = 224;
            obj11->poligons[i]->color.g = 255;
            obj11->poligons[i]->color.b = 255;
            obj11->poligons[i]->lightColor.r = 225;
            obj11->poligons[i]->lightColor.g = 255;
            obj11->poligons[i]->lightColor.b = 255;
        }
        {
            l3SetTransposeObject(obj11, 59.5, 53.5, -0.5);
            l3SetScaleObject(obj11, 60, 8, 0.5);
            l3AddObjectToEnvironment(&env, obj11, "3mado");
        }
        l3Object* obj12 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj12->poligons[i]->material = l3PoligonMaterialColor;
            obj12->poligons[i]->color.r = 224;
            obj12->poligons[i]->color.g = 255;
            obj12->poligons[i]->color.b = 255;
            obj12->poligons[i]->lightColor.r = 248;
            obj12->poligons[i]->lightColor.g = 255;
            obj12->poligons[i]->lightColor.b = 255;
        }
        {
            l3SetTransposeObject(obj12, 59.5, 33.5, -0.5);
            l3SetScaleObject(obj12, 60, 8, 0.5);
            l3AddObjectToEnvironment(&env, obj12, "2mado");
        }
        l3Object* obj13 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj13->poligons[i]->material = l3PoligonMaterialColor;
            obj13->poligons[i]->color.r = 224;
            obj13->poligons[i]->color.g = 255;
            obj13->poligons[i]->color.b = 255;
            obj13->poligons[i]->lightColor.r = 248;
            obj13->poligons[i]->lightColor.g = 255;
            obj13->poligons[i]->lightColor.b = 255;
        }
        {
            l3SetTransposeObject(obj13, 38.5, 11.5, -0.5);
            l3SetScaleObject(obj13, 18, 18, 0.5);
            l3AddObjectToEnvironment(&env, obj13, "1madori");
        }
        l3Object* obj14 = l3CreateBox();
        for (int i = 0; i < 12; i++) {
            obj14->poligons[i]->material = l3PoligonMaterialColor;
            obj14->poligons[i]->color.r = 224;
            obj14->poligons[i]->color.g = 255;
            obj14->poligons[i]->color.b = 255;
            obj14->poligons[i]->lightColor.r = 224;
            obj14->poligons[i]->lightColor.g = 255;
            obj14->poligons[i]->lightColor.b = 255;
        }
        {
            l3SetTransposeObject(obj14, 80.5, 11.5, 0.5);
            l3SetScaleObject(obj14, 18, 18, 0.5);
            l3AddObjectToEnvironment(&env, obj14, "1madolef");
        }

        //ATフィールド
        l3Object* _atfield = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(_atfield, l3CreateVertex(0, 1, 0, &red)),
                l3AddVertexToObject(_atfield, l3CreateVertex(0, 0.707107, 0.707107, &green)),
                l3AddVertexToObject(_atfield, l3CreateVertex(0, 0, 1, &blue)),
                l3AddVertexToObject(_atfield, l3CreateVertex(0, -0.707107, 0.707107, &white)),
                l3AddVertexToObject(_atfield, l3CreateVertex(0, -1, 0, &red)),
                l3AddVertexToObject(_atfield, l3CreateVertex(0, -0.707107, -0.707107, &green)),
                l3AddVertexToObject(_atfield, l3CreateVertex(0, 0, -1, &blue)),
                l3AddVertexToObject(_atfield, l3CreateVertex(0, 0.707107, -0.707107, &white)),
            };
            // 右回りが表、数字はオブジェクトごとの頂点のインデックス
            l3Poligon* poligons[] = {
                l3CreatePoligon(0, 3, 2),
                l3CreatePoligon(0, 4, 3),
                l3CreatePoligon(0, 5, 4),
                l3CreatePoligon(0, 6, 5),
                l3CreatePoligon(0, 7, 6),
                l3CreatePoligon(0, 2, 1),
            };
            for (int i = 0; i < 6; i++) {
                poligons[i]->material = l3PoligonMaterialColor;
                poligons[i]->color.r = 255;
                poligons[i]->color.g = 204;
                poligons[i]->color.b = 0;
                poligons[i]->lightColor.r = 255;
                poligons[i]->lightColor.g = 204;
                poligons[i]->lightColor.b = 0;
            }
            l3SetPoligonsToObject(_atfield, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(_atfield, -261, 456, 176);
            l3SetScaleObject(_atfield, 50, 50, 50);
            l3AddObjectToEnvironment(&env, _atfield, "atfield");
        }

        //エアトラ発射台
        l3Object* _hatudai = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, 1, 0, &red)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, 0, 1, &green)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, -1, 0, &blue)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(-1, 0, -1, &white)),
                l3AddVertexToObject(_hatudai, l3CreateVertex(1, 1, 0, &red)),
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
                poligons[i]->color.r = 189;
                poligons[i]->color.g = 195;
                poligons[i]->color.b = 201;
                poligons[i]->lightColor.r = 189;
                poligons[i]->lightColor.g = 195;
                poligons[i]->lightColor.b = 201;
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
                l3AddVertexToObject(_eatora, l3CreateVertex(-1, 1, 0, &red)),
                l3AddVertexToObject(_eatora, l3CreateVertex(-1, 0, 1, &green)),
                l3AddVertexToObject(_eatora, l3CreateVertex(-1, 0, -1, &white)),
                l3AddVertexToObject(_eatora, l3CreateVertex(1, 1, 0, &red)),
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
                poligons[i]->color.r = 255;
                poligons[i]->color.g = 255;
                poligons[i]->color.b = 255;
                poligons[i]->lightColor.r = 255;
                poligons[i]->lightColor.g = 255;
                poligons[i]->lightColor.b = 255;
            }
            for (int i = 8; i < 12; i++) {
                poligons[i]->material = l3PoligonMaterialColor;
                poligons[i]->color.r = 0;
                poligons[i]->color.g = 0;
                poligons[i]->color.b = 0;
                poligons[i]->lightColor.r = 0;
                poligons[i]->lightColor.g = 0;
                poligons[i]->lightColor.b = 0;
            }
            for (int i = 0; i < 8; i++) {
                poligons[i]->transparency = 0.5;
            }
            l3SetPoligonsToObject(_eatora, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(_eatora, 100, 95, 35);
            l3SetScaleObject(_eatora, 10, 10, 10);
            l3AddObjectToEnvironment(&env, _eatora, "eatora");
        }

        l3Object* obj3 = l3CreateObject();
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
        l3Object* ramiel_nomal = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(ramiel_nomal, l3CreateVertex(0, 0, 0, &ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal, l3CreateVertex(0, 200, 0, &ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal, l3CreateVertex(200, 0, 0, &ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal, l3CreateVertex(0, 0, -200, &ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal, l3CreateVertex(0, 0, 200, &ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal, l3CreateVertex(-200, 0, 0, &ramiel_blue)),
                l3AddVertexToObject(ramiel_nomal, l3CreateVertex(0, -200, 0, &ramiel_blue)),
            };

            // 右回りが表、数字はオブジェクトごとの頂点のインデックス
            l3Poligon* poligons[] = {
                l3CreatePoligon(1, 3, 2),
                l3CreatePoligon(1, 2, 4),
                l3CreatePoligon(1, 5, 3),
                l3CreatePoligon(1, 4, 5),
                l3CreatePoligon(6, 2, 3),
                l3CreatePoligon(6, 4, 2),
                l3CreatePoligon(6, 3, 5),
                l3CreatePoligon(6, 5, 4),
            };

            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->color = ramiel_blue;
            poligons[1]->material = l3PoligonMaterialColor;
            poligons[1]->color = ramiel_blue;
            poligons[2]->material = l3PoligonMaterialColor;
            poligons[2]->color = ramiel_blue;
            poligons[3]->material = l3PoligonMaterialColor;
            poligons[3]->color = ramiel_blue;
            ;
            poligons[4]->material = l3PoligonMaterialColor;
            poligons[4]->color = ramiel_blue;
            poligons[5]->material = l3PoligonMaterialColor;
            poligons[5]->color = ramiel_blue;
            poligons[6]->material = l3PoligonMaterialColor;
            poligons[6]->color = ramiel_blue;
            poligons[7]->material = l3PoligonMaterialColor;
            poligons[7]->color = ramiel_blue;

            for (int i = 0; i < 8; i++) {
                poligons[i]->lightType = l3LightTypePoint;
                poligons[i]->lightIntensity = 0.2;
                poligons[i]->lightColor = ramiel_blue;
                poligons[i]->lightAttenuation = 0.01;
            }

            poligons[0]->metalness[0] = poligons[1]->metalness[0] = poligons[2]->metalness[0] = poligons[3]->metalness[0] = poligons[4]->metalness[0] = poligons[5]->metalness[0] = poligons[6]->metalness[0] = poligons[7]->metalness[0] = 0.5;
            poligons[0]->metalness[1] = poligons[1]->metalness[1] = poligons[2]->metalness[1] = poligons[3]->metalness[1] = poligons[4]->metalness[1] = poligons[5]->metalness[1] = poligons[6]->metalness[1] = poligons[7]->metalness[1] = 0.5;
            poligons[0]->metalness[2] = poligons[1]->metalness[2] = poligons[2]->metalness[2] = poligons[3]->metalness[2] = poligons[4]->metalness[2] = poligons[5]->metalness[2] = poligons[6]->metalness[2] = poligons[7]->metalness[2] = 0.0005;

            poligons[0]->transparency = poligons[1]->transparency = poligons[2]->transparency = poligons[3]->transparency = poligons[4]->transparency = poligons[5]->transparency = poligons[6]->transparency = poligons[7]->transparency = 0.05;

            // poligons[0]->lightType        = poligons[1]->lightType        = poligons[2]->lightType        = poligons[3]->lightType        = poligons[4]->lightType        = poligons[5]->lightType        = poligons[6]->lightType        = poligons[7]->lightType        = l3LightTypeParallel;
            // poligons[0]->lightIntensity   = poligons[1]->lightIntensity   = poligons[2]->lightIntensity   = poligons[3]->lightIntensity   = poligons[4]->lightIntensity   = poligons[5]->lightIntensity   = poligons[6]->lightIntensity   = poligons[7]->lightIntensity   = 0.01;
            // poligons[0]->lightAttenuation = poligons[1]->lightAttenuation = poligons[2]->lightAttenuation = poligons[3]->lightAttenuation = poligons[4]->lightAttenuation = poligons[5]->lightAttenuation = poligons[6]->lightAttenuation = poligons[7]->lightAttenuation = 0.4;

            // テクスチャ読み込み・貼り付け
            // l3Texture texture;
            // l3Load2DTexture("assets/tex4.ppm", &texture);
            // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
            l3SetPoligonsToObject(ramiel_nomal, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(ramiel_nomal, -360, 505, 225);
            l3SetScaleObject(ramiel_nomal, 0.5, 0.5, 0.5);
            l3AddObjectToEnvironment(&env, ramiel_nomal, "ramiel");
        }
        l3Object* light1 = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(light1, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(light1, l3CreateVertex(0, 100, 0, &red)),
            };
            l3Poligon* poligons[] = {
                l3CreatePoligonSphere(0, 1, 100),
            };
            poligons[0]->lightColor.r = 255;
            poligons[0]->lightColor.g = 255;
            poligons[0]->lightColor.b = 255;
            poligons[0]->material = l3PoligonMaterialColor;
            poligons[0]->lightType = l3LightTypePoint;
            poligons[0]->lightIntensity = 0.5;
            poligons[0]->noSize = true;
            // poligons[0]->transparency = 0.5;
            // poligons[0]->lightAttenuation = 0.0004;

            l3SetPoligonsToObject(light1, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(light1, -3000, 1000, -3000);
            l3AddObjectToEnvironment(&env, light1, "light1");
        }

        l3Object* light2 = l3CloneObject(light1);
        {
            light2->poligons[0]->lightColor.r = 255;
            light2->poligons[0]->lightColor.g = 255;
            light2->poligons[0]->lightColor.b = 255;
            light2->poligons[0]->transparency = 0.01;
            light2->poligons[0]->lightType = l3LightTypePoint;

            light2->poligons[0]->lightIntensity = 1;

            l3SetTransposeObject(light2, 800, 1000, 300);
            l3AddObjectToEnvironment(&env, light2, "light2");
        }

        l3Object* light3 = l3CloneObject(light1);
        {
            light3->poligons[0]->lightColor.r = 255;
            light3->poligons[0]->lightColor.g = 255;
            light3->poligons[0]->lightColor.b = 255;
            light3->poligons[0]->transparency = 0.01;

            light3->poligons[0]->lightType = l3LightTypePoint;
            light3->poligons[0]->lightIntensity = 1;

            l3SetTransposeObject(light3, 50, 1200, 100);
            l3AddObjectToEnvironment(&env, light3, "light3");
        }
        l3SetCameraInfoToEnvironment(&env, 0, 20, -60,
                                     0, 0, 0,
                                     0, 1, 0,
                                     radians(50), 2, 100000);

        env.environmentColor = white;
        env.environmentLightRate = 0.1;
        env.environmentLightIntensity = 1;

        l3MultithreadSequentialRenderer(&env, transition, options);
        // l3MultithreadRenderer(&env, options->renderer, transition, options->frames, options->threads);
        // l3MultithreadSequentialRenderer(&env, l3RasterizingRenderer, transition, 100, 16);

        l3DestructEnvironment(&env);
    }
    return 0;
}