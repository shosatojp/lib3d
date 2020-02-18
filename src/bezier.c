#include "lib3d.h"


bezier l3GetBezierCurve(double timeTransition,int cpoint_number, bezier cpoints[]){
    int n = cpoint_number,i;
    double t = timeTransition;
    bezier result;
    for(i = 0;i < n;i++){
        result.x += cpoints[i].x * Bernstein(n-1,i,t);
        result.y += cpoints[i].y * Bernstein(n-1,i,t);
        result.z += cpoints[i].z * Bernstein(n-1,i,t);
    }
    return result;
}

double Bernstein(int n, int i, double t){
    return Binomial(n,i) * pow(t,i) * pow(1 - t, n - i);
}

double Binomial(int n, int k){
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

double Factorial(int a){
    double result = 1.0;
    int i;
    for(i = 2; i <= a; i++){
        result *= i;
    }
    return result;
}