#include "common.h"
#include "lib3d.h"

#define l3RAY_TRACE_MAX_DEPTH 4

void l3GetRayStartPointAndDirection(l3Mat33 p_world_to_camera, l3Mat31 camera_pos, l3Type angle,
                                    l3Type w, l3Type h,
                                    l3Type u, l3Type v, l3Mat31 start_point, l3Mat31 direction);

bool l3IntersectRayPoligon(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Poligon* poligon, l3Mat41 r, l3Mat21 uv);
void l3GetPoligonNormal(l3Poligon* poligon, l3Mat41 normal);
void l3GetRayPoligon2DTextureCoordinate(l3Poligon* poligon, l3Mat21 uv, l3Mat21 r);
bool l3IntersectRaySphere(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Mat41 sphere_center, l3Type sphere_radius, l3Mat41 r);
void l3GetRaySphere2DTextureCoordinate(l3Mat41 sphere_local, l3Mat21 r);
void l3GetSphereNormal(l3Mat41 sphere_local, l3Mat41 r);
void l3GetReflectedVec(l3Mat41 incident_vec, l3Mat41 normal, l3Mat41 r);
void l3MultiplyColor(l3RGB* a, l3RGB* b, l3RGB* r);
void l3MakeWorldToCameraBasisChangeMat33(l3CameraInfo* camerainfo, l3Mat33 r);
bool l3FindRayCrossPoint(l3Ray* ray, l3Environment* env);
void l3TraceRay(l3Ray* ray, l3Environment* env, int depth);
void l3SetWorldCoordinate(l3Environment* env);