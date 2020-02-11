#include "lib3d.h"

void l3SetOuterRectPoligon(l3Poligon* _poligon) {
    l3Type maxx = _poligon->vertices[0]->coordinate2d[0],
           maxy = _poligon->vertices[0]->coordinate2d[1],
           minx = _poligon->vertices[0]->coordinate2d[0],
           miny = _poligon->vertices[0]->coordinate2d[1];

    for (int i = 1; i < 3; i++) {
        if (_poligon->vertices[i]->coordinate2d[0] > maxx) maxx = _poligon->vertices[i]->coordinate2d[0];
        if (_poligon->vertices[i]->coordinate2d[1] > maxy) maxy = _poligon->vertices[i]->coordinate2d[1];
        if (_poligon->vertices[i]->coordinate2d[0] < minx) minx = _poligon->vertices[i]->coordinate2d[0];
        if (_poligon->vertices[i]->coordinate2d[1] < miny) miny = _poligon->vertices[i]->coordinate2d[1];
    }

    _poligon->max[0] = maxx;
    _poligon->max[1] = maxy;
    _poligon->min[0] = minx;
    _poligon->min[1] = miny;
}

l3Type l3GetTriangleArea(l3Mat21 p, l3Mat21 a, l3Mat21 b) {
    return fabs((a[0] - p[0]) * (b[1] - p[1]) - (a[1] - p[1]) * (b[0] - p[0])) / 2;
}

l3Vertex* l3CreateVertex(l3Type x, l3Type y, l3Type z, l3RGB* color) {
    l3Vertex* _v = (l3Vertex*)calloc(sizeof(l3Vertex), 1);
    _v->coordinate[0] = x;
    _v->coordinate[1] = y;
    _v->coordinate[2] = z;
    _v->coordinate[3] = 1;
    _v->color.r = color->r;
    _v->color.g = color->g;
    _v->color.b = color->b;
    return _v;
}

void l3DestructVertices(int vc, l3Vertex* vs[]) {
    while (vc)
        free((void*)vs[--vc]);
}

l3Vertex* l3CloneVertex(l3Vertex* v) {
    l3Vertex* _v = (l3Vertex*)malloc(sizeof(l3Vertex));
    memcpy(_v, v, sizeof(l3Vertex));
    return _v;
}

l3Poligon* l3CreatePoligon(int v1, int v2, int v3) {
    l3Poligon* _p = (l3Poligon*)calloc(sizeof(l3Poligon), 1);
    _p->vertices[0] = v1;
    _p->vertices[1] = v2;
    _p->vertices[2] = v3;
    return _p;
}

l3Poligon* l3ClonePoligon(l3Poligon* p) {
    l3Poligon* _p = (l3Poligon*)malloc(sizeof(l3Poligon));
    memcpy(_p, p, sizeof(l3Poligon));
    // _p->vertices[0] = l3CloneVertex(p->vertices[0]);
    // _p->vertices[1] = l3CloneVertex(p->vertices[1]);
    // _p->vertices[2] = l3CloneVertex(p->vertices[2]);
    _p->textureVertices = l3CloneMat(p->textureVertices, 2, 3);
    _p->textureAffineMatInv = l3CloneMat(p->textureAffineMatInv, 3, 3);
    // textureは放置
    return _p;
}

// destructはVertexごと、Poligonごとにやる？
void l3DestructPoligon(l3Poligon* p) {
    safe_free(p->textureVertices);
    safe_free(p->textureAffineMatInv);
}

void l3DestructPoligons(int pc, l3Poligon* ps[]) {
    while (pc) {
        free((void*)ps[--pc]);
    }
}

void l3InitializeObject(l3Object* o) {
    memset(o, 0, sizeof(l3Object));
}

l3Object* l3CloneObject(l3Object* o) {
    l3Object* _o = (l3Object*)malloc(sizeof(l3Object));
    memcpy(_o, o, sizeof(l3Object));
    _o->poligons = (l3Poligon**)malloc(sizeof(l3Poligon*) * o->poligon_count);
    for (int i = 0; i < o->poligon_count; i++) {
        _o->poligons[i] = l3ClonePoligon(o->poligons[i]);
    }
}

void l3SetTransposeObject(l3Object* o, l3Type dx, l3Type dy, l3Type dz) {
    o->dx = dx;
    o->dy = dy;
    o->dz = dz;
}

void l3SetScaleObject(l3Object* o, l3Type sx, l3Type sy, l3Type sz) {
    o->sx = sx;
    o->sy = sy;
    o->sz = sz;
}

void l3SetPoligonsObject(l3Object* o, int count, l3Poligon* ps[]) {
    o->poligons = ps;
    o->poligon_count = count;
}

void l3MakeCameraInfo(l3Type cx, l3Type cy, l3Type cz,
                      l3Type tx, l3Type ty, l3Type tz,
                      l3Type ux, l3Type uy, l3Type uz, l3CameraInfo* camerainfo) {
    camerainfo->coordinate[0] = cx;
    camerainfo->coordinate[1] = cy;
    camerainfo->coordinate[2] = cz;
    camerainfo->target[0] = tx;
    camerainfo->target[1] = ty;
    camerainfo->target[2] = tz;
    camerainfo->upper[0] = tx;
    camerainfo->upper[1] = ty;
    camerainfo->upper[2] = tz;
}

void l3InitializeEnvironment(l3Environment* env) {
    env->objects = array_new(sizeof(l3Object*), true, 10);
    env->poligons = array_new(sizeof(l3Poligon*), true, 10);
    env->vertices = array_new(sizeof(l3Vertex*), true, 10);
}

/**
 * オブジェクトを追加、インデックスを返却
 */
int l3AddObjectEnvironment(l3Environment* env, l3Object* obj) {
    return array_push(env->objects, obj);
}

/**
 * ポリゴンを追加、インデックスを返却
 */
int l3AddPoligonEnvironment(l3Environment* env, l3Poligon* p) {
    return array_push(env->poligons, p);
}

/**
 * 頂点を追加、インデックスを返却
 */
int l3AddVertexEnvironment(l3Environment* env, l3Vertex* v) {
    return array_push(env->vertices, v);
}

l3Object* l3GetObjectPtrEnvironment(l3Environment* env, int index) {
    return array_at(env->objects, index);
}
l3Poligon* l3GetPoligonPtrEnvironment(l3Environment* env, int index) {
    return array_at(env->poligons, index);
}
l3Vertex* l3GetVertexPtrEnvironment(l3Environment* env, int index) {
    return array_at(env->vertices, index);
}