/*
 * spline_gpu.hpp
 *
 *  Created on: 2018/04/12
 *      Author: dvr1
 */

#ifndef SPLINE_GPU_HPP_
#define SPLINE_GPU_HPP_


#include <cusparse_v2.h>

class pSpline_polynomial
{
public:
	float2 start;
	float2 end;
	float2 a,b,c,d;
	float length;

	__host__ __device__ float2 x_and_y(float in);

	__host__ __device__ float P2(float in);

	__host__ __device__ float P(float in){
		return sqrtf(this->P2(in));
	}

	__host__ __device__ float2 speed(float);
	__host__ __device__ float2 accel(float in);

	__host__ __device__ float curvature_radius(float in);
	__host__ __device__ pSpline_polynomial()
	{

	}
	pSpline_polynomial(float args[8]);
};



__host__ void pSpline(float2 points[], pSpline_polynomial f[32] , int pointcount, cusparseHandle_t * handle);


#endif /* SPLINE_GPU_HPP_ */
