#lab1 q4
#include "mpi.h"
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[]){
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	char str[5] = {'H', 'E', 'L', 'L', 'O'};  

	if(rank < strlen(str)){
		str[rank] = str[rank] + 32;
	}

	printf("Process%d: ", rank);
	for(int i=0; i<strlen(str); i++){
		printf("%c", str[i]);
	}
	printf("\n");

	MPI_Finalize();
	return 0;
}