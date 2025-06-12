#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    double start = MPI_Wtime();  // Wall-clock start

    // Simulate work (uneven per rank)
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (int i = 0; i < 100000000 * (rank + 1); i++);

    double end = MPI_Wtime();  // Wall-clock end
    printf("Rank %d: Wall-clock time = %.6f sec\n", rank, end - start);

    MPI_Finalize();
    return 0;
}
