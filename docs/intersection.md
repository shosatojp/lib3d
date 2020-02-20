# レイトレーシングで描画できる図形の条件
レイトレーシングで図形を描画するには次の条件を満たせば良い。
* レイとの交点を求めることができること
* その交点の法線ベクトルを求めることができること

# レイと基本図形の交差判定

## レイと三角ポリゴンの交差判定

3つの頂点$v_0,v_1,v_2$で作られる三角ポリゴンと始点$\vec{a}$、方向ベクトル$\dev{d}$で表されるレイの交点を求める。

レイ上の点はベクトル方程式を用いて
$$\vec{p} = \vec{a} + t\vec{d}$$
と書ける。ただし、$t\gt 0$

ポリゴン上の点は$\vec{e_1} = \vec{v_1} - \vec{v_0}$、$\vec{e_2} = \vec{v_2} - \vec{v_0}$を用いて、
$$
    \vec{p} = \vec{v_0} + u\vec{e_1} + v\vec{e_2}
$$
で表される。ただし、$0 \le u \le 1, \, 0 \le v \le 1, \, u+v \le 1$

この２つのベクトル方程式を連立し、$t,u,v$を求める。

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

レイ上の点はベクトル方程式を用いて
$$\vec{p} = \vec{a} + t\vec{d}$$
と書ける。ただし、$t\gt 0$

球面上の点は
$$
    |\vec{p} - \vec{p_c}|^2 = r^2
$$
で表される。

２つのベクトル方程式を連立して式変形すると、
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
