#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#define THREADS_PER_BLOCK 256
__global__ void vectorAdd(int *a, int *b, int *c, int n) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    if (index < n) {
        c[index] = a[index] + b[index];
    }
}
int main() {
    int n;
    printf("Enter the number of elements in the vectors: ");
    scanf("%d", &n);
    int *h_a = (int*)malloc(n * sizeof(int));
    int *h_b = (int*)malloc(n * sizeof(int));
    int *h_c = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        h_a[i] = i;
        h_b[i] = i * 2;
    }
    printf("Initial vector A: ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", h_a[i]);
    }
    printf("\n");

    printf("Initial vector B: ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", h_b[i]);
    }
    printf("\n");
    int *d_a, *d_b, *d_c;
    cudaMalloc((void**)&d_a, n * sizeof(int));
    cudaMalloc((void**)&d_b, n * sizeof(int));
    cudaMalloc((void**)&d_c, n * sizeof(int));
    cudaMemcpy(d_a, h_a, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, n * sizeof(int), cudaMemcpyHostToDevice);
    int numBlocks = (n + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    vectorAdd<<<numBlocks, THREADS_PER_BLOCK>>>(d_a, d_b, d_c, n);
    cudaMemcpy(h_c, d_c, n * sizeof(int), cudaMemcpyDeviceToHost);
    printf("Resulting vector after addition: ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", h_c[i]);
    }
    printf("\n");
    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
