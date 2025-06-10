#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* arr;
    int len;
    
    if(rank == 0) {
        printf("Enter array length: \n");
        scanf("%d", &len);
        
        printf("Enter array elements: \n");
        arr = (int*) malloc(sizeof(int) * len);
        for(int i = 0;i < len;i++) {
             scanf("%d", &arr[i]);
        }
    }
 
    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank != 0) {
       arr = (int*) malloc(sizeof(int) * len);
    }
   
    MPI_Bcast(arr, len, MPI_INT, 0, MPI_COMM_WORLD);
    int portion = len / size;
    int st, e;
    if(rank != 0) {
       st = (rank * portion) + len % size;
       e = st + portion;
       int local_sum = 0;
       for(int i = st;i < e;i++) {
          local_sum += arr[i];
       }
       
       MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
       st = 0;
       e = len % size + portion; // the root process must processes the remainder
       
       int sum = 0, recv_sum;
       for(int i = st;i < e;i++) {
          sum += arr[i];
       }
       for(int i = 1 ;i < size;i++ ){
           MPI_Recv(&recv_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           sum += recv_sum;
       }
       printf("The sum of the array is: %d\n", sum);
   }
   free(arr);
   
    MPI_Finalize();
    return 0;
}
