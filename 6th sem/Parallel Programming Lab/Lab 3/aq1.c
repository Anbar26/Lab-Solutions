#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, m, nvm;
    int *arr = NULL, *local_arr = NULL;
    int *result = NULL, *local_result = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter value m: ");
        scanf("%d", &m);
        printf("Enter number of elements nvm: ");
        scanf("%d", &nvm);

        arr = (int *)malloc(nvm * sizeof(int));
        printf("Enter %d elements: ", nvm);
        for (int i = 0; i < nvm; i++) {
            scanf("%d", &arr[i]);
        }
    }

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nvm, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_len = nvm / size;
    if (rank < nvm % size) {
        local_len++; // Handle remainder
    }

    local_arr = (int *)malloc(local_len * sizeof(int));
    local_result = (int *)malloc(local_len * sizeof(int));

    MPI_Scatter(arr, local_len, MPI_INT, local_arr, local_len, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_len; i++) {
        int idx = rank * local_len + i;
        if (idx < nvm) {
            int exp = (idx / m) + 2; // square for the first m, cube for the next m, etc.
            local_result[i] = 1;
            for (int j = 0; j < exp; j++) {
                local_result[i] *= local_arr[i];
            }
        }
    }

    result = (int *)malloc(nvm * sizeof(int));
    MPI_Gather(local_result, local_len, MPI_INT, result, local_len, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Results:\n");
        for (int i = 0; i < nvm; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");

        free(arr);
        free(result);
    }

    free(local_arr);
    free(local_result);

    MPI_Finalize();
    return 0;
}
