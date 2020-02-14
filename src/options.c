#include "lib3d.h"

bool l3ParseArgs(int argc, const char* argv[], l3Options* r) {
    for (size_t i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            r->outdir = argv[++i];
        } else if (!strcmp(argv[i], "-w")) {
            r->w = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-h")) {
            r->h = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-t")) {
            r->threads = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-f")) {
            r->frames = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-r")) {
            ++i;
            if (!strcmp(argv[i], "raytrace")) {
                r->renderer = l3RaytracingRenderer;
            } else if (!strcmp(argv[i], "rasterize")) {
                r->renderer = l3RasterizingRenderer;
            } else {
                fprintf(stderr, "invalid renderer\n");
                return false;
            }
        } else {
            fprintf(stderr, "invalid argument\n");
        }
    }
    return true;
}