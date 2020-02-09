#include "common.h"
void l3WriteBuffer(unsigned char* buffer, int w, int h, const char* name);
unsigned char* l3MakeBuffer(int w, int h, char c);
#define SET_BUFFER_RGB(buffer, w, h, x, y, c) \
    if (0 <= x && x < w && 0 <= y && y < h) { \
        buffer[(x + y * w) * 3] = c[0];       \
        buffer[(x + y * w) * 3 + 1] = c[1];   \
        buffer[(x + y * w) * 3 + 2] = c[2];   \
    }

void l3ClearBuffer(unsigned char* buf, int w, int h);
