#include "lib3d.h"

/**
 * 反射率を求める関数
 * angle: 法線ベクトルと入射ベクトルの逆ベクトルがなす角
 * metallicity: 物体特有の金属度。金属度が上がると鏡面反射が多くなる
 * 
 * 
 * 他に、roughness: 拡散反射光の取得範囲
 * 透過率
 */
l3Type l3ReflectionRate(l3Type angle, l3Type metallicity) {
    return exp(angle / (PI / 2) * 5) / exp(5) * (1 - metallicity) + metallicity;
}