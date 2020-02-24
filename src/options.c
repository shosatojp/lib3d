#include "lib3d.h"

bool l3ParseArgs(int argc, const char* argv[], l3Options* r) {
    for (int i = 1; i < argc; i++) {
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
        } else if (!strcmp(argv[i], "-b")) {
            r->frame_begin = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-p")) {
            r->prefix = argv[++i];
        } else if (!strcmp(argv[i], "-bx")) {
            r->block_x = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-by")) {
            r->block_y = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-q")) {
            ++i;
            if (!strcmp(argv[i], "4k")) {
                r->w = 4096;
                r->h = 2160;
            } else if (!strcmp(argv[i], "2k")) {
                r->w = 2560;
                r->h = 1440;
            } else if (!strcmp(argv[i], "fhd")) {
                r->w = 1920;
                r->h = 1080;
            } else if (!strcmp(argv[i], "hd")) {
                r->w = 1280;
                r->h = 720;
            } else if (!strcmp(argv[i], "sd")) {
                r->w = 720;
                r->h = 480;
            } else if (!strcmp(argv[i], "ld")) {
                r->w = 480;
                r->h = 320;
            }
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