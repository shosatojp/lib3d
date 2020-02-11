#include "lib3d.h"

void l3MultithreadRenderer(l3Environment* env, l3FrameTransitionFunction* transitionFn, int frames, int thread_count) {
    printf("starting multithreaded rendering...\n");
    printf("thread count : %d\n", thread_count);
    printf("frame count  : %d\n", frames);

    time_t s, f;
    time(&s);

    int frame_per_thread = frames / thread_count;
    int current_frame = 0;
    pthread_t** threads = (pthread_t**)malloc(sizeof(pthread_t*) * thread_count);

    for (int i = 0; i < thread_count; i++) {
        pthread_t thread;
        l3Environment* _env = l3CloneEnvironment(env);
        _env->frame_begin = current_frame;
        _env->frame_end = current_frame + min(frame_per_thread, frames - current_frame);
        _env->transitionFn = transitionFn;
        current_frame += frame_per_thread;
        pthread_create(&thread, NULL, (void* (*)(void*))l3RenderEnvironment, _env);
        threads[i] = &thread;
    }

    // 片付け
    while (thread_count) pthread_join(*threads[--thread_count], NULL);
    free(threads);

    time(&f);
    printf("rendering finished successfully.\ntotal: %d frames, %ld s, %.3f s/frame\n", frames, f - s, (double)(f - s) / frames);
}

void l3RenderEnvironment(l3Environment* env) {
    // ラスタマップとバッファーを作る
    l3PixelInfo* map = l3CreateRasterMap(env->w, env->h);
    unsigned char* buf = l3CreateBuffer(env->w, env->h);

    for (int f = env->frame_begin; f < env->frame_end; f++) {
        printf("rendering frame %d\n", f);
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
    l3DestructEnvironment(env);
    safe_free(env);
}
