#include <stdio.h>
#include <mpi.h>

void ErrorHandler(int error_code) {
    char error_string[MPI_MAX_ERROR_STRING];
    int length_of_error_string, error_class;
    MPI_Error_class(error_code, &error_class);
    MPI_Error_string(error_code, error_string, &length_of_error_string);
    if (error_code != 0) {
        printf("Error class %d \nError string: %s\n", error_class, error_string);
    }
}

int main(int argc, char *argv[]) {
    int rank, ele, size;
    int a1[3][3];
    int b1[3];
    int count = 0;
    int tc = 0;
    int error_code;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

    error_code = MPI_Comm_size(MPI_COMM_WORLD, &size);
    ErrorHandler(error_code);

    if (rank == 0) {
        printf("Enter elements into matrix\n");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                scanf("%d", &a1[i][j]);
            }
        }
        printf("Enter element to count \n");
        scanf("%d", &ele);
    }

    MPI_Bcast(&ele, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(a1, 3, MPI_INT, b1, 3, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 3; i++) {
        printf("%d ", b1[i]);
        if (b1[i] == ele) {
            count++;
        }
    }

    printf("\nProcess %d found %d occurrences\n", rank, count);
    MPI_Reduce(&count, &tc, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nNumber of occurrences is %d\n", tc);
    }

    MPI_Finalize();
}