#pragma once
#include "common.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define safe_free(p) \
    if (p) {         \
        free(p);     \
        p = NULL;    \
    }

void l3SetOuterRectPoligon(l3Poligon* poligon);
l3Type l3GetTriangleArea(l3Mat21 p, l3Mat21 a, l3Mat21 b);
l3Vertex* l3CreateVertex(l3Type x, l3Type y, l3Type z, l3RGB* color);
l3Poligon* l3CreatePoligon(int v1, int v2, int v3);
l3Poligon* l3CreatePoligonSphere(int center, int upper, l3Type radius);
l3Poligon* l3CreatePoligonPlane(int point, l3Mat31 normal);
l3Poligon* l3CreatePoligonSky();
l3Poligon* l3CreatePoligonColumn(int top, int end, l3Type radius);
l3Poligon* l3CreatePoligonCircle(int point, l3Mat31 normal, l3Type radius);
l3Poligon* l3ClonePoligon(l3Poligon* p);
void l3DestructPoligon(l3Poligon* p);
void l3ClearPoligon(l3Poligon* p);

void l3DestructVertices(int vc, l3Vertex* vs[]);
void l3DestructPoligons(int pc, l3Poligon* ps[]);
void l3ClearObject(l3Object* o);
void l3SetTransposeObject(l3Object* o, l3Type dx, l3Type dy, l3Type dz);
void l3SetScaleObject(l3Object* o, l3Type sx, l3Type sy, l3Type sz);
void l3SetPoligonsToObject(l3Object* o, int count, l3Poligon* ps[]);

void l3SetCameraInfoToEnvironment(l3Environment* env,
                                  l3Type cx, l3Type cy, l3Type cz,
                                  l3Type tx, l3Type ty, l3Type tz,
                                  l3Type ux, l3Type uy, l3Type uz,
                                  l3Type angle, l3Type near, l3Type far);
void l3InitializeEnvironment(l3Environment* env);
/**
 * オブジェクトを追加、インデックスを返却
 */
int l3AddObjectToEnvironment(l3Environment* env, l3Object* obj, const char* name);

/**
 * ポリゴンを追加、インデックスを返却
 */
int l3AddPoligonEnvironment(l3Environment* env, l3Poligon* p);
/**
 * 頂点を追加、インデックスを返却
 */
int l3AddVertexToObject(l3Object* obj, l3Vertex* v);

l3Object* l3GetObjectPtrEnvironment(l3Environment* env, int index);
l3Poligon* l3GetPoligonPtrEnvironment(l3Environment* env, int index);
l3Vertex* l3GetVertexPtrEnvironment(l3Environment* env, int index);
void l3SolvePtrsEnvironment(l3Environment* env);
void l3ClearEnvironment(l3Environment* env);

void l3DestructEnvironment(l3Environment* env);

l3Object* l3CreateObject();
void l3InitializeEnvironment(l3Environment* env);
l3Environment* l3CloneEnvironment(l3Environment* env);
l3Object* l3CloneObject(l3Object* o);

// void l3InitializeRay(l3Ray* ray);
l3Object* l3FindObject(l3Environment* env, const char* name);