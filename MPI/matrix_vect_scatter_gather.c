#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
  
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int n = 4, m = 4;
    int* mat;
    int* vect = (int*) malloc(sizeof(int)* m);
    int*final_res;
    // root initializing main parts 
    if(rank == 0) {
            mat = (int*) malloc(sizeof(int) * (n * m));
            for(int i = 0;i < n;i++) {
               for(int j = 0;j < m;j++) {
                    mat[i * m + j] = i * m + j;
               }
            }
            
            for(int i = 0;i < m;i++) {
                vect[i] = i;
            }
            final_res = (int*) malloc(sizeof(int) * n);
    }
    MPI_Bcast(vect, m, MPI_INT, 0, MPI_COMM_WORLD);
    int portion = n / size;
    int* recv = (int*) malloc(sizeof(int) * (portion * m));
    int* res = (int*) malloc(sizeof(int) * portion);
    MPI_Scatter(mat, portion * m, MPI_INT, recv, portion * m, MPI_INT, 0, MPI_COMM_WORLD);
   
    for(int i = 0;i < portion;i++) {
          res[i] = 0;
          for(int j = 0;j < m;j++) {
             res[i] += recv[i * m + j] * vect[j];
          }
    }
    MPI_Gather(res, portion, MPI_INT, final_res, portion, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0) {
        if(n % size != 0) {
            int st = size * portion;
             for(int i = st;i < n;i++) {
                  final_res[i] = 0;
                  for(int j = 0;j < m;j++) {
                       final_res[i] += mat[i * m + j] * vect[j];
                  }
              }
        }
        
        for(int i = 0;i < n; i++) {
            printf("%d ", final_res[i]);
        }
        printf("\n");
        
        
       free(mat);
       free(final_res);
    }
     free(recv);
     free(res);
     free(vect);
     MPI_Finalize();
    return 0;
}
