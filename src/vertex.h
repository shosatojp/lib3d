#pragma once

#include "common.h"

void l3MakeWorldToCameraMat44(l3CameraInfo* camerainfo, l3Mat44 r);
void l3MakeCameraToProjectionMat44(l3Type angle, l3Type aspect, l3Type near, l3Type far, l3Mat44 r);
void l3MakeProjectionToScreenMat44(l3Type width, l3Type height, l3Mat44 r);
void l3MakeRoundXMat44(l3Type theta, l3Mat44 r);
void l3MakeRoundYMat44(l3Type theta, l3Mat44 r);
void l3MakeRoundZMat44(l3Type theta, l3Mat44 r);
void l3MakeTransposeMat44(l3Type dx, l3Type dy, l3Type dz, l3Mat44 r);
void l3MakeScaleMat44(l3Type sx, l3Type sy, l3Type sz, l3Mat44 r);
void l3MakeLocalToWorldMat44(l3Type dx, l3Type dy, l3Type dz,
                             l3Type sx, l3Type sy, l3Type sz,
                             l3Type theta_x, l3Type theta_y, l3Type theta_z, l3Mat44 r);
void l3AppendPoligonsFromEnvironment(l3Environment* env, l3Mat44 wcps, int w, int h);
l3Type l3GetBoundingRadius(l3Object* _object);
bool l3FrustumCulling(l3Object* _object, l3Mat44 lwcps, int w, int h);
