#include "lib3d.h"
#include <math.h>

double logn(int base, double antilog) {
    return log(antilog) / log((double)base);
}

double l3TimeTransition(l3TimeTransitionType TypeId ,int frame, int start_frame,int end_frame){
    double t;
    double p;
     if(frame<start_frame){
        p=0.0;
    }else if(frame>end_frame){
        p=1.0;
    }else{
        p = ((double)frame-(double)start_frame)/((double)end_frame-(double)start_frame); // 経過時間の割合;
    }


    if(TypeId == l3TimeType_linear){
        t = p;
        return t;
    }

    if(TypeId == l3TimeType_EasyEase){
        t = (tanh((p-0.5)*6)+1)/2;
        return t;
    }
    if(TypeId == l3TimeType_EasyEaseIn){
        t = pow(p,4);
        return t;
    }
    if(TypeId == l3TimeType_EasyEaseOut){
        t =-p*(p-2);
        return t;
    }
}