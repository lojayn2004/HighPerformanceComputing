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

    if(rank == 0) {
            int** mat = (int**) malloc(sizeof(int*) * n);
            for(int i = 0;i < n;i++) {
                 mat[i] = (int*) malloc(sizeof(int) * m);
            }
            for(int i = 0;i < n;i++) {
               for(int j = 0;j < m;j++) {
                    mat[i][j] = i * m + j;
               }
            }
            int* vect = (int*) malloc(sizeof(int)* m);
            for(int i = 0;i < m;i++) {
                vect[i] = i;
            }
            
            // Send vector for each process 
            for(int i = 1; i < size;i++) {
               MPI_Send(vect, m, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            
            int* res = (int*) malloc(sizeof(int)* m);
            
            // Send Matrix rows for each process
            int st_row, e_row;
            int portion = n / size; 
            int rem = n % size;
            for(int i = 1; i < size;i++) {
                st_row = portion * i + rem;
                MPI_Send(&mat[st_row][0], portion * m , MPI_INT, i, 0, MPI_COMM_WORLD);
                
            }
            for(int i = 0;i < portion + rem;i++) {
                res[i] = 0;
                for(int j = 0;j < m;j++) {
                    res[i] += mat[i][j] * vect[j];
                }
            }
            
            for(int i = 1; i < size;i++) {
                  st_row = portion * i + rem;
                  MPI_Recv(res + st_row, portion, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            
            for(int i = 0;i < m; i++) {
                  printf("%d ", res[i]);
            }
            printf("\n");
            
            
            
            for(int i = 0;i < n;i++) {
               free(mat[i]);
            }
            free(vect);
            free(mat);
            free(res);
    
    }
    else {
        int* vect = (int*) malloc(sizeof(int) * m);
        MPI_Recv(vect, m, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        int portion = n / size;
        int* mat = (int*) malloc(sizeof(int) * (portion * n));
        MPI_Recv(mat, portion * m, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        
        int* res = (int*) malloc(sizeof(int) * portion);
        
        for(int i = 0;i < portion;i++) {
            res[i] = 0;
            for(int j = 0;j < m;j++) {
               res[i] += mat[i * m + j] * vect[j];
            }
            printf("----> %d ", res[i]);
        }
        printf("\n");
        
        MPI_Send(res, portion, MPI_INT,  0, 0, MPI_COMM_WORLD);
        
        
        free(vect);
        free(mat);
        free(res);
    
    }
    
    
    
    MPI_Finalize();
    return 0;
}
