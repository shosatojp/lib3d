#include "common.h"

typedef struct _bezier { double x, y, z;} bezier;

bezier l3GetBezierCurve(double timeTransition,int cpoint_number, bezier cpoints[]);
double Bernstein(int n, int i, double t);
double Binomial(int n, int k);
double Factorial(int a);
void l3SetBezierPoint(bezier* b,double x,double y, double z);
void l3BezierToMat41A( l3Mat41 mat, bezier b);