#include "common.h"

/**
 * aを上書き
 */
#define l3AddColor(color_a, color_b) \
    color_a.r += color_b.r;          \
    color_a.g += color_b.g;          \
    color_a.b += color_b.b;

/**
 * aを上書き
 */
#define l3DivColor(color_a, scalar) \
    color_a.r /= scalar;            \
    color_a.g /= scalar;            \
    color_a.b /= scalar;
