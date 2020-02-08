---
title: ３次元の描画について
---

# バーテックスシェーダ
変換行列は左から掛けるものとし、3次元空間には左手系座標系を用いる。

## ワールド座標変換
拡大縮小・平行移動・回転行列は以下のように求まる。
$$
    S(s) = \begin{bmatrix}
        s_x & 0 & 0 & 0 \\
        0 & s_y & 0 & 0 \\
        0 & 0 & s_z & 0 \\
        0 & 0 & 0 & 1
    \end{bmatrix}
$$
$$
    D(d) = \begin{bmatrix}
        1 & 0 & 0 & d_x \\
        0 & 1 & 0 & d_y \\
        0 & 0 & 1 & d_z \\
        0 & 0 & 0 & 1
    \end{bmatrix}
$$
$$
    R_x(\theta_x) = \begin{bmatrix}
        1 & 0 & 0 & 0 \\
        0 & \cos\theta_x & -\sin\theta_x & 0 \\
        0 & \sin\theta_x & \cos\theta_x & 0 \\
        0 & 0 & 0 & 1
    \end{bmatrix}
$$
$$
    R_y(\theta_y) = \begin{bmatrix}
        \cos\theta_y & 0 & \sin\theta_y & 0 \\
        0 & 1 & 0 & 0 \\
        -\sin\theta_y & 0 & \cos\theta_y & 0 \\
        0 & 0 & 0 & 1
    \end{bmatrix}
$$
$$
    R_z(\theta_z) = \begin{bmatrix}
        \cos\theta_z & -\sin\theta_z & 0 & 0 \\
        \sin\theta_z & \cos\theta_z & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1
    \end{bmatrix}
$$
ワールド座標変換は以下の式で表される。
$$
    [a]_{world} = D(-position) \cdot S(s) \cdot R_z(-\theta_z) \cdot R_y(-\theta_y) \cdot R_x(-\theta_x) \cdot [a]_{local}
$$

## ビュー座標変換
$target$を注視点、$camera$をカメラ座標、$upper$をカメラの上方ベクトルとする。
ビュー座標の基底を以下のように求める。
$$    V_z = normarize(target - camera)    $$
$$    V_x = normarize(V_z \times upper)    $$
$$    V_y = normarize(V_x \times V_z)    $$

ワールド座標空間からビュー座標空間への基底変換行列を用いて次のように書ける。
$$
    (V_x,V_y,V_z) = (W_x,W_y,W_z)P_{W\to V}
$$
ここで、ワールド座標空間の基底は基本ベクトルの組なので、
$$
    P_{W\to V} = [V_x\,V_y\,V_z]
$$

また、ビュー座標空間からワールド座標空間への基底変換行列を用いると、
$$
    [a]_{view} = P_{W\to V} [a]_{world}
$$
（ただし、カメラの位置は原点とする）

$P_{W\to V}$は直交行列であるから、
$$
    P_{V\to W} = P_{W\to V}^{-1} = P_{W\to V}^{\mathrm{T}}
$$

カメラの位置に移動してから基底変換をすれば良いので、
$$
    [a]_{view} = P_{W\to V}^{\mathrm{T}} \cdot D(-camera) \cdot [a]_{world}
$$

## プロジェクション座標変換
やること

* $x,y$それぞれを$-1 \lt x \lt 1,-1 \lt y \lt 1$の範囲に収める
* $x,y$を$z$の値によって拡大縮小する
* $z$を$0 \lt z \lt 1$の範囲に収める
* $w$で割ったら上記の条件を満たすようにする

$$
    \begin{bmatrix}
        x_1 \\ y_1 \\ z_1 \\ w
    \end{bmatrix} = \begin{bmatrix}
    \frac{1}{\tan{(\theta/2)}} & 0 & 0 & 0 \\
    0 & \frac{1}{aspect \cdot \tan{(\theta/2)}} & 0 & 0 \\
    0 & 0 & \frac{far}{far-near} & -\frac{far\cdot near}{far - near} \\
    0 & 0 & 1 & 0
\end{bmatrix} [a]_{view}
$$
$$
    [a]_{clip} = \frac{1}{w} \begin{bmatrix}
        x_1 \\ y_1 \\ z_1 \\ w
    \end{bmatrix}
$$

$$
    z_1 = \frac{z_0}{far-near} - \frac{near}{far - near}
$$
でも、0-1の間に収まる。しかし、$x_1,y_1,z_1,w$の値を一様に$w$で割ろうとすると都合が悪い。
$$
    z_1 = \left(\frac{z_0}{far-near} - \frac{near}{far - near}\right)z_0
$$
としておけば、$w$で割ったときに0~1の範囲に行くが、$z_0$を掛けるのは行列演算ではできないので$far$を掛けて次の式のようにする。
$$
    z_1 = \left(\frac{z_0}{far-near} - \frac{near}{far - near}\right)far
$$
この式の妥当性を証明する。$z_0$で割った後にどうなるかを考えたいので$z_0$で割り、整理して

$$
    F(z) = \frac{z-n}{f-n}\frac{f}{z}
$$
ただし、$z_0 \in R, \, z_0 \gt 0$
$$
    \frac{d F(z)}{d z} = \frac{f}{f-n}\left(\frac{1}{z}+z+n\right) \gt 0
$$
より、単調増加。
$F(z)=1$のとき、$z = f$、$F(z) = 0$のとき、$z = n$であるから、この$F(z)$は$z_1$を求めるのに適している。


## スクリーン座標変換
やること

* 原点を$(x,y) = (-1,1)$の位置に変更する
* $y$を反転する
* $x,y$を画面サイズに引き伸ばす

$$
    [a]_{screen} = \begin{bmatrix}
        w/2 & 0 & 0 & w/2 \\
        0 & -h/2 & 0 & h/2 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1
    \end{bmatrix} [a]_{clip}
$$

# ラスタライザ
プロジェクション座標系において正面から見た2次元のマップを作成する。あるピクセルがポリゴンの範囲内ならば、そのピクセルをそのポリゴンに関連付ける。Z座標の大きい方から描画する。

# フラグメントシェーダ
