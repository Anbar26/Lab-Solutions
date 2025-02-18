#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void convolution_kernel(float *N, float *M, float *P, int width, int mask_width) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    int half_mask_width = mask_width / 2;

    if (idx < width) {
        float sum = 0.0f;
        for (int j = 0; j < mask_width; j++) {
            int input_index = idx + j - half_mask_width;
            if (input_index >= 0 && input_index < width) {
                sum += N[input_index] * M[j];
            }
        }
        P[idx] = sum;
    }
}

int main() {
    int width, mask_width;

    printf("Enter the size of the input array (width): ");
    scanf("%d", &width);

    printf("Enter the size of the mask (mask_width): ");
    scanf("%d", &mask_width);

    float *N = (float*)malloc(width * sizeof(float));
    float *M = (float*)malloc(mask_width * sizeof(float));
    float *P = (float*)malloc(width * sizeof(float));

    printf("Enter the elements of the input array N:\n");
    for (int i = 0; i < width; ++i) {
        scanf("%f", &N[i]);
    }

    printf("Enter the elements of the mask M:\n");
    for (int i = 0; i < mask_width; ++i) {
        scanf("%f", &M[i]);
    }

    float *d_N, *d_M, *d_P;
    cudaMalloc((void**)&d_N, width * sizeof(float));
    cudaMalloc((void**)&d_M, mask_width * sizeof(float));
    cudaMalloc((void**)&d_P, width * sizeof(float));

    cudaMemcpy(d_N, N, width * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_M, M, mask_width * sizeof(float), cudaMemcpyHostToDevice);

    int block_size = 256; 
    int grid_size = (width + block_size - 1) / block_size; 

    convolution_kernel<<<grid_size, block_size>>>(d_N, d_M, d_P, width, mask_width);

    cudaMemcpy(P, d_P, width * sizeof(float), cudaMemcpyDeviceToHost);

    cudaDeviceSynchronize();

    printf("Resultant Convolution Output P:\n");
    for (int i = 0; i < width; i++) {
        printf("%f ", P[i]);
    }
    printf("\n");

    free(N);
    free(M);
    free(P);
    cudaFree(d_N);
    cudaFree(d_M);
    cudaFree(d_P);

    return 0;
}
