#include "common.h"

typedef struct _bezier { double x, y, z;} bezier;

bezier l3GetBezierCurve(double timeTransition,int cpoint_number, bezier cpoints[]);
double Bernstein(int n, int i, double t);
double Binomial(int n, int k);
double Factorial(int a);