#include "common.h"

typedef struct _bezier { double x, y, z;} bezier;
typedef struct Cpoint {double x,y,z;} cpoint; 

bezier l3GetBezierCurve(int frame,int start_frame, int end_frame,int cpoint_number, cpoint cpoints[]);