#include "mpi.h"
#include<stdio.h>
#include<math.h>

int isPrime(int n){
    if(n <=1){
        return 0;
    }
    for(int i = 2; i<= sqrt(n); i++){
        if(n%i ==0){
            return 0;
        }
    }
    return 1;
}
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("prime numbers from 1 t 100\n");
    for(int i=1; i<=100; i++){
        if(isPrime(i)){
            printf("%d\n", i);
        }
    }
    MPI_Finalize();
    return 0;
}