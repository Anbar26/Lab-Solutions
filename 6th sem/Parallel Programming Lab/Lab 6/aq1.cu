#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__device__ int decimal_to_octal(int num) {
    int octal = 0, place = 1;
    while (num > 0) {
        octal += (num % 8) * place;
        num /= 8;
        place *= 10;
    }
    return octal;
}

__global__ void convert_to_octal(int *arr, int *result, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        result[idx] = decimal_to_octal(arr[idx]);
    }
}

int main() {
    int n;

    printf("Enter the number of integers: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    printf("Enter the integers:\n");
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }

    int *result = (int *)malloc(n * sizeof(int));
    int *d_arr, *d_result;

    cudaMalloc((void**)&d_arr, n * sizeof(int));
    cudaMalloc((void**)&d_result, n * sizeof(int));

    cudaMemcpy(d_arr, arr, n * sizeof(int), cudaMemcpyHostToDevice);

    int block_size = 256;
    int grid_size = (n + block_size - 1) / block_size;
    convert_to_octal<<<grid_size, block_size>>>(d_arr, d_result, n);

    cudaDeviceSynchronize();

    cudaMemcpy(result, d_result, n * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Original integers and their octal equivalents:\n");
    for (int i = 0; i < n; ++i) {
        printf("Decimal: %d -> Octal: %d\n", arr[i], result[i]);
    }

    free(arr);
    free(result);
    cudaFree(d_arr);
    cudaFree(d_result);

    return 0;
}
