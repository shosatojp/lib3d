#include "common.h"
typedef struct _l3Options {
    int w, h;
    const char* outdir;
    int threads, frames;
    l3Renderer* renderer;
} l3Options;

bool l3ParseArgs(int argc, const char* argv[], l3Options* r);