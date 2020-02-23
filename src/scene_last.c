//最後のシーン

#include "lib3d.h"

static void transition(l3Environment* env, int frame) {
    l3Object* ramiel_nomal = l3CreateObject(env, "ramiel");

    env->camera.coordinate[0] = -1000;
    env->camera.coordinate[1] = 800 - frame;
    env->camera.coordinate[2] = -250;

}

int scene_last(int argc, const char* argv[], l3Options* options) {
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

l3Object* b01 = l3CreateBox();
        {
            // テクスチャ読み込み
            //l3Texture texture;
            //l3Load2DTexture("assets/building.ppm", &texture);
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
        double i,j,s,t;
        for(i = 0; i < 7; ++i){
            double n = (i+1.0)*10.0;
            for(j = 0; j < n; ++j){
                l3Object* b02 = l3CloneObject(b01);//クローン
                {
                    s = (i+5.0)*80*cos(j / n * 2.0 * PI);
                    t = (i+5.0)*80*sin(j / n * 2.0 * PI);
                    if(t < -400 || 400 < t){
                        l3SetTransposeObject(b01, s , 20.0, t);
                        l3SetScaleObject(b01, 20 + (rand()%5)*5, 50 + (rand()%10)*20, 20 + (rand()%5)*5);
                        l3AddObjectToEnvironment(&env, b02, "box02");
                    }
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
        l3Object* column = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(column, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(column, l3CreateVertex(0, 20, 0, &red)),
            };
            l3Poligon* poligons[] = {
                l3CreatePoligonColumn(0, 1, 10),
            };
            poligons[0]->color.r = 255;
            poligons[0]->color.g = 250;
            poligons[0]->color.b = 50;
            poligons[0]->material = l3PoligonMaterialColor;

            l3SetPoligonsToObject(column, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(column, 0, 0, 0);
            // l3AddObjectToEnvironment(&env, column, "column");
        }
        //l3Texture texture2;
        //l3Load2DTexture("assets/star.ppm", &texture2);

        l3Object* sphere = l3CreateObject();
        {
            int vs[] = {
                l3AddVertexToObject(sphere, l3CreateVertex(0, 0, 0, &red)),
                l3AddVertexToObject(sphere, l3CreateVertex(0, 10, 0, &red)),//上方向
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
            // l3Mat32A texture_vertices = {0.5, 0.5, 0, 1, 1, 1};
            // poligons[0]->textureType = l3TextureTypeTiled;
            // poligons[0]->textureRepeatX = 10;
            // poligons[0]->textureRepeatY = 10;
            // poligons[0]->textureCoordinateSystem = l3CoordinateSystemLocal;
            // poligons[0]->texture = &texture2;
            // poligons[0]->normal[0] = 0;
            // poligons[0]->normal[1] = 1;
            // poligons[0]->normal[2] = 0;

            l3SetPoligonsToObject(sphere, sizeof(poligons) / sizeof(l3Poligon*), poligons);
            l3SetTransposeObject(sphere, 0, 0, 0);
            //l3AddObjectToEnvironment(&env, sphere, "sphere");
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