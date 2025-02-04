#lab1 q5
#include "mpi.h"
#include<stdio.h>

int factorial(int n){
	if(n==1 || n==0) return 1;
	return n * factorial(n-1);
}

int fibonacci(int n){
	if(n==1) return 0;
	if(n==2) return 1;
	return fibonacci(n-1) + fibonacci(n-2);
}


int main(int argc, char* argv[]){
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank%2 == 0 ) printf("Process%d-factorial : %d\n", rank, factorial(rank));
	else if(rank%2 == 1) printf("Process%d-fibonacci : %d\n", rank, fibonacci(rank));

	MPI_Finalize();
	return 0;
}