#include "lib3d.h"


void l3PolarToMat41A(l3Mat41 v, double r, double theta, double phi)
{

    // v[0] = r * sin(theta) * sin(phi);
    // v[1] = r * cos(theta);
    // v[2] = r * sin(theta) * cos(phi);

    // v[0] = r * cos(theta);
    // v[1] = r * sin(theta) * cos(phi);
    // v[2] = r * sin(theta) * sin(phi);

    v[0] = r * cos(theta);
    v[1] = r * sin(theta) * sin(phi);
    v[2] = r * sin(theta) * cos(phi);

    // v[0] = r * sin(theta) * cos(phi);
    // v[1] = r * cos(theta);
    // v[2] = r * sin(theta) * sin(phi);

    v[3] = 1;
}