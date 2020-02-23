#include "lib3d.h"

static int frame_count = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void l3MultithreadRenderer(l3Environment* env, l3Renderer* renderer, l3FrameTransitionFunction* transitionFn, int frames, int thread_count) {
    printf("starting multithreaded rendering...\n");
    thread_count = min(thread_count, frames);
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

void l3MultithreadSequentialRenderer(l3Environment* env,
                                     l3FrameTransitionFunction* transitionFn,
                                     l3Options* options) {
    printf("starting multithreaded sequential rendering...\n");
    int thread_count = min(options->threads, options->frames);
    printf("thread count : %d\n", thread_count);
    printf("frame count  : %d\n", options->frames);

    time_t s, f;
    time(&s);

    int frame_per_thread = options->frames / thread_count;
    int amari = options->frames % thread_count;
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * thread_count);

    for (int i = 0; i < thread_count; i++) {
        l3Environment* _env = l3CloneEnvironment(env);
        _env->thread_count = thread_count;
        _env->frame_begin = options->frame_begin + i;
        _env->frame_end = options->frame_begin + options->frames;
        _env->transitionFn = transitionFn;
        _env->renderType = l3MultiThreadRenderingTypeSequential;
        _env->prefix = options->prefix;
        printf("thread %d: %d - %d (every %d frames)\n", i, _env->frame_begin, _env->frame_end, _env->thread_count);
        pthread_create(&threads[i], NULL, (void* (*)(void*))options->renderer, _env);
    }
    // 片付け
    for (int i = 0; i < thread_count; i++) {
        int result = pthread_join(threads[i], NULL);
    }

    time(&f);
    free(threads);
    printf("rendering finished successfully.\ntotal: %d frames, %ld s, %.3f s/frame\n", frame_count, f - s, (double)(f - s) / options->frames);
    printf("out dir: %s\n", options->outdir);
}
#define l3ANTI_ALIASING_RAYS_COUNT 6
// #define l3ANTI_ALIASING_ENABLED

void l3RaytracingRenderer(l3Environment* env) {
    // バッファーを作る
    unsigned char* buf = l3CreateBuffer(env->w, env->h);

    // 各フレームについて
    // solve ptrs
    l3SolvePtrsEnvironment(env);

#ifdef l3ANTI_ALIASING_ENABLED
    // アンチエイリアス用Rayの回転行列
    l3Type anti_aliasing_max_degree = 0.1;
    // l3Mat44A anti_aliasing_ray_rotate_mats[l3ANTI_ALIASING_RAYS_COUNT];
    l3Mat44 anti_aliasing_ray_rotate_mats[l3ANTI_ALIASING_RAYS_COUNT];
    for (size_t i = 0; i < l3ANTI_ALIASING_RAYS_COUNT; i++) {
        l3Ray antieilias_ray = {0};
        l3Mat44A rx = {0};
        l3MakeRoundXMat44(radians((l3Type)rand() / (l3Type)RAND_MAX * anti_aliasing_max_degree), rx);
        l3Mat44A ry = {0};
        l3MakeRoundYMat44(radians(rand() / (l3Type)RAND_MAX * anti_aliasing_max_degree), ry);
        l3Mat44A rz = {0};
        l3MakeRoundZMat44(radians(rand() / (l3Type)RAND_MAX * anti_aliasing_max_degree), rz);
        l3Mat44 rmats[] = {rx, ry, rz};
        l3Mat44A r = {0};
        l3MulMat44s44(3, rmats, r);
        anti_aliasing_ray_rotate_mats[i] = r;
    }
#endif

    for (int i = 0; i < env->frame_begin; i++) {
        env->transitionFn(env, i);
    }

    //for begin
    for (int f = env->frame_begin, e = env->frame_end; f < e; f++) {
        /* 動かす */
        env->transitionFn(env, f);
        if (env->renderType == l3MultiThreadRenderingTypeSequential &&
            (f - env->frame_begin) % env->thread_count) {
            continue;
        } else {
            printf("rendering frame %d\n", f);
            frame_count++;
            l3ClearBuffer(buf, env->w, env->h, 255);
        }
        // ワールド座標を設定
        l3SetWorldCoordinate(env);

        l3Mat33A p_wtoc = {0};
        l3MakeWorldToCameraBasisChangeMat33(&env->camera, p_wtoc);

        for (int j = 0, h = env->h; j < h; j++) {
            for (int i = 0, w = env->w; i < w; i++) {
                l3RGB sumcolor = {0};
                l3Ray ray = {0};
                l3GetRayStartPointAndDirection(p_wtoc, env->camera.coordinate,
                                               env->camera.near, env->w, env->h, i, j,
                                               ray.rayStartPoint, ray.rayDirection);
                l3TraceRay(&ray, env, 0);
                l3AddColor(sumcolor, ray.color);

#ifdef l3ANTI_ALIASING_ENABLED
                // アンチエイリアス
                for (int i = 0; i < l3ANTI_ALIASING_RAYS_COUNT; i++) {
                    l3Ray antieilias_ray = {0};
                    l3MulMat(anti_aliasing_ray_rotate_mats[i], ray.rayDirection, antieilias_ray.rayDirection, 4, 4, 1);
                    memcpy(antieilias_ray.rayStartPoint, ray.rayStartPoint, sizeof(l3Type) * 3);
                    l3TraceRay(&antieilias_ray, env, 0);
                    l3AddColor(sumcolor, antieilias_ray.color);
                }
                l3DivColor(sumcolor, l3ANTI_ALIASING_RAYS_COUNT + 1);
#endif

                // バッファ(i,j)に色を設定
                l3SET_BUFFER_RGB(buf, env->w, env->h, i, j, sumcolor);
            }
        }

        // 2D処理
        // bufの中身を変更

        // PPMに出力
        char name[100] = {0};
        sprintf(name, "%s/%s%06d.ppm", env->outdir, env->prefix, f);
        l3WriteBuffer(buf, env->w, env->h, name);

        // 初期化
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
