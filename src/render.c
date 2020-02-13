#include "lib3d.h"

static int frame_count = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void l3MultithreadRenderer(l3Environment* env, l3Renderer* renderer,l3FrameTransitionFunction* transitionFn,  int frames, int thread_count) {
    printf("starting multithreaded rendering...\n");
    printf("thread count : %d\n", thread_count);
    printf("frame count  : %d\n", frames);

    time_t s, f;
    time(&s);

    int frame_per_thread = frames / thread_count;
    int amari = frames % thread_count;
    int current_frame = 0;
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * thread_count);

    for (int i = 0; i < thread_count; i++) {
        l3Environment* _env = l3CloneEnvironment(env);
        _env->frame_begin = frame_per_thread * i + min(amari, i);
        _env->frame_end = frame_per_thread * (i + 1) + min(amari, i + 1);
        printf("thread %d: %d - %d\n", i, _env->frame_begin, _env->frame_end);
        _env->transitionFn = transitionFn;
        pthread_create(&threads[i], NULL, (void* (*)(void*))renderer, _env);
    }
    // 片付け
    for (int i = 0; i < thread_count; i++) {
        int result = pthread_join(threads[i], NULL);
    }

    time(&f);
    free(threads);
    printf("rendering finished successfully.\ntotal: %d frames, %ld s, %.3f s/frame\n", frame_count, f - s, (double)(f - s) / frames);
}

void l3RaytracingRenderer(l3Environment* env) {
    // バッファーを作る
    unsigned char* buf = l3CreateBuffer(env->w, env->h);

    // 各フレームについて
    // solve ptrs
    l3SolvePtrsEnvironment(env);

    //for begin
    for (int f = env->frame_begin; f < env->frame_end; f++) {
        printf("rendering frame %d\n", f);
        frame_count++;
        l3ClearBuffer(buf, env->w, env->h, 255);
        /* 動かす */
        env->transitionFn(env, f);
        // ワールド座標を設定
        l3SetWorldCoordinate(env);
        // transitionFn
        // 出力
        // 初期化
        l3Mat33A p_wtoc = {0};
        l3MakeWorldToCameraBasisChangeMat33(&env->camera, p_wtoc);

        for (size_t j = 0; j < env->h; j++) {
            for (size_t i = 0; i < env->w; i++) {
                l3Ray ray = {0};
                l3GetRayStartPointAndDirection(p_wtoc, env->camera.coordinate,
                                               env->camera.angle, env->w, env->h, i, j,
                                               ray.rayStartPoint, ray.rayDirection);
                l3TraceRay(&ray, env, 0);

                // バッファ(i,j)に色を設定
                l3SET_BUFFER_RGB(buf, env->w, env->h, i, j, ray.color);
            }
        }

        // PPMに出力
        char name[100] = {0};
        sprintf(name, "%s/%06d.ppm", env->outdir, f);
        l3WriteBuffer(buf, env->w, env->h, name);

        l3ClearEnvironment(env);
    }
    safe_free(buf);
    l3DestructEnvironment(env);
    safe_free(env);
}

void l3RasterizingRenderer(l3Environment* env) {
    // ラスタマップとバッファーを作る
    l3PixelInfo* map = l3CreateRasterMap(env->w, env->h);
    unsigned char* buf = l3CreateBuffer(env->w, env->h);

    /* 頂点のインデックスを実ポインタに変換 */
    l3SolvePtrsEnvironment(env);

    env->map = map;
    for (int f = env->frame_begin; f < env->frame_end; f++) {
        printf("rendering frame %d\n", f);

        // ロックしないでも行けそう（遅くなる）
        frame_count++;

        // fflush(stdout);
        l3ClearRasterMap(map, env->w, env->h);
        l3ClearBuffer(buf, env->w, env->h, 255);

        /* 動かす */
        env->transitionFn(env, f);

        // 変換行列作成
        l3Type wc[16] = {0}, cp[16] = {0}, ps[16] = {0}, wcps[16] = {0};
        l3MakeWorldToCameraMat44(&env->camera, wc);

        l3MakeCameraToProjectionMat44(env->camera.angle, (double)env->w / env->h,
                                      env->camera.near, env->camera.far, cp);
        l3MakeProjectionToScreenMat44(env->w, env->h, ps);

        l3Mat44 mats[] = {ps, cp, wc};
        l3MulMat44s44(3, mats, wcps);

        /* Environmentに入ってるオブジェクトをすべて描画 */
        l3AppendPoligonsFromEnvironment(env, wcps, env->w, env->h);

        /* Z座標でソートして・・・ */
        l3SortPoligonsByMaxZ(env->poligons.length, (l3Poligon**)env->poligons.data);

        /* まとめて描画 */
        array_each_i(&env->poligons, l3WriteRasterMap(env, env->w, env->h, array_ei));

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
