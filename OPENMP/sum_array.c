#include <omp.h>
#include <stdio.h>

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arrLen = sizeof(arr) / sizeof(arr[0]);
    int arrSum = 0;
    int i = 0;
    /*
    *  The reduction clause in OpenMP is used to safely perform parallel accumulation operations like: +, -, *, &, |, ^, && and ||, min, max
    *  It helps us prevent race conditions
    */
    #pragma omp parallel for private(i) reduction(+:arrSum)
    for(i = 0; i < arrLen; i++) {
        arrSum += arr[i];
    }

    printf("Sum of array: %d\n", arrSum);
    return 0;
}
