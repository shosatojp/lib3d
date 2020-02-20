# テクスチャ

## 三角ポリゴンへのテクスチャマッピング

三角ポリゴンの頂点を順に$a,b,c$とすると、レイとポリゴンの交差判定時に$e_1 = b-a$、$e_2 = c-a$を用いて、$p = a + ue_1 + ve_2$となる$u,v$を求めてあるので、これを用いてテクスチャマッピングを行うことを考える。しかし、$e_1,e_2$は直交するとは限らないのでこれを用いるとテクスチャが歪んでしまう。そこでこの$uv$座標空間の基底$(e_1,e_2)$の座標$(u,v)$を適当な直交座標に変換する必要がある。直交座標系の$u'v'$座標空間の基底$(e_1',e_2')$をポリゴンの法線ベクトル$n$を用いて次のように設定する。しかし、基準が法線ベクトルしか無いと向きが定まらないので、ローカル座標またはワールド座標の$x$軸方向の単位ベクトルをもう一つの基準に設定する。

$$e_1' = n \times axis_x$$
$$e_2' = e_1' \times n$$

基底変換行列$P_{uv \rightarrow u'v'}$は定義より、
$$
    (e_1',e_2',n) = (e_1,e_2,n)P_{uv \rightarrow u'v'}
$$
を満たすから、行列
$$
    \begin{bmatrix}
    e_1' & e_2' & n & e_1 & e_2 & n
    \end{bmatrix}
$$
を簡約化し、$normal$方向の値は必要ないので、$(1,4)$から$(2,5)$を$2\times2$行列の$P_{uv \rightarrow u'v'}$とする。

この基底変換行列を用いて$uv$座標をテクスチャ座標に変換する式は次のように書ける。ただし、$w,h$はテクスチャの幅と高さ、$s_x,s_y$はテクスチャの繰り返し回数、$\theta$は回転角である。

$$
    T' = [a]_{texture}' = \begin{bmatrix}
    w & 0 \\
    0 & h
    \end{bmatrix} \begin{bmatrix}
    s_x & 0 \\
    0 & s_y
    \end{bmatrix} \begin{bmatrix}
    \cos\theta & -\sin\theta \\
    \sin\theta & \cos\theta
    \end{bmatrix} P_{uv \rightarrow u'v'} [a]_{uv}
$$

$T'$はテクスチャの範囲外の値も取るのでこれをテクスチャの範囲内に収める必要がある。これは次の式で行う。

$$
    T_x = T_x' - w \left\lfloor \frac{T_x'}{w} \right\rfloor
$$
$$
    T_y = T_y' - h \left\lfloor \frac{T_y'}{h} \right\rfloor
$$

したがって、ポリゴン内の座標$(u,v)$の物体の色はテクスチャの座標$T$の色を用いれば良い。