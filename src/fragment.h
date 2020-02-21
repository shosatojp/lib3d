#pragma once

#include "common.h"
/**
 * フラグメントシェーダ
 */
void l3FragmentShader(l3PixelInfo* p, l3Mat21 v);
unsigned char* l3LoadPPM(const char* path, int* w, int* h);
int l3Load2DTexture(const char* path, l3Texture* texture);
void l3DestructTexture(l3Texture* texture);
void l3SetTextureMatInv(l3Poligon* poligon);
void l3SetUVTexturePoligon(l3Poligon* poligon, l3Texture* texture,l3Mat23 texture_vertices);
unsigned char* l3GetColorAtTexture(l3Texture* texture, int x, int y);
