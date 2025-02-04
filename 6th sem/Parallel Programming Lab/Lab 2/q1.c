//lab2 q1
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, x;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    if (size != 2) {
        if (rank == 0) {
            printf("This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {

        printf("Enter a value in master process (Process 0): ");
        scanf("%d", &x);
        
        
        MPI_Ssend(&x, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printf("I have sent %d from process 0\n", x);
    } 
    else if (rank == 1) {
        
        MPI_Recv(&x, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("I have received %d in process 1\n", x);
              
        x = x * 2;        
        
        MPI_Ssend(&x, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        printf("Process 1 has sent modified value %d back to process 0\n", x);
    }

    if (rank == 0) {
        MPI_Recv(&x, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, &status);
        printf("Process 0 received modified value %d from process 1\n", x);
    }

    MPI_Finalize();
    return 0;
}
