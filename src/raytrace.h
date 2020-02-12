#include "common.h"
#include "lib3d.h"

bool l3IntersectRayPoligon(l3Mat31 ray_origin, l3Mat31 ray_direction, l3Poligon* poligon, l3Mat31 r, l3Mat21 uv);
void l3PoligonNormal(l3Poligon* poligon, l3Mat31 normal);
void l3GetRayPoligon2DTextureCoordinate(l3Poligon* poligon, l3Mat21 uv, l3Mat21 r);
bool l3IntersectRaySphere(l3Mat31 ray_origin, l3Mat31 ray_direction, l3Mat31 sphere_center, l3Type sphere_radius, l3Mat31 r);
void l3GetRaySphere2DTextureCoordinate(l3Mat31 sphere_local, l3Mat21 r);
void l3GetSphereNormal(l3Mat31 sphere_local, l3Mat31 r);