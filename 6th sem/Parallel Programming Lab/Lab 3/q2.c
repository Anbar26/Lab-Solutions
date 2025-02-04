#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, m, n;
    double *data = NULL, *local_data = NULL, *averages = NULL, total_sum = 0, total_average;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter the number of processes (n): ");
        scanf("%d", &n);
        printf("Enter the number of elements per process (m): ");
        scanf("%d", &m);

        data = (double *)malloc(n * m * sizeof(double));
        printf("Enter the %d * %d elements:\n", n, m);
        for (int i = 0; i < n * m; i++) {
            scanf("%lf", &data[i]);
        }
    }

    local_data = (double *)malloc(m * sizeof(double));
    
    MPI_Scatter(data, m, MPI_DOUBLE, local_data, m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double local_sum = 0;
    for (int i = 0; i < m; i++) {
        local_sum += local_data[i];
    }
    double local_average = local_sum / m;

    if (rank == 0) {
        averages = (double *)malloc(n * sizeof(double));
    }
    MPI_Gather(&local_average, 1, MPI_DOUBLE, averages, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            total_sum += averages[i] * m;
        }
        total_average = total_sum / (n * m);
        printf("Total average: %lf\n", total_average);
        
        free(data);
        free(averages);
    }

    free(local_data);

    MPI_Finalize();

    return 0;
}
