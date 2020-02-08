#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    int w = 1920,
        h = 1080;
    unsigned char* buf = make_buffer(w, h, 255);
    unsigned char white[] = {0, 0, 0};
    for (int i = 10; i < 11; i++) {
        for (int j = 50; j < 51; j++) {
            SET_BUFFER_RGB(buf, w, h, i, j, white);
        }
    }
    write_buffer(buf, w, h, "out.ppm");
}