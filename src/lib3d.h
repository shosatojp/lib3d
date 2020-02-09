#include <stdarg.h>
#include <stdbool.h>

typedef double vtype;
typedef vtype* mat;
typedef vtype* mat44;
typedef vtype* mat41;  // 3次元座標、インデックス3は1
typedef vtype* mat21;

typedef struct {
    mat41 coordinate;
    vtype color[3];
} vertex;

typedef vertex* poligon[];  // vertexのポインタの配列

typedef struct {
    poligon* poligons;  // poligonの配列
} object;

typedef struct {
    vtype color[3];
} point_info;