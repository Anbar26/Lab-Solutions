//q4
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, size, str_len;
    char *s1 = NULL, *s2 = NULL, *result = NULL;
    char *local_s1 = NULL, *local_s2 = NULL, *local_result = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter string s1: ");
        s1 = (char *)malloc(100 * sizeof(char));
        fgets(s1, 100, stdin);
        s1[strcspn(s1, "\n")] = '\0';
        printf("Enter string s2: ");
        s2 = (char *)malloc(100 * sizeof(char));
        fgets(s2, 100, stdin);
        s2[strcspn(s2, "\n")] = '\0';

        str_len = strlen(s1);
        if (str_len != strlen(s2)) {
            printf("Strings must be of equal length.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_len = str_len / size;
    if (rank < str_len % size) {
        local_len++; // Handle remainder
    }

    local_s1 = (char *)malloc(local_len * sizeof(char));
    local_s2 = (char *)malloc(local_len * sizeof(char));
    local_result = (char *)malloc(local_len * sizeof(char));

    MPI_Scatter(s1, local_len, MPI_CHAR, local_s1, local_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatter(s2, local_len, MPI_CHAR, local_s2, local_len, MPI_CHAR, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_len; i++) {
        local_result[i] = (rank % 2 == 0) ? local_s1[i] : local_s2[i];
    }

    result = (char *)malloc(str_len * sizeof(char));
    MPI_Gather(local_result, local_len, MPI_CHAR, result, local_len, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        result[str_len] = '\0';
        printf("Resultant string: %s\n", result);

        free(s1);
        free(s2);
        free(result);
    }

    free(local_s1);
    free(local_s2);
    free(local_result);

    MPI_Finalize();
    return 0;
}
