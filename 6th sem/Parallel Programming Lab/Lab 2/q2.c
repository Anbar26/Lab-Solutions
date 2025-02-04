//lab2 q2
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, number;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    if (size < 2) {
        if (rank == 0) {
            printf("This program requires at least 2 processes (1 master and 1 slave).\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        
        printf("Enter a number to send to the slaves: ");
        scanf("%d", &number);

        for (int i = 1; i < size; i++) {
            MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("Master (Process 0) sent number %d to Process %d\n", number, i);
        }
    } 
    else {
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        printf("Slave Process %d received number %d\n", rank, number);
    }

    MPI_Finalize();
    return 0;
}
