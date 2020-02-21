# 3Dを描画するライブラリ

## 使用方法
```cpp
// lib3d.hをインクルードする
#include "lib3d.h"
```

## サンプル動画

![](sample.gif)

### コンパイル
```sh
make
make -j8
```

### gif生成
```sh
make gif
```

### mp4生成
```sh
make mp4
```

### 片付け
```sh
make clean
```

## APIの使い方
### テクスチャ
```cpp
// テクスチャ読み込み **Objectの直後の{}に入れてはいけない**
l3Texture texture;
// convertコマンド等でppmに変換して使用する
l3Load2DTexture("assets/dot.ppm", &texture);

//==============================================
//=============== タイルテクスチャ=================
//===============================================
// テクスチャの貼り付け方法。
// ここをみてテクスチャを使用するか判断しているので、
// デバッグ時にテクスチャを使用しない場合は
// 以下の設定はそのままでここだけ設定変えれば良い
poligons[0]->textureType = l3TextureTypeTiled;
// テクスチャの繰り返し回数(XY違う値でも良い)
poligons[0]->textureRepeatX = 10;
poligons[0]->textureRepeatY = 10;
// 基準となる座標系を指定。
// Localだと物体が動いてもテクスチャは張り付いたまま
// Worldにするとワールド座標に対して固定
poligons[0]->textureCoordinateSystem = l3CoordinateSystemLocal;
// テクスチャのポインタを指定
poligons[0]->texture = &texture;

//==============================================
//=============== UVマッピング====================
//===============================================
// UV座標(0.0-1.0, この範囲外を指定したときはpoligon.colorの色が描画される)
// ポリゴン作成時の頂点の順番にu,v,u,v,u,vを指定
l3Mat32A texture_vertices = {
    0.5, 0,
    0, 0.8,
    1, 0.8
};

// UVテクスチャを貼り付け
l3SetUVTexturePoligon(poligons[i], &texture, texture_vertices);
//==============================================

// textureTypeに指定できる値
typedef enum _l3TextureType {
    l3TextureTypeNone,
    l3TextureTypeTiled,
    l3TextureTypeUV,
} l3TextureType;

// textureCoordinateSystemに指定できる値
typedef enum _l3CoordinateSystem {
    l3CoordinateSystemLocal,
    l3CoordinateSystemWorld,
} l3CoordinateSystem;
```
