#include "mpi.h"
#include<stdio.h>
#include<math.h>
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int a = 5;
    int p = pow(a,rank);
    printf("Integer: %d Rank: %d : Power: %d \n", a, rank, p);
    MPI_Finalize();
    return 0;
}
