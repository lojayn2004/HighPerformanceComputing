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

    if (rank == 0) {
       int* array = (int*) malloc(sizeof(int) * 4);
       
       for(int i = 0;i < 4;i++) 
          array[i] = i;
          
        array[0] += rank;
        
        for(int i = 1;i < size;i++) {
           MPI_Send(&array[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        for(int i = 1;i < size;i++) {
           int recv;
           MPI_Recv(&recv, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           array[i] = recv;
        }
        
        for(int i = 0;i < 4;i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
       
       free(array);
      
    } else {
       int recv = 0;
       MPI_Recv(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       
       recv += rank;
       
       MPI_Send(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
