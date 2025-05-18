#include <omp.h>
#include <stdio.h>
#include <limits.h>  

int main() {
    int arr[] = {10, 5, 8, -1, 15, 3, 20};
    int n = sizeof(arr) / sizeof(arr[0]);

    // Start with largest possible int
    int min_val = INT_MAX;  

    #pragma omp parallel for reduction(min:min_val)
    for(int i = 0; i < n; i++) {
        if(arr[i] < min_val) {
            min_val = arr[i];
        }
    }

    printf("Minimum value: %d\n", min_val);
    return 0;
}
