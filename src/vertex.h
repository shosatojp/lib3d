#pragma once

#include "common.h"

void make_world_to_camera_mat44(mat41 camera, mat41 target, mat41 upper, mat44 r);
void make_camera_to_projection_mat44(vtype angle, vtype aspect, vtype near, vtype far, mat44 r);
void make_projection_to_screen_mat44(vtype width, vtype height, mat44 r);
void make_matrix_round_x(vtype theta, mat44 r);
void make_matrix_round_y(vtype theta, mat44 r);
void make_matrix_round_z(vtype theta, mat44 r);
void make_matrix_transpose(vtype dx, vtype dy, vtype dz, mat44 r);
void make_matrix_scale(vtype sx, vtype sy, vtype sz, mat44 r);
void make_local_to_world_mat44(vtype dx, vtype dy, vtype dz,
                               vtype sx, vtype sy, vtype sz,
                               vtype theta_x, vtype theta_y, vtype theta_z, mat44 r);
void convert_object(object* _object, mat44 wcp, mat44 ps, pixel_info* _map, int w, int h);