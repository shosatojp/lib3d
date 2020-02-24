#include "common.h"

typedef struct {
    l3Environment* env;
    int y_begin;
    int y_end;
    int x_begin;
    int x_end;
    int frame;
    int block_x;
    int block_y;
    int block_cx;
    int block_cy;
} l3BlockRendererInfo;

void l3MultithreadRenderer(l3Environment* env, l3Renderer* renderer, l3FrameTransitionFunction* transitionFn, int frames, int thread_count);
void l3MultithreadSequentialRenderer(l3Environment* env,
                                     l3FrameTransitionFunction* transitionFn,
                                     l3Options* options);
void l3RasterizingRenderer(l3Environment* env);
void l3RaytracingRenderer(l3Environment* env);

void l3RaytracingBlockMultithreadedRenderer(l3Environment* env,
                                            l3FrameTransitionFunction* transitionFn,
                                            l3Options* options);
void l3RaytracingBlockRenderer(l3BlockRendererInfo* blockinfo, int thread_num);
