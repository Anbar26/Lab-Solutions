#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, n;
    int *A = NULL, *local_A = NULL;
    int local_even_count = 0, local_odd_count = 0;
    int total_even_count = 0, total_odd_count = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter the size of the array (n): ");
        scanf("%d", &n);

        A = (int *)malloc(n * sizeof(int));
        printf("Enter %d elements for the array:\n", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &A[i]);
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = n / size;
    local_A = (int *)malloc(local_n * sizeof(int));

    MPI_Scatter(A, local_n, MPI_INT, local_A, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++) {
        if (local_A[i] % 2 == 0) {
            local_A[i] = 1;
            local_even_count++;
        } else {
            local_A[i] = 0;
            local_odd_count++;
        }
    }

    MPI_Gather(local_A, local_n, MPI_INT, A, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Reduce(&local_even_count, &total_even_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_odd_count, &total_odd_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Modified array:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", A[i]);
        }
        printf("\n");

        printf("Total number of even elements: %d\n", total_even_count);
        printf("Total number of odd elements: %d\n", total_odd_count);

        free(A);
    }

    free(local_A);
    MPI_Finalize();
    return 0;
}
