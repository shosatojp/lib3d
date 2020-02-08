
void write_buffer(unsigned char* __buffer, int __w, int __h, const char* __name);
unsigned char* make_buffer(int __w, int __h, char __c);
#define SET_BUFFER_RGB(__buffer, __w, __h, __x, __y, __c) \
    if (0 <= __x && __x < __w && 0 <= __y && __y < __h) { \
        __buffer[(__x + __y * __w) * 3] = __c[0];         \
        __buffer[(__x + __y * __w) * 3 + 1] = __c[1];     \
        __buffer[(__x + __y * __w) * 3 + 2] = __c[2];     \
    }
