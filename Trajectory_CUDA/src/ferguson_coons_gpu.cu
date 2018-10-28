/*
 * ferguson_coons_gpu.cu
 *
 *  Created on: 2018/04/17
 *      Author: dvr1
 */


#include <stdio.h>
#include "helper_math.h"

#include "spline_gpu.hcu"

#include "ferguson_coons_gpu.hcu"


const float mat[16] =
{
		2, -2,  1,  1,
		-3,  3, -2, -1,
		0,  0,  1,  0,
		1,  0,  0,  0,
};

__host__ pSpline_polynomial Ferguson(cublasHandle_t* handle, float2 v0, float2 v1, float2 x0, float2 x1)
{

	float _endpoint[8] = { x0.x, x1.x, v0.x, v1.x, x0.y, x1.y, v0.y, v1.y };

	float* points_gpu;
	float* matrix_gpu;

	float* output;

	cudaMalloc(&output,sizeof(float) * 8);
	cudaMalloc(&points_gpu,sizeof(float) * 8);
	cudaMalloc(&matrix_gpu,sizeof(float) * 18);

	cublasSetMatrix(4,2,sizeof(float),_endpoint, 4, points_gpu, 4);
	cublasSetMatrix(4,4,sizeof(float),mat, 4, matrix_gpu, 4);

	
	float coef=1.0;
	float coef2 =0.0;

	cublasStatus_t r = cublasSgemm_v2(*handle, CUBLAS_OP_T, CUBLAS_OP_N, 4,2,4,&coef,matrix_gpu,4,points_gpu,4,&coef2,output,4);

	float out_host[8];

	cudaMemcpy(out_host, output, sizeof(float) * 8, cudaMemcpyDeviceToHost);

	pSpline_polynomial x(out_host);

	cudaFree(output);
	cudaFree(points_gpu);
	cudaFree(matrix_gpu);

	return x;
}
