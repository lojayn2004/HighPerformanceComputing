// Multiplying a 3x1 vector with a scalar while taking the input from the command line 
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {

    if(argc != 5) {
        printf("Input count is not correct \n");
        return 0;
    }
  
    int vect[3];
    int gatheredVect[3];
    int scalar, recv;
    int processId = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    // Root process initalizes the main variables used by other processes 
    if(processId == 0) {  
         scalar = atoi(argv[1]);   
         vect[0] = atoi(argv[2]);
         vect[1] = atoi(argv[3]);
         vect[2] = atoi(argv[4]);
         
    }
    // Bcasting the scalar variable so all processes can access the correct value
    MPI_Bcast(&scalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // scattering the vector accross processes so each process recieves an element
    MPI_Scatter(vect, 1, MPI_INT, &recv, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Doing the computation 
    recv *= scalar;

    // Gathering the result from each process into << gathered vector>>
    MPI_Gather(&recv, 1, MPI_INT, gatheredVect, 1, MPI_INT, 0, MPI_COMM_WORLD);
     if(processId == 0) {
          printf(" %d * (%d,  %d, %d) = ", scalar, vect[0], vect[1], vect[2]);
          printf("( %d, %d, %d) \n", gatheredVect[0], gatheredVect[1],gatheredVect[2]);
     } 
    MPI_Finalize();    
}
