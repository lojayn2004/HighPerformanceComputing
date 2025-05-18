#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main() {
    // Section outside parallel part is printed only once
    int num_threads = omp_get_max_threads();
    printf("Total Max Threads: %d \n", num_threads); // 4 : since their is 4 cores in VM
    int total_outside = omp_get_num_threads();
    printf("Total Outside Parallel Section: %d \n", total_outside); // print 1
    
    #pragma omp parallel 
    {
       int tid = omp_get_thread_num();
       int total = omp_get_num_threads();
       printf("Current Thread: %d \n", tid); // Each thread prints its number
       printf("Total In Parallel Section: %d \n", total); // 4
    }
}
