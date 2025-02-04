#lab1 q3
#include "mpi.h"
#include<stdio.h>
#include<math.h>

int main(int argc, char* argv[]){
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int operation = rank;

	int a = 5, b = 3;

	switch(operation){
	case 0:
		printf("Rank %d: Add: %d\n",rank, a+b);
		break;
	case 1:
		printf("Rank %d: Subtract: %d\n",rank, a-b);
		break;
	case 2:
		printf("Rank %d: Multiply: %d\n",rank, a*b);
		break;
	case 3:
		printf("Rank %d: Divide: %d\n",rank, a/b);
		break;
	case 4:
		printf("Rank %d: Power: %d",rank, pow(a, b));
		break;
	default:
		printf("Invalid!");
		break;
	}

	MPI_Finalize();
	return 0;
}