#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
   
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* a_flat;
    int* b_flat;
    int* res_flat;
    int a_r, a_c;
    int eachProccessPortion;
    if(rank == 0) {
        int b_r, b_c;
        printf("Number of rows in first Matrix: \n");
        scanf("%d", &a_r);

        printf("Number of columns in first Matrix: \n");
        scanf("%d", &a_c);
        
        printf("Number of rows in second Matrix: \n");
        scanf("%d", &b_r);

        printf("Number of columns in second Matrix: \n");
        scanf("%d", &b_c);
        
        int numOfMatrixElements = a_r * a_c;
        
        if(a_r != b_r || a_c != b_c) {
              printf("\nCannot Add Two Matrices\nNot of same dimensions\n");
              MPI_Abort(MPI_COMM_WORLD, 1);
              return -1;
        }
       
        a_flat = (int *) malloc(sizeof(int) * (numOfMatrixElements));
        b_flat   = (int *) malloc(sizeof(int) * (numOfMatrixElements));
        res_flat = (int *) malloc(sizeof(int) * (numOfMatrixElements));
        int cur = 0;
        printf("Enter First Array elements: \n");
        
        for(int i = 0;i < numOfMatrixElements;i++) {
              scanf("%d", &a_flat[cur]);
              cur++;
        }
        cur = 0;
        printf("Enter Second Array elements: \n");
        for(int i = 0;i < numOfMatrixElements;i++) {
              scanf("%d", &b_flat[cur]);
              cur++;
        }
    }
    MPI_Bcast(&a_r, 1, MPI_INT,0, MPI_COMM_WORLD);
    MPI_Bcast(&a_c, 1, MPI_INT,0, MPI_COMM_WORLD);
    eachProccessPortion = (a_r * a_c) / size;
    int* recv_a = (int*) malloc(sizeof(int) * (eachProccessPortion));
    int* recv_b = (int*) malloc(sizeof(int) * (eachProccessPortion));
    int* partial_res = (int*) malloc(sizeof(int) * (eachProccessPortion));
    
    MPI_Scatter(a_flat, eachProccessPortion, MPI_INT, recv_a, eachProccessPortion, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b_flat, eachProccessPortion, MPI_INT, recv_b, eachProccessPortion, MPI_INT, 0, MPI_COMM_WORLD);
    
    for(int i = 0;i < eachProccessPortion;i++) {
        partial_res[i] = recv_a[i] + recv_b[i];
    }
    MPI_Gather(partial_res,eachProccessPortion, MPI_INT, res_flat, eachProccessPortion, MPI_INT,0, MPI_COMM_WORLD);
    if(rank == 0) {
        int cur = 0;
        int rem = (a_r * a_c) % size;
        int st = (a_r*a_c) - rem;
       
        // The master process must handle remaining elements
        for(int i = st; i < a_r * a_c;i++) {
            res_flat[i] = a_flat[i] + b_flat[i];
        }
        printf("\nResult of summing two matrices: \n");
        for(int i = 0;i < a_r;i++) {
           for(int j = 0;j < a_c;j++) {
              printf("%d ", res_flat[i * a_c + j]);
           }
           printf("\n");
        }
        free(a_flat);
        free(b_flat);
        free(res_flat);
    }
    free(recv_a);
    free(recv_b);
    free(partial_res);
    MPI_Finalize();
    return 0;
}
