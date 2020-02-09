#pragma once

#include "common.h"

/**
 * 行列を零行列に初期化する
 */
void l3InitMat(l3Mat a, int x, int y);

/**
 * a    4*4
 * b    4*1
 * r    4*1
 * 
 * a != r, b != r
 * must initialize r : true
 */
void l3MulMat4441(l3Mat44 a, l3Mat41 b, l3Mat41 r);

/**
 * a    4*4
 * b    4*4
 * r    4*4
 * 
 * a != r, b != r
 * must initialize r : true
 * 
 */
void l3MulMat4444(l3Mat44 a, l3Mat44 b, l3Mat44 r);

/**
 * a = r, b = r
 * must initialize r : false
 */
void l3SubMat(l3Mat a, l3Mat b, l3Mat r, int n);

/**
 * a = r
 * must initialize r : false
 */
void l3DivMat(l3Mat a, l3Type d, l3Mat r, int n);
void l3PrintMat(l3Mat a, int x, int y);
/**
 * a = r
 */
void l3NormarizeVec(l3Mat a, l3Mat r, int n);
/**
 * a = r, b = r
 */
void l3CrossProductVec3(l3Mat41 a, l3Mat41 b, l3Mat41 r);
l3Type l3CrossProductVec2(l3Mat21 a, l3Mat21 b);
l3Type l3InnerProductVec(l3Mat a, l3Mat b, int n);
void l3MulMat44s44(int c, l3Mat44* as[], l3Mat44 r);
bool l3InsideOfPoligon2D(int c, l3Vertex* _poligon[], l3Mat21 a);