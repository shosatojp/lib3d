#include <stdarg.h>
#include <stdbool.h>

typedef double vtype;
typedef vtype* mat;
typedef vtype* mat44;
typedef vtype* mat41;  // 3次元座標、インデックス3は1
typedef vtype* mat21;

typedef struct {
    vtype coordinate[4];
    vtype coordinate2d[4];
    vtype color[3];
    bool converted;  // 複数ポリゴンで重複して使用している場合に無駄に計算しないように
} vertex;

typedef struct {
    vertex* vertices[3];
    // int vertice_count;
    // texture
} poligon;

typedef struct {
    poligon** poligons;  // poligonの配列
    int poligon_count;
    vtype dx, dy, dz;
    vtype sx, sy, sz;
    vtype theta_x, theta_y, theta_z;
} object;

typedef struct {
    vtype color[3];
    poligon* _poligon;
} point_info;

typedef struct {
    vtype r, g, b;
} rgb;