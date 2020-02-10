#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point {
    int x, y;
} point1;

void* func(void* param) {
    printf("%d\n", ((struct point*)param)->x);
}

int main() {
    pthread_t t;
    point1.x = 3;
    int r;
    r = pthread_create(&t, NULL, func, &point1);
    r = pthread_join(t, NULL);
    
}