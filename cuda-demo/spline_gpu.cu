/*
 * spline_gpu.cu
 *
 *  Created on: 2018/01/12
 *      Author: kazuki
 */

#include "helper_math.h"
#include "spline_gpu.hpp"
#include <stdio.h>

using namespace std;

//#define DEBUG

//http://www.yamamo10.jp/yamamoto/lecture/2006/5E/interpolation/interpolation_html/node3.html


//原点0,0
// pointcount は原点を含まない店の個数
__global__ void splineprep(float2 points[32], float temp_tdma_v[64], int pointcount =32) {

	float2 point = points[threadIdx.x];

	float2 point_prev = make_float2(__shfl_up(point.x,1),__shfl_up(point.y,1));

	if(threadIdx.x ==0) {
		point_prev.x = 0.0; point_prev.y = 0.0;
	}
	float2 dif = point - point_prev;

	float2 dif_next = make_float2(__shfl_down(dif.x,1), __shfl_down(dif.y,1));
	float2 difdif = dif_next - dif;

	// v_i/6
	temp_tdma_v[threadIdx.x] = difdif.x;
	temp_tdma_v[threadIdx.x+pointcount-1] = difdif.y;

}


__host__ __device__ float2 pSpline_polynomial::x_and_y(float in) {

	float t2=in*in;
	float t3=in*t2;
	return (this->a) * t3 + (this->b) * t2 + (this->c) * in + this->d;
}

__host__ __device__ float pSpline_polynomial::P2(float in)
{
	float t2=in*in;
	float x_ = ((this->a.x+this->a.x+this->a.x) * t2) + ((this->b.x+this->b.x) * in) + (this->c.x);
	float y_ = ((this->a.y+this->a.y+this->a.y) * t2) + ((this->b.y+this->b.y) * in) + (this->c.y);
	return ( x_ * x_ + y_ * y_ );
}

__host__ __device__ float pSpline_polynomial::curvature_radius(float in)
{

	float t2=in*in;
	float x_ = ((this->a.x+this->a.x+this->a.x) * t2) + ((this->b.x+this->b.x) * in) + (this->c.x);
	float x__ = (this->a.x * 6) * in + (this->b.x+this->b.x);
	float y_ = ((this->a.y+this->a.y+this->a.y) * t2) + ((this->b.y+this->b.y) * in) + (this->c.y);
	float y__ = (this->a.y * 6) * in + (this->b.y+this->b.y);

	float p2 ( x_ * x_ + y_ * y_ );
	float tmp = powf(p2,1.5);
	return tmp / (x_ * y__ - y_ * x__);
}
__host__ __device__ float2 pSpline_polynomial::accel(float in)
{
	float t2=in*in;
	float x__ = (this->a.x * 6) * in + (this->b.x+this->b.x);
	float y__ = (this->a.y * 6) * in + (this->b.y+this->b.y);
	float2 r = {X__,y__};
	return r;
}
__host__ __device__ float2 pSpline_polynomial::speed(float in)
{

	float t2=in*in;
	float x_ = ((this->a.x+this->a.x+this->a.x) * t2) + ((this->b.x+this->b.x) * in) + (this->c.x);
	float y_ = ((this->a.y+this->a.y+this->a.y) * t2) + ((this->b.y+this->b.y) * in) + (this->c.y);

	float2 r = {x_,y_};
	return r;
}

pSpline_polynomial::pSpline_polynomial(float args[8])
{
	this->a = { args[0] ,args[4] };
	this->b = { args[1] ,args[5] };
	this->c = { args[2] ,args[6] };
	this->d = { args[3] ,args[7] };
}



//static __device__ float out_x[16], out_y[16];


__global__ void spline(float2 points[], pSpline_polynomial f[32], int pointcount, float _tdma_u[64])
{
	float2 point = points[threadIdx.x];

	float2 point_prev = make_float2(__shfl_up(point.x,1),__shfl_up(point.y,1));

	__shared__ float u[2][33];
	u[0][threadIdx.x+1] = _tdma_u[threadIdx.x];
	u[1][threadIdx.x+1] = _tdma_u[threadIdx.x+pointcount-1]; //segfault注意

	if(threadIdx.x ==0) {
		point_prev.x = 0.0; point_prev.y = 0.0;
		u[0][0] = 0.0;
		u[0][pointcount] = 0.0;
		u[1][0] = 0.0;
		u[1][pointcount] = 0.0;
	}

	float2 dif = point - point_prev;


	pSpline_polynomial out;

	out.a.x = u[0][threadIdx.x + 1] -u[0][threadIdx.x];
	out.a.y = u[1][threadIdx.x + 1] -u[1][threadIdx.x];
	out.b.x = u[0][threadIdx.x] * 3.0;
	out.b.y = u[1][threadIdx.x] * 3.0;
	out.c.x = dif.x -(u[0][threadIdx.x]+u[0][threadIdx.x]+u[0][threadIdx.x+1]);
	out.c.y = dif.y -(u[1][threadIdx.x]+u[1][threadIdx.x]+u[1][threadIdx.x+1]);
	out.d = point_prev;
	out.start = point_prev;
	out.end = dif;

	const int integration_part = 2;

	float section[integration_part][3] = {0};
#pragma unroll
	for (int var = 0; var < integration_part; ++var) {
		section[var][0] = (float)(var) / (float)integration_part;
		section[var][2] = (float)(var+1) / (float)integration_part;
		section[var][1] = (section[var][0] + section[var][2]) /2.0;
	}
	/*
	 * 行列で工夫できたらいいと思った時があった
	__threadfence_block();
	*/
	out.length=0.0;
#pragma unroll
	for (int var = 0; var < integration_part; ++var) {
		out.length += (out.P(section[var][0])
						+ 4.0 * out.P(section[var][1])
							+ out.P(section[var][2]))
						* (section[var][2] - section[var][0]);
	}

	out.length /= 6.0;

	f[threadIdx.x] =out;

}

const float dl[32] = {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,};
float du[32] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
				1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
				1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
				1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0,};
float d[32] = {4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0,
			4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0,
			4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0,
			4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, };

__host__ void pSpline(float2 points[], pSpline_polynomial f[32] , int pointcount, cusparseHandle_t * handle)
{

	float2* pts;

	cudaMalloc(&pts, sizeof(float2) * pointcount);

	cudaMemcpy(pts,points,sizeof(float2) * pointcount,cudaMemcpyHostToDevice);

	float* temp_tdma_v;

	cudaMalloc(&temp_tdma_v , sizeof(float) * 64);
	#ifdef DEBUG
	printf("(%f,%f),",0.0,0.0);
	for (int var = 0; var < pointcount; ++var) {
		printf("(%f,%f),",points[var].x,points[var].y);
	}
	#endif


	splineprep<<<1,pointcount >>>(pts, temp_tdma_v,pointcount);

#ifdef DEBUG
	printf("\ninfo: Prep completed\n");
#endif

	pSpline_polynomial *p;
	cudaMalloc((void**)&p, sizeof(pSpline_polynomial) * pointcount);

	float* dl_cuda;
	float* du_cuda;
	float* d_cuda;
	cudaMalloc(&dl_cuda, sizeof(float) * (pointcount-1));
	cudaMalloc(&du_cuda, sizeof(float) * (pointcount-1));
	cudaMalloc(&d_cuda, sizeof(float) * (pointcount-1));
	cudaMemcpy(dl_cuda, dl, sizeof(float) * (pointcount-1),cudaMemcpyHostToDevice);
	cudaMemcpy(du_cuda, du, sizeof(float) * (pointcount-1),cudaMemcpyHostToDevice);
	cudaMemcpy(d_cuda, d, sizeof(float) * (pointcount-1),cudaMemcpyHostToDevice);


	cudaDeviceSynchronize();



#ifdef DEBUG

	puts("temp_tdma_v");
	float matrix[64];
	cudaMemcpy(matrix,temp_tdma_v,(pointcount-1)*2*sizeof(float), cudaMemcpyDeviceToHost);
	for (int var = 0; var < (pointcount-1)*2; ++var) {
		printf("%f\n",matrix[var]);
	}

#endif


	//printf("%x %x %x %x\n",dl_cuda, d_cuda,du_cuda, temp_tdma_v);


    cusparseStatus_t x = cusparseSgtsv(*handle, pointcount - 1, 2, dl_cuda, d_cuda, du_cuda, temp_tdma_v, pointcount - 1);

#ifdef DEBUG
	puts("temp_tdma_v(after gtsv)");
    cudaMemcpy(matrix,temp_tdma_v,(pointcount-1)*2*sizeof(float), cudaMemcpyDeviceToHost);
   	for (int var = 0; var < (pointcount-1)*2; ++var) {
   		printf("%f\n",matrix[var]);
   	}
#endif

    if (x!=0)
    	printf("err %d\n" , x);

	cudaDeviceSynchronize();

	spline<<<1,pointcount>>>(pts, p, pointcount,temp_tdma_v);
	cudaDeviceSynchronize();
	cudaFree(temp_tdma_v);

	cudaMemcpy(f,p,sizeof(pSpline_polynomial)*pointcount, cudaMemcpyDeviceToHost);

	cudaFree(p);
	cudaFree(dl_cuda);
	cudaFree(du_cuda);
	cudaFree(d_cuda);

	cudaFree(pts);
}

