#include "lib3d.h"

void write_buffer(unsigned char* __buffer, int __w, int __h, const char* __name) {
    FILE* fp = fopen(__name, "w");
    if (fp) {
        fprintf(fp, "P6\n%d %d\n255\n", __w, __h);
        fwrite(__buffer, __w * __h * 3, 1, fp);
        fclose(fp);
    } else {
        fprintf(stderr, "Error: cannot open file '%s'\n", __name);
    }
}

unsigned char* make_buffer(int __w, int __h, char __c) {
    int size = __w * __h * sizeof(char) * 3;
    unsigned char* buffer = (unsigned char*)malloc(size);
    memset(buffer, __c, size);
    return buffer;
}

void clear_buffer(unsigned char* buf, int w, int h) {
    memset(buf, 0, w * h * sizeof(char) * 3);
}
