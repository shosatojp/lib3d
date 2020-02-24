#include "lib3d.h"
#include "scenes.h"
#include "threadpool.h"

/**
 * シーンファイルの分け方
 * みんなでmain.cを編集するとマージが大変というか無理なので
 * シーンごとにファイル分けします。
 * 
 * scene_hogehoge.cを作成し、
 * その中にstatic void transition()と
 * void scene_hogehoge()関数を作ります。
 * 
 * scenes.hに#include "scene_hogehoge.c"を書き加えます。
 * 
 * main()に実行するためのコードを書きます。
 * 
 * main.cはGitには上げないようにしてください。
 */

int main(int argc, const char* argv[]) {
    // threadpool_test();
    // // voxel_test();
    // exit(0);
    // default options
    l3Options options = {0};
    options.outdir = "bin";
    options.w = 480;
    options.h = 320;
    options.frame_begin = 0;
    options.frames = 1;
    options.threads = 1;
    options.renderer = l3RaytracingRenderer;
    options.prefix = "";
    options.block_x = 1;
    options.block_y = 1;
    if (!l3ParseArgs(argc, argv, &options)) exit(0);

    // voxel_test();

    // scene_core(argc, argv, &options);
    scene_eatora(argc, argv, &options);
    // scene_bezier(argc, argv, &options);
    // scene_ramiel(argc, argv, &options);
    // scene_start(argc, argv, &options);
    //scene_last(argc, argv, &options);
}
