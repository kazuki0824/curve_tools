/*
 * trajectory_tools.cu
 *
 *  Created on: 2018/04/20
 *      Author: dvr1
 */

#include <vector>
#include <iostream>

#include "spline_gpu.hpp"

__host__ float max_speed_cpu(vector<pSpline_polynomial>& C, float step){
    int count = C.size();
    float v = 0.0;
    for (int var =0 ; var<count; var++)
    {
        for (float t = 0;t<1.0;t+=step)
        {
            v = fmaxf(v, length(C[var].speed(t)));
        }
    }
    return v;
}
__host__ float max_accel_cpu(vector<pSpline_polynomial>& C){
    int count = C.size();
    float v = 0.0;
    for (int var =0 ; var<count; var++)
    {
        for (float t = 0;t<1.0;t+=step)
        {
            a = fmaxf(a, length(C[var].accel(t)));
        }
    }
    return a;
}

__host__ __device__ static bool isInside(pSpline_polynomial* curve__part , float2 pos, float t, float v)
{
    const float boundary_radius = 0.1; //TODO: 定数
    boundary_radius *= v;
    return (boundary_radius > length(curve__part->x_and_y(t) - pos));
}

__host__ float sweep_cpu(vector<pSpline_polynomial>& C, float2 pos, float _speed, float step, float start = 0.0)
{
     int count = C.size();

    float end = (float)count;

    for (float t = start; t<end; t+=step)
    {
        int var = t / 1;
        if (isInside(&C[var], pos, t % 1.0, _speed))
            continue;
        else
            break;
    }
    return t;
}
