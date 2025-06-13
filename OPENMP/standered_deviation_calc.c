#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

int main (int argc , char * argv []) {
     int n;
     
     printf("Enter number of array elements \n");
     scanf("%d", &n);
     
     int* arr = (int*) malloc(sizeof(int) * n);
     
     printf("Enter array elements: \n");
     for(int i = 0;i < n;i++) {
         scanf("%d", &arr[i]);
     }
     
     // Calculating the mean 
     double mean = 0.0;
     #pragma omp parallel for shared(arr) reduction(+:mean)
     for(int i = 0;i < n;i++) {
         mean += arr[i];
     }
    
     mean /= (double) n;
     
     double variance = 0.0;
     
     #pragma omp parallel for shared(arr) reduction(+:variance)
     for(int i = 0;i < n;i++) {
         variance += pow(arr[i] - mean, 2);
     }
     variance /= (double) n;
     
     double standered_dev = sqrt(variance);
     
     printf("The standered deviation is: %.3f\n", standered_dev);
     
    free(arr);
}
