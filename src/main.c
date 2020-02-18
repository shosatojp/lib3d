#include "lib3d.h"
#include "scenes.h"

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
 * main.cはGitには上げないようにしてください。ahhhh
 */

int main(int argc, const char* argv[]) {
    // default options
    l3Options options = {0};
    options.outdir = "bin";
    options.h = 480;
    options.w = 320;
    options.frame_begin = 0;
    options.frames = 1;
    options.threads = 1;
    options.renderer = l3RaytracingRenderer;
    if (!l3ParseArgs(argc, argv, &options)) exit(0);

    scene_core(argc, argv, &options);
    // scene_bezier(argc, argv);
}