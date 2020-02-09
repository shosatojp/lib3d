#pragma once

#include "common.h"

/**
 * 行列を零行列に初期化する
 */
void l3InitMat(mat a, int x, int y);

/**
 * a    4*4
 * b    4*1
 * r    4*1
 * 
 * a != r, b != r
 * must initialize r : true
 */
void l3MulMat4441(mat44 a, mat41 b, mat41 r);

/**
 * a    4*4
 * b    4*4
 * r    4*4
 * 
 * a != r, b != r
 * must initialize r : true
 * 
 */
void l3MulMat4444(mat44 a, mat44 b, mat44 r);

/**
 * a = r, b = r
 * must initialize r : false
 */
void l3SubMat(mat a, mat b, mat r, int n);

/**
 * a = r
 * must initialize r : false
 */
void l3DivMat(mat a, vtype d, mat r, int n);
void l3PrintMat(mat a, int x, int y);
/**
 * a = r
 */
void l3NormarizeVec(mat a, mat r, int n);
/**
 * a = r, b = r
 */
void l3CrossProductVec3(mat41 a, mat41 b, mat41 r);
vtype l3CrossProductVec2(mat21 a, mat21 b);
vtype l3InnerProductVec(mat a, mat b, int n);
void l3MulMat44s44(int c, mat44* as, mat44 r);
bool l3InsideOfPoligon2D(int c, vertex* _poligon[], mat21 a);