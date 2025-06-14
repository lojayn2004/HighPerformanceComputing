/*
Write a program that will run on 4 processes that uses a one-dimensional array of size 4. The
first process sends each process i the element i of the array and keeps the first element for
itself. Each process will add its rank to the number it received and sends it back to the first
process. The first process will then collect the received numbers and prints the new array.
*/


#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* array = (int*) malloc(sizeof(int) * 4);
    int gathered_res[4];
    if (rank == 0) {
       for(int i = 0;i < 4;i++) 
          array[i] = i;
    } 
    
    MPI_Bcast(array, 4, MPI_INT, 0, MPI_COMM_WORLD);
    int recv;
    MPI_Scatter(array, 1, MPI_INT, &recv, 1, MPI_INT, 0,  MPI_COMM_WORLD);
    
    recv += rank;
    
    MPI_Gather(&recv, 1, MPI_INT, gathered_res, 1, MPI_INT, 0,  MPI_COMM_WORLD);
    
    if(rank == 0) {
       for(int i = 0;i < 4;i++) {
           printf("%d ", gathered_res[i]);
       }
       printf("\n");
    
    }
    free(array);
    MPI_Finalize();
    return 0;
}
