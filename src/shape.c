#include "lib3d.h"

/**
 * 円柱
 */
bool l3IntersectRayColumn(l3Mat41 ray_origin, l3Mat41 ray_direction,
                          l3Mat41 v0, l3Mat41 v1, l3Type radius, l3Mat41 r) {
    l3Mat41A u;
    l3SubMat3(v1, v0, u);
    l3NormarizeVec3(u, u);

    l3Mat41A tmp;
    l3SubMat3(ray_origin, v0, tmp);

    l3Mat41A l;
    l3CrossProductVec3(tmp, u, l);

    l3Mat41A right;
    l3CrossProductVec3(ray_direction, u, right);

    l3Type a = l3InnerProductVec3(right, right);
    l3Type b = l3InnerProductVec3(l, right);
    l3Type c = l3InnerProductVec3(l, l) - radius * radius;

    l3Type d = b * b - a * c;
    if (d > 0) {
        l3Type t = (-b - sqrtf(d)) / a;
        if (t > 0) {
            // 長さ判定
            l3Mat41A tmp_l;
            l3SubMat3(v1, v0, tmp_l);
            l3Type length = l3VecAbs3(tmp_l);

            r[0] = ray_origin[0] + t * ray_direction[0];
            r[1] = ray_origin[1] + t * ray_direction[1];
            r[2] = ray_origin[2] + t * ray_direction[2];

            l3Mat41A tmp_l2;
            l3SubMat3(r, v0, tmp_l2);
            l3Type length2 = l3InnerProductVec3(tmp_l2, u);
            l3Type p0_p = l3VecAbs3(tmp_l2);

            l3Type cos_theta = length2 / p0_p;

            if (0 <= cos_theta && cos_theta <= 1 && 0 <= length2 && length2 <= length) {
                return true;
            }
        }
    }

    return false;
}

/**
 * 円盤
 */
bool l3IntersectRayCircle(l3Mat41 ray_origin, l3Mat41 ray_direction, l3Mat41 center, l3Mat41 normal, l3Type radius, l3Mat41 r) {
    l3Mat31A tmp;
    l3SubMat3(center, ray_origin, tmp);
    l3Type t = l3InnerProductVec3(tmp, normal) / l3InnerProductVec3(ray_direction, normal);
    if (t > 0) {
        r[0] = ray_origin[0] + t * ray_direction[0];
        r[1] = ray_origin[1] + t * ray_direction[1];
        r[2] = ray_origin[2] + t * ray_direction[2];
        l3Mat31A r_center;
        l3SubMat3(r, center, r_center);
        if (l3VecAbs3(r_center) <= radius) {
            return true;
        }
    }
    return false;
}