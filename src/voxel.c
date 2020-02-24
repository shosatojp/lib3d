#include "lib3d.h"

void l3InitVoxelMap(l3VoxelMap* map, int voxel_size, int sx, int sy, int sz, l3Type ox, l3Type oy, l3Type oz) {
    map->voxels = calloc(sizeof(l3Voxel), sx * sy * sz);
    map->voxel_size = voxel_size;
    map->size[0] = ceil(sx / voxel_size);
    map->size[1] = ceil(sy / voxel_size);
    map->size[2] = ceil(sz / voxel_size);
    map->origin[0] = ox;
    map->origin[1] = oy;
    map->origin[2] = oz;
}

void l3DestructVoxelMap(l3VoxelMap* map) {
    safe_free(map->voxels);
}

void l3SetVoxel(l3VoxelMap* map, l3Mat31 p) {
    l3Mat31A local = {0};
    l3SubMat3(p, map->origin, local);
    int x = local[0] / map->voxel_size,
        y = local[1] / map->voxel_size,
        z = local[2] / map->voxel_size;
    if (0 <= x && x < map->size[0] &&
        0 <= y && y < map->size[1] &&
        0 <= z && z < map->size[2]) {
        long index = x +
                     y * map->size[0] +
                     z * map->size[0] * map->size[1];
        map->voxels[index].a = true;
    }
}

/**
 * ワールド座標変換の後
 */
void l3VoxelizePoligonTriangle(l3VoxelMap* map, l3Poligon* poligon) {
    l3Mat31A e1, e2;
    l3SubMat3(poligon->vertices[1]->coordinateWorld, poligon->vertices[0]->coordinateWorld, e1);
    l3SubMat3(poligon->vertices[2]->coordinateWorld, poligon->vertices[0]->coordinateWorld, e2);

    l3Mat31A e1_e2;
    l3AddMat3(e1, e2, e1_e2);

    l3Type n = ceilf(l3VecAbs3(e1_e2) / (sqrtf(3) * map->voxel_size));

    for (int u = 0; u <= n; u++) {
        for (int v = 0; v <= n - u; v++) {
            l3Mat31A p, tmp1, tmp2;
            l3ScalarMulMat3(e1, u / n, tmp1);
            l3ScalarMulMat3(e2, v / n, tmp2);
            l3AddMat3(tmp1, tmp2, p);
            l3AddMat3(poligon->vertices[0]->coordinateWorld, p, p);
            l3SetVoxel(map, p);
        }
    }
}

void l3VoxelizePoligonSphere(l3VoxelMap* map, l3Poligon* poligon) {
    l3Mat31 c = poligon->vertices[0]->coordinateWorld;
    l3Type r = poligon->sphere_radius;

    for (int i = c[0] - r, ii = c[0] + r; i < ii; i++) {
        for (int j = c[1] - r, jj = c[1] + r; j < jj; j++) {
            for (int k = c[2] - r, kk = c[2] + r; k < kk; k++) {
                l3Mat31A p = {i, j, k};
                l3Type d = l3DistanceVec3(p, c);
                if (d < r) {
                    l3SetVoxel(map, p);
                }
            }
        }
    }
}

void l3VoxelizePoligonPlane(l3VoxelMap* map, l3Poligon* poligon) {
}

void l3PrintVoxel(l3VoxelMap* map) {
    for (int i = 0; i < map->size[2]; i++) {
        for (int j = 0; j < map->size[1]; j++) {
            printf("|");
            for (int k = 0; k < map->size[0]; k++) {
                long index = k + j * map->size[0] + i * map->size[0] * map->size[1];
                printf("%c ", map->voxels[index].a ? '.' : ' ');
            }
            printf("|");
            printf("\n");
        }
        for (int k = 0; k < map->size[0]; k++) {
            printf("--");
        }
        printf("--\n");
    }
}

void voxel_test() {
    l3VoxelMap map = {0};
    l3InitVoxelMap(&map, 3, 21, 11, 21, 0, 0, 0);
    l3Poligon p = {0};
    l3Vertex a, b, c;
    p.vertices[0] = &a;
    p.vertices[1] = &b;
    p.vertices[2] = &c;
    p.vertices[0]->coordinateWorld[0] = 10;
    p.vertices[0]->coordinateWorld[1] = 0;
    p.vertices[0]->coordinateWorld[2] = 0;
    p.vertices[1]->coordinateWorld[0] = 20;
    p.vertices[1]->coordinateWorld[1] = 10;
    p.vertices[1]->coordinateWorld[2] = 20;
    p.vertices[2]->coordinateWorld[0] = 0;
    p.vertices[2]->coordinateWorld[1] = 0;
    p.vertices[2]->coordinateWorld[2] = 20;
    p.sphere_radius = 10;
    l3VoxelizePoligonSphere(&map, &p);
    l3PrintVoxel(&map);
}