#include "common.h"
#include "lib3d.h"

bool l3IntersectRayPoligon(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Poligon* poligon, l3Mat41 r, l3Mat21 uv);
void l3GetPoligonNormal(l3Poligon* poligon, l3Mat41 normal);
void l3GetRayPoligon2DTextureCoordinate(l3Poligon* poligon, l3Mat21 uv, l3Mat21 r);
bool l3IntersectRaySphere(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Mat41 sphere_center, l3Type sphere_radius, l3Mat41 r);
void l3GetRaySphere2DTextureCoordinate(l3Mat41 sphere_local, l3Mat21 r);
void l3GetSphereNormal(l3Mat41 sphere_local, l3Mat41 r);