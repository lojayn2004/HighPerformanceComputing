#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x) {
    return sin(x);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double a = 0.0, b = 1.0;  // Global bounds
    int n;                     // Total number of trapezoids

    if (rank == 0) {
        printf("Enter the number of trapezoids: \n");
        scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double h = (b - a) / n;    // Width of each trapezoid (global)

    // Calculate local bounds for this process
    int local_n = n / size;     // Base trapezoids per process
    int remainder = n % size;   // Extra trapezoids to distribute

    // Determine start and end indices for this process
    int st = rank * local_n + (rank < remainder ? rank : remainder);
    int e = st + local_n + (rank < remainder ? 1 : 0);

    // Compute local_a and local_b (starting and ending x-values)
    double local_a = a + st * h;
    double local_b = a + e * h;

    // Number of trapezoids this process handles
    int local_trapezoids = e - st;

    // Compute local integral (trapezoidal rule)
    double local_sum = 0.0;
    for (int i = 0; i < local_trapezoids; i++) {
        double x1 = local_a + i * h;
        double x2 = x1 + h;
        local_sum += (f(x1) + f(x2)) * 0.5 * h;  // Area of trapezoid
    }

    // Sum results from all processes
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Approximate integral: %.10f\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}
