#include <mpi.h>
#include <stdio.h>


int main(int argc, char **argv) {

     int processId;
     MPI_Init(&argc, &argv);

     MPI_Comm_rank(MPI_COMM_WORLD, &processId);
     printf("Hello I am a process My Id is %d\n", processId);

     MPI_Finalize();
      
     return 0;
}
