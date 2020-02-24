#include "lib3d.h"

bezier l3GetBezierCurve(double timeTransition, int cpoint_number, bezier cpoints[]) {
    volatile int n = cpoint_number, i;
    double t = timeTransition;
    volatile bezier result;
    for (i = 0; i < n; i++) {
        result.x += cpoints[i].x * Bernstein(n - 1, i, t);
        result.y += cpoints[i].y * Bernstein(n - 1, i, t);
        result.z += cpoints[i].z * Bernstein(n - 1, i, t);
    }
    return result;
}

double Bernstein(int n, int i, double t) {
    return Binomial(n, i) * pow(t, i) * pow(1 - t, n - i);
}

double Binomial(int n, int k) {
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

double Factorial(int a) {
    double result = 1.0;
    int i;
    for (i = 2; i <= a; i++) {
        result *= i;
    }
    return result;
}

void l3SetBezierPoint(bezier* b, double x, double y, double z) {
    b->x = x;
    b->y = y;
    b->z = z;
}

void l3BezierToMat41A(l3Mat41 mat, bezier b) {
    mat[0] = b.x;
    mat[1] = b.y;
    mat[2] = b.z;
    mat[3] = 1;
}