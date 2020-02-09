#pragma once

#include "common.h"

/**
 * 行列を零行列に初期化する
 */
void init_mat(mat a, int x, int y);

/**
 * a    4*4
 * b    4*1
 * r    4*1
 * 
 * a != r, b != r
 * must initialize r : true
 */
void mul44(mat44 a, mat41 b, mat41 r);

/**
 * a    4*4
 * b    4*4
 * r    4*4
 * 
 * a != r, b != r
 * must initialize r : true
 * 
 */
void mul4444(mat44 a, mat44 b, mat44 r);

/**
 * a = r, b = r
 * must initialize r : false
 */
void vector_sub(mat a, mat b, mat r, int n);

/**
 * a = r
 * must initialize r : false
 */
void vector_div(mat a, vtype d, mat r, int n);
void mat_print(mat a, int x, int y);
/**
 * a = r
 */
void normarize_vector(mat a, mat r, int n);
/**
 * a = r, b = r
 */
void vector_product3(mat41 a, mat41 b, mat41 r);
vtype vector_product2(mat21 a, mat21 b);
vtype inner_product_1d(mat a, mat b, int n);

void mul44s(int c, mat44* as, mat44 r);
bool is_in_2d_poligon(int c, vertex* _poligon[], mat21 a);