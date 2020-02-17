#include "lib3d.h"
#include <math.h>

double logn(int base, double antilog) {
    return log(antilog) / log((double)base);
}

double l3TimeTransition(l3TimeTransitionType TypeId ,int frame, int start_frame,int end_frame){
    double t;
    double p = ((double)frame-(double)start_frame)/((double)end_frame-(double)start_frame); // 経過時間の割合


    if(TypeId == l3TimeType_linear){
        if(frame<start_frame){
            t=0.0;
        }else if(frame>end_frame){
            t=1.0;
        }else{
            t = p;
        }
        return t;
    }

    if(TypeId == l3TimeType_EasyEase){
        if(frame<start_frame){
            t=0.0;
        }else if(frame>end_frame){
            t=1.0;
        }else{
            t = (tanh((p-0.5)*6)+1)/2;
        }
        return t;
    }
    if(TypeId == l3TimeType_EasyEaseIn){
        if(frame<start_frame){
            t=0.0;
        }else if(frame>end_frame){
            t=1.0;
        }else{
            t = pow(p,4);
        }
        return t;
    }
    if(TypeId == l3TimeType_EasyEaseOut){
        if(frame<start_frame){
            t=0.0;
        }else if(frame>end_frame){
            t=1.0;
        }else{
            t =-p*(p-2);
        }
        return t;
    }
}