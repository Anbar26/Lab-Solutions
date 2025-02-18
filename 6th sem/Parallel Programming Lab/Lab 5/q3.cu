#include <stdio.h>
#include <cuda_runtime.h>
#include <math.h>

__global__ void computeSine(float *angles, float *sines, int N) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < N) {
        sines[idx] = sinf(angles[idx]); 
    }
}

int main() {
    int N;

    printf("Enter the number of angles: ");
    scanf("%d", &N);

    int size = N * sizeof(float);

    float *h_angles = (float*)malloc(size);
    float *h_sines = (float*)malloc(size);

    printf("Enter the angles in radians:\n");
    for (int i = 0; i < N; i++) {
        printf("Angle %d: ", i + 1);
        scanf("%f", &h_angles[i]);
    }

    printf("\nAngles in Radians:\n");
    for (int i = 0; i < N; i++) {
        printf("%f ", h_angles[i]);
    }
    printf("\n");

    float *d_angles, *d_sines;
    cudaMalloc((void**)&d_angles, size);
    cudaMalloc((void**)&d_sines, size);

    cudaMemcpy(d_angles, h_angles, size, cudaMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (N + blockSize - 1) / blockSize;

    computeSine<<<numBlocks, blockSize>>>(d_angles, d_sines, N);

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA error: %s\n", cudaGetErrorString(err));
    }

    cudaMemcpy(h_sines, d_sines, size, cudaMemcpyDeviceToHost);

    printf("\nSine of Angles:\n");
    for (int i = 0; i < N; i++) {
        printf("%f ", h_sines[i]);
    }
    printf("\n");

    cudaFree(d_angles);
    cudaFree(d_sines);
    free(h_angles);
    free(h_sines);

    return 0;
}
