#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

long long factorial(int n) {
    long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int n; 
    int *values = NULL; 
    long long *factorials = NULL; 
    long long sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {

        printf("Enter the number of values: ");
        scanf("%d", &n);

        if (n != size) {
            printf("The number of processes should be equal to the number of values.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        values = (int *)malloc(n * sizeof(int));
        printf("Enter %d values:\n", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &values[i]);
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int value;
    if (rank != 0) {
        factorials = (long long *)malloc(sizeof(long long));
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&values[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        value = values[0];
    } else {
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    long long factorial_result = factorial(value);

    if (rank == 0) {
        factorials = (long long *)malloc(n * sizeof(long long));
        factorials[0] = factorial_result;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&factorials[i], 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for (int i = 0; i < n; i++) {
            sum += factorials[i];
        }

        printf("Sum of factorials: %lld\n", sum);

        free(values);
        free(factorials);
    } else {
        MPI_Send(&factorial_result, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
        free(factorials);
    }

    MPI_Finalize();

    return 0;
}
