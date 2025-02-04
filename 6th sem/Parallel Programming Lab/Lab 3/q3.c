#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <ctype.h>

int is_non_vowel(char c) {
    char lower = tolower(c);
    return (lower >= 'a' && lower <= 'z') && !(lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u');
}

int main(int argc, char *argv[]) {
    int rank, size, str_len, *counts = NULL, *displacements = NULL, local_count = 0, *total_count = NULL;
    char *str = NULL, *local_str = NULL;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter a string: ");
        str = (char *)malloc(100 * sizeof(char));
        fgets(str, 100, stdin);
        str_len = strlen(str) - 1;  // Exclude the newline character
    }

    MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int base_len = str_len / size;
    int remainder = str_len % size;

    counts = (int *)malloc(size * sizeof(int));
    displacements = (int *)malloc(size * sizeof(int));

    // Distribute the counts and displacements to each process
    for (int i = 0; i < size; i++) {
        counts[i] = base_len + (i < remainder ? 1 : 0);  // Handle remainder
        displacements[i] = i * base_len + (i < remainder ? i : remainder);  // Calculate displacement
    }

    local_str = (char *)malloc(counts[rank] * sizeof(char));

    MPI_Scatterv(str, counts, displacements, MPI_CHAR, local_str, counts[rank], MPI_CHAR, 0, MPI_COMM_WORLD);

    for (int i = 0; i < counts[rank]; i++) {
        if (is_non_vowel(local_str[i])) {
            local_count++;
        }
    }

    total_count = (int *)malloc(size * sizeof(int));

    MPI_Gather(&local_count, 1, MPI_INT, total_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            printf("Process %d found %d non-vowels\n", i, total_count[i]);
        }

        int total_non_vowels = 0;
        for (int i = 0; i < size; i++) {
            total_non_vowels += total_count[i];
        }
        printf("Total number of non-vowels: %d\n");

        free(str);
        free(total_count);
    }

    free(local_str);
    free(counts);
    free(displacements);
    MPI_Finalize();
    return 0;
}
