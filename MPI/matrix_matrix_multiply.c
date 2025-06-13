#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
    int size, rank;
   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n, m, r, c;
     
    int* mat1;
    int* mat2;
    
    if(rank == 0) {
       // Root process takes the size of the matrix from the user
       printf("Enter matrix number of rows: \n");
       scanf("%d", &n);
       
       printf("Enter matrix number of columns: \n");
       scanf("%d", &m);
       
       printf("Enter matrix number of rows: \n");
       scanf("%d", &r);
       
       printf("Enter matrix number of columns: \n");
       scanf("%d", &c);
       
       if(m != r) {
            printf("Cannot Multiply\nIncompatible rows & cols count\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
       
       }
       
       mat1 = (int*) malloc(sizeof(int) * (n * m));
       mat2 = (int*) malloc(sizeof(int) * (r * c));
       printf("Enter first matrix elements: \n");
       for(int i = 0;i < n;i++) {
          for(int j = 0;j < m;j++) {
              scanf("%d", &mat1[i * m + j]);
          }
       }
       
       printf("Enter second matrix  elements: \n");
       for(int i = 0;i < r;i++) {
          for(int j = 0;j < c;j++) {
              scanf("%d", &mat2[i * c + j]);
          }
       }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&c, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank != 0) {  
      mat1 = (int*) malloc(sizeof(int) * (n * m));
       mat2 = (int*) malloc(sizeof(int) * (r * c));
    }
    
    MPI_Bcast(mat1, n * m, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(mat2, r * c, MPI_INT, 0, MPI_COMM_WORLD);
    // Each process calculates its portion from matrix rows that is has to process
    int p_rows = n / size;
    int cur_st, cur_end;
    if(rank == 0) {
        cur_st = 0;
        cur_end = p_rows + n % size;
    }
    else {
        cur_st = rank * p_rows + n % size;
        cur_end = cur_st + p_rows;
    }
    // If the process is not the root it will calculate its portion & send it back to the root
    // The root gathers the result in the final array based on which portion each process took 
    if(rank != 0) {
         int* res = (int*) malloc(sizeof(int) * (p_rows * c));
         for(int i = cur_st;i < cur_end;i++) {
             for(int k = 0; k < c;k++) {
                int idx = i - cur_st;
                res[idx * c + k] = 0;
                for(int j = 0; j < m;j++) {
                    res[idx * c + k] += mat1[i * m + j] * mat2[j * c + k];
                }
             }
         }
         
         MPI_Send(res, p_rows * c, MPI_INT, 0, 0, MPI_COMM_WORLD);
         free(res);
    }
    else {
         int* final_res = (int*) malloc(sizeof(int) * (n * c));
         for(int i = cur_st;i < cur_end;i++) {
             for(int k = 0; k < c;k++) {
                int idx = i - cur_st;
                final_res[idx * c + k] = 0;
                for(int j = 0; j < m;j++) {
                    final_res[idx * c + k] += mat1[i * m + j] * mat2[j * c + k];
                }
             }
         }
         for(int i = 1;i < size;i++) {
             int* process_start = final_res + n % size + i * p_rows * c;
             MPI_Recv(process_start, p_rows * c, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }
         printf("Result \n");
         for(int i = 0;i < n;i++) {
             for(int j = 0;j < c;j++) {
                 printf("%d ", final_res[i * c + j]);
             }
             printf("\n");
         }
        
         free(final_res);
    
    }
    free(mat1);
    free(mat2);
   
    MPI_Finalize();
}
