#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

__global__ void ones_complement(int *arr, int *result, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        result[idx] = ~arr[idx];
    }
}

int binary_to_decimal(const char *binary) {
    int decimal = 0;
    for (int i = 0; binary[i] != '\0'; i++) {
        decimal = (decimal << 1) | (binary[i] - '0');
    }
    return decimal;
}

void decimal_to_binary(int decimal, char *binary) {
    for (int i = 31; i >= 0; i--) {
        binary[31 - i] = (decimal & (1 << i)) ? '1' : '0';
    }
    binary[32] = '\0';
}

int main() {
    int n;

    printf("Enter the number of binary numbers: ");
    scanf("%d", &n);

    char **arr = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; ++i) {
        arr[i] = (char *)malloc(33 * sizeof(char));
    }

    printf("Enter the binary numbers (as strings, e.g., 1010):\n");
    for (int i = 0; i < n; ++i) {
        scanf("%s", arr[i]);
    }

    int *decimal_arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        decimal_arr[i] = binary_to_decimal(arr[i]);
    }

    int *result = (int *)malloc(n * sizeof(int));
    int *d_arr, *d_result;

    cudaMalloc((void**)&d_arr, n * sizeof(int));
    cudaMalloc((void**)&d_result, n * sizeof(int));

    cudaMemcpy(d_arr, decimal_arr, n * sizeof(int), cudaMemcpyHostToDevice);

    int block_size = 256;
    int grid_size = (n + block_size - 1) / block_size;
    ones_complement<<<grid_size, block_size>>>(d_arr, d_result, n);

    cudaDeviceSynchronize();

    cudaMemcpy(result, d_result, n * sizeof(int), cudaMemcpyDeviceToHost);

    printf("Original binary numbers and their ones' complements (in binary):\n");
    for (int i = 0; i < n; ++i) {
        char original_binary[33], complement_binary[33];
        decimal_to_binary(decimal_arr[i], original_binary);
        decimal_to_binary(result[i], complement_binary);
        printf("Original: %s -> One's complement: %s\n", original_binary, complement_binary);
    }

    for (int i = 0; i < n; ++i) {
        free(arr[i]);
    }
    free(arr);
    free(decimal_arr);
    free(result);
    cudaFree(d_arr);
    cudaFree(d_result);

    return 0;
}
