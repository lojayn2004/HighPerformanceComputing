#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
    int size, rank;
   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n, m;
     
    int* mat;
    int* vect;
    
    if(rank == 0) {
       // Root process takes the size of the matrix from the user
       printf("Enter matrix number of rows: \n");
       scanf("%d", &n);
       
       printf("Enter matrix number of columns: \n");
       scanf("%d", &m);
       
       // Root Takes matrix & vector but takes matrix as one dimensional array in order to
       // Bcast it easily to other processes
       mat = (int*) malloc(sizeof(int) * (n * m) );
       vect = (int*) malloc(sizeof(int) * m);
       printf("Enter matrix elements: \n");
       for(int i = 0;i < n;i++) {
          for(int j = 0;j < m;j++) {
              scanf("%d", &mat[i * m + j]);
          }
       }
       
       printf("Enter vector elements: \n");
       for(int i = 0;i < m;i++) {
            scanf("%d", &vect[i]);
       }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank != 0) {  
         mat = (int*) malloc(sizeof(int) * (n * m));
         vect = (int*) malloc(sizeof(int) * m);
    }
    
    MPI_Bcast(mat, n * m, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vect, m, MPI_INT, 0, MPI_COMM_WORLD);
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
         int* res = (int*) malloc(sizeof(int) * (p_rows));
         for(int i = cur_st;i < cur_end;i++) {
             res[i - cur_st] = 0;
             for(int j = 0;j < m;j++) {
                res[i - cur_st] += mat[i * m + j] * vect[j];  
            }
         }
         
         MPI_Send(res, p_rows, MPI_INT, 0, 0, MPI_COMM_WORLD);
         free(res);
    }
    else {
         int* final_res = (int*) malloc(sizeof(int) * n);
         for(int i = cur_st;i < cur_end;i++) {
             final_res[i] = 0;
             for(int j = 0;j < m;j++) {
                final_res[i] += mat[i * m + j] * vect[j];
            }
         }
    
         for(int i = 1;i < size;i++) {
             int* process_start = final_res + n % size + i * p_rows;
             MPI_Recv(process_start, p_rows, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }
         printf("Result \n");
         for(int i = 0;i < n;i++) {
             printf("%d ", final_res[i]);
         }
         printf("\n");
         free(final_res);
    
    }
    free(mat);
    free(vect);
   
    MPI_Finalize();
}
