#pragma once

#include "common.h"

void l3MakeWorldToCameraMat44(mat41 camera, mat41 target, mat41 upper, mat44 r);
void l3MakeCameraToProjectionMat44(vtype angle, vtype aspect, vtype near, vtype far, mat44 r);
void l3MakeProjectionToScreenMat44(vtype width, vtype height, mat44 r);
void l3MakeRoundXMat44(vtype theta, mat44 r);
void l3MakeRoundYMat44(vtype theta, mat44 r);
void l3MakeRoundZMat44(vtype theta, mat44 r);
void l3MakeTransposeMat44(vtype dx, vtype dy, vtype dz, mat44 r);
void l3MakeScaleMat44(vtype sx, vtype sy, vtype sz, mat44 r);
void l3MakeLocalToWorldMat44(vtype dx, vtype dy, vtype dz,
                               vtype sx, vtype sy, vtype sz,
                               vtype theta_x, vtype theta_y, vtype theta_z, mat44 r);
void l3ConvertObject(object* _object, mat44 wcp, mat44 ps, pixel_info* _map, int w, int h);