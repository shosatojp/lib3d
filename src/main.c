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
 * main.cはGitには上げないようにしてください。
 */

int main(int argc, const char* argv[]) {
    scene_core(argc, argv);
    // scene_bezier(argc, argv);
}