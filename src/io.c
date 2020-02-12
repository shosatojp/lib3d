#include "lib3d.h"

void l3WriteBuffer(unsigned char* buffer, int w, int h, const char* name) {
    FILE* fp = fopen(name, "w");
    if (fp) {
        fprintf(fp, "P6\n%d %d\n255\n", w, h);
        fwrite(buffer, w * h * 3, 1, fp);
        fclose(fp);
    } else {
        fprintf(stderr, "Error: cannot open file '%s'\n", name);
    }
}

/**
 * freeする必要あり
 */
unsigned char* l3CreateBuffer(int w, int h) {
    int size = w * h * sizeof(char) * 3;
    return (unsigned char*)malloc(size);
}

void l3ClearBuffer(unsigned char* buf, int w, int h, char c) {
    memset(buf, c, w * h * sizeof(char) * 3);
}
