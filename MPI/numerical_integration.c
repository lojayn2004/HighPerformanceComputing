#include <stdio.h>
#include <math.h>
#include <mpi.h>

// Function to integrate could be any function we want (sin(x), x*x, ....)
double f(double x) {
    return sin(x); 
}
int main(int arg, char** argv) {
    int size, rank;
    MPI_Init(&arg, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    double a = 0.0, b = 1.0; // Integration bounds
    double result = 0.0;
    int n;      // Number of trapezoids
    
    if(rank == 0) {
         printf("Enter number of trapezoids: \n");
         scanf("%d", &n);
         
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double h = (b - a) / n; 
    int st, e;
    if(rank == 0) {
        st = 0;
        e = n / size + n % size;
    }
    else {
        st = rank * (n / size) + n % size;
        e = st + n / size;
        
    }
    
    double x, sum = 0.0;
    for (int i = st; i < e; i++) {
        x = a + i * h;
        sum += f(x);
    }
    MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0) {
        // Add End points to intervals
        result += 0.5 * (f(a) + f(b)); 
        result = result * h;
        printf("Approximate integral: %.10f\n", result);
    } 
    MPI_Finalize();
    return 0;
}
