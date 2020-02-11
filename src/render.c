#include "lib3d.h"

void l3RenderEnvironment(l3Environment* env) {
    // ラスタマップとバッファーを作る
    l3PixelInfo* map = l3CreateRasterMap(env->w, env->h);
    unsigned char* buf = l3CreateBuffer(env->w, env->h);

    for (int f = env->frame_begin; f < env->frame_end; f++) {
        printf("frame = %d\n", f);
        l3ClearRasterMap(map, env->w, env->h);
        l3ClearBuffer(buf, env->w, env->h, 255);

        /* 頂点のインデックスを実ポインタに変換 */
        l3SolvePtrsEnvironment(env);

        /* 動かす */
        env->transitionFn(env, f);

        /* 座標変換のあと、ここにポリゴンを集めて・・・ */
        array* poligons_all = array_new(sizeof(l3Poligon*), true, 10);

        // 変換行列作成
        l3Type wc[16] = {0}, cp[16] = {0}, ps[16] = {0}, wcps[16] = {0};
        l3MakeWorldToCameraMat44(&env->camera, wc);

        l3MakeCameraToProjectionMat44(120, (double)env->w / env->h, 10, 100, cp);
        l3MakeProjectionToScreenMat44(env->w, env->h, ps);

        l3Mat44 mats[] = {ps, cp, wc};
        l3MulMat44s44(3, mats, wcps);

        /* Environmentに入ってるオブジェクトをすべて描画 */
        l3AppendPoligonsFromEnvironment(env, wcps, env->w, env->h, poligons_all);

        /* Z座標でソートして・・・ */
        l3SortPoligonsByMaxZ(poligons_all->length, (l3Poligon**)poligons_all->data);

        /* まとめて描画 */
        array_each_i(poligons_all, l3WriteRasterMap(map, env->w, env->h, array_ei));
        array_clear(poligons_all);
        safe_free(poligons_all);

        // 出力
        l3ConvertRasterMapToBuffer(map, buf, env->w, env->h);
        char name[100] = {0};
        sprintf(name, "%s/%06d.ppm", env->outdir, f);
        l3WriteBuffer(buf, env->w, env->h, name);

        // 初期化
        l3ClearEnvironment(env);
    }
    safe_free(buf);
    safe_free(map);
}
