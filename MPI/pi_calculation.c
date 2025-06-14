#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define PI 3.1415926535897932384626433832795029L

int main(int argc, char** argv) {
    int rank, size;
  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int n;
    double result = 0.0;
    
    if(rank == 0) {
       printf("Enter number of intervals: \n");
       scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int st , e;
    if(rank == 0) { // Master process processes remaining elements 
        st = 0; e = n / size + n % size;
    } else {
        st = rank * (n / size) + n % size;
        e = st + n / size;
    }
    double local_sum = 0.0;
    double x;
    for(int i = st + 1; i <= e;i++) {
        x = (i - 0.5) / n;
        local_sum += 4.0 / (1.0 + x * x);
    }
    MPI_Reduce(&local_sum, &result, 1, MPI_DOUBLE, MPI_SUM ,0, MPI_COMM_WORLD);
    
    if(rank == 0) {
       result /= n;
       printf ("Real PI: %.16Lf \n", PI);
       printf ("Computed PI: %.16f \n", result);
    }
    
    MPI_Finalize();
    return 0;
}
