#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void odd_phase(int *arr, int width) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx % 2 == 1 && idx < width - 1) {
        if (arr[idx] > arr[idx + 1]) {
            int temp = arr[idx];
            arr[idx] = arr[idx + 1];
            arr[idx + 1] = temp;
        }
    }
}

__global__ void even_phase(int *arr, int width) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx % 2 == 0 && idx < width - 1) {
        if (arr[idx] > arr[idx + 1]) {
            int temp = arr[idx];
            arr[idx] = arr[idx + 1];
            arr[idx + 1] = temp;
        }
    }
}

void odd_even_transposition_sort(int *arr, int width) {
    int *d_arr;
    
    cudaMalloc((void**)&d_arr, width * sizeof(int));
    cudaMemcpy(d_arr, arr, width * sizeof(int), cudaMemcpyHostToDevice);

    int block_size = 256;
    int grid_size = (width + block_size - 1) / block_size;

    int sorted = 0;
    while (!sorted) {
        sorted = 1;

        odd_phase<<<grid_size, block_size>>>(d_arr, width);
        cudaDeviceSynchronize();

        even_phase<<<grid_size, block_size>>>(d_arr, width);
        cudaDeviceSynchronize();

        int *h_arr = (int*)malloc(width * sizeof(int));
        cudaMemcpy(h_arr, d_arr, width * sizeof(int), cudaMemcpyDeviceToHost);
        
        for (int i = 1; i < width; i++) {
            if (h_arr[i - 1] > h_arr[i]) {
                sorted = 0;
                break;
            }
        }
        
        free(h_arr);
    }

    cudaMemcpy(arr, d_arr, width * sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(d_arr);
}

int main() {
    int width;

    printf("Enter the size of the array: ");
    scanf("%d", &width);

    int *arr = (int*)malloc(width * sizeof(int));
    printf("Enter the elements of the array:\n");
    for (int i = 0; i < width; ++i) {
        scanf("%d", &arr[i]);
    }

    printf("Original Array: ");
    for (int i = 0; i < width; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    odd_even_transposition_sort(arr, width);

    printf("Sorted Array: ");
    for (int i = 0; i < width; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
