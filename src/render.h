#include "common.h"
void l3MultithreadRenderer(l3Environment* env, l3Renderer* renderer, l3FrameTransitionFunction* transitionFn, int frames, int thread_count);
void l3RasterizingRenderer(l3Environment* env);
void l3RaytracingRenderer(l3Environment* env);