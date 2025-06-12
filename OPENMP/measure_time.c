#include <stdio.h>
#include <omp.h>

int main() {
    double start_time = omp_get_wtime();  // Start timing

    long long sum = 0;
    long long N = 100000000;  // Large number to make computation noticeable

    #pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < N; i++) {
        sum += i;
    }
    double end_time = omp_get_wtime();  // End timing
    double elapsed_time = end_time - start_time;

    printf("Sum = %lld, Time taken = %f seconds\n", sum, elapsed_time);

    return 0;
}
