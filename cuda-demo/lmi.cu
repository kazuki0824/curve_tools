
#define SIZE 3
#define THREADX 3
#define THREADY 3
#define BLOCKX (SIZE/THREADX)
#define BLOCKY (SIZE/THREADY)
__global__ void matmulGPU(const float *A, const float *B, float *C) {
	int i,j,k;
	float
	sum=0.0f;
	int tx;
	__shared__
	float
	sB[THREADX];
	i	=
			blockIdx.x*blockDim.x+threadIdx.x;
	j	=
			blockIdx.y*blockDim.y+threadIdx.y;
	tx  =	threadIdx.x;
	for (k = 0; k < SIZE; k += THREADX) {
		sB[tx] = B[(k + tx) + SIZE * j];
		__syncthreads();
		for (int w = 0; w < THREADX; w++) {
			sum += A[i + SIZE * (k + w)] * sB[w];
		}
		__syncthreads();
	}
	C[i+SIZE*j] = sum;
}
