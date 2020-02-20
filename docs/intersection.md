右手座標系を用いる

# レイの照射方向

カメラ座標系で照射方向を考える。最終出力画像の幅と高さ$w,h$が与えられており、$near$よりも遠いところを描画するものとする。

スクリーン座標からカメラ座標への変換行列は次のように書ける。
$$
    [a]_{camera}=\begin{bmatrix}
    2/h & 0 & -w/h \\
    0 & -2/w & 1 \\
    0 & 0 & near
    \end{bmatrix} \begin{bmatrix}
    u\\v\\1
    \end{bmatrix}
$$

これをワールド座標空間へ変換したい。
ワールド座標空間からカメラ座標空間への基底変換行列$P_{w\rightarrow c}$は定義より、ワールド座標空間の基底$(W_x,W_y,W_z)$とカメラ座標空間の基底$(C_x,C_y,C_z)$を用いて次のように表せる。ただし、$C_x,C_y,C_z$はそれぞれカメラ座標空間の$x,y,z$軸方向の単位ベクトルである。
$$
    (C_x,C_y,C_z) = (W_x,W_y,W_z)P_{w\rightarrow c}
$$
$W_x = e_1,W_y = e_2,W_z = e_3$であるので
$$P_{w\rightarrow c} = \begin{bmatrix} C_x & C_y & C_z \end{bmatrix}$$
である。

ここでカメラ座標空間の軸方向の単位ベクトルを求める。

カメラの座標を$P$、視点を$T$、カメラの上方ベクトルを$U$とする。

カメラの座標空間では$z$軸方向が視線の向きにしたいので、
$$C_z = normarize(T-P)$$
である。また、これと$U$に垂直なベクトルが$C_x$なので、
$$C_x = normarize(U \times C_z)$$
$C_y$は$C_x,C_z$に垂直なので、
$$C_y = normarize(C_z \times C_x)$$
となる。

したがって、カメラ座標$P$のとき、スクリーン座標$(u,v)$に対応するレイの始点は、
$$
    \vec{s}=\begin{bmatrix} C_x & C_y & C_z \end{bmatrix} \begin{bmatrix}
    2/h & 0 & -w/h \\
    0 & -2/w & 1 \\
    0 & 0 & near
    \end{bmatrix} \begin{bmatrix}
    u\\v\\1
    \end{bmatrix} + P
$$
であり、レイの方向ベクトルは
$$
    \vec{d} = normarize(s - P)
$$
である。


# レイトレーシングで描画できる図形の条件
レイトレーシングで図形を描画するには次の条件を満たせば良い。
* レイとの交点を求めることができること
* その交点の法線ベクトルを求めることができること

# レイと基本図形の交差判定

レイ上の点はベクトル方程式を用いて
$$\vec{p} = \vec{a} + t\vec{d}$$
と書ける。ただし、$t\gt 0$

## レイと三角ポリゴンの交差判定

3つの頂点$v_0,v_1,v_2$で作られる三角ポリゴンと始点$\vec{a}$、方向ベクトル$\dev{d}$で表されるレイの交点を求める。

ポリゴン上の点は$\vec{e_1} = \vec{v_1} - \vec{v_0}$、$\vec{e_2} = \vec{v_2} - \vec{v_0}$を用いて、
$$
    \vec{p} = \vec{v_0} + u\vec{e_1} + v\vec{e_2}
$$
で表される。ただし、$0 \le u \le 1, \, 0 \le v \le 1, \, u+v \le 1$

レイとポリゴンのベクトル方程式を連立し、$t,u,v$を求める。

$$
    \vec{a} + t\vec{d} = \vec{v_0} + u\vec{e_1} + v\vec{e_2}
$$
移項して、$\vec{r} = \vec{a} - \vec{v_0}$とおいて、
$$
    -t\vec{d} + u\vec{e_1} + v\vec{e_2} = \vec{r}
$$
行列として表すと、
$$
    \begin{bmatrix}
    -\vec{d} & \vec{e_1} & \vec{e_2}
    \end{bmatrix} \begin{bmatrix}
    t \\ u \\ v
    \end{bmatrix} = \vec{r}
$$
クラメールの公式より、
$$
    \begin{bmatrix}
    t \\ u \\ v
    \end{bmatrix} = \frac{1}{\begin{vmatrix} -d & e_1 & e_2\end{vmatrix}} \begin{bmatrix}
    \begin{vmatrix} r & e_1 & e_2 \end{vmatrix} \\
    \begin{vmatrix} -d & r & e_2\end{vmatrix} \\
    \begin{vmatrix} -d & e_1 & r\end{vmatrix}
    \end{bmatrix}
$$
$$
    = \frac{1}{ -d \cdot (e_2 \times e_1)} \begin{bmatrix}
     -r \cdot (e_2 \times e_1)  \\
     e_2 \cdot (r \times d) \\
     -e_1 \cdot (r \times d)
    \end{bmatrix}
$$

求めた$t,u,v$が先の条件を満たすとき、交点の座標は
$$\vec{p} = \vec{a} + t\vec{d}$$
で求めることができる。
なお、$u,v$はテクスチャマッピングのときに利用する。

## レイと球体の交差判定

中心が$\vec{p_c}$の球体と始点$\vec{a}$、方向ベクトル$\dev{d}$($|\vec{d}| = 1$)で表されるレイの交点を求める。


球面上の点は
$$
    |\vec{p} - \vec{p_c}|^2 = r^2
$$
で表される。

レイと球面のベクトル方程式を連立して式変形すると、
$$  
    t^2 + 2Bt + C = 0
$$
ただし、
$$
B = (\vec{s} - \vec{p_c})\cdot \vec{d}
$$
$$
C = |\vec{s}-\vec{p_c}|^2 - r^2
$$

交点を持つということは実数の範囲で解が存在するということだから、判別式$D = B^2 - C$で、$D \ge 0$のときに交点が存在する。ただし、$D = 0$のとき交点が一つだけ存在するが、球体で交点が一つだけ存在するということはレイが球面に接しているということだから、この場合は交点とは考えない。

よって、
$$
t = -B \pm \sqrt{D}
$$
であるが、レイが球体に初めて当たった点を求めたいので、$t$が小さい方を交点として採用する。したがって、レイと球面の交点は
$$
t = -B - \sqrt{D}
$$
のとき、$\vec{p} = \vec{s} + t\vec{d}$である。

## レイと無限平面の交差判定

平面上の点$\vec{p}$は平面上の任意の点$\vec{p_c}$と平面の法線ベクトル$\vec{n}$を用いて
$$
    (\vec{p} - \vec{p_c}) \cdot \vec{n} = 0
$$
と書ける。

レイとこのベクトル方程式を連立して、整理すると、
$$
    t = \frac{(\vec{p_c}-\vec{s})\cdot \vec{n}}{\vec{d}\cdot\vec{n}}
$$
となる。ただし、$t \gt 0$のときのみ交差すると判定する。

