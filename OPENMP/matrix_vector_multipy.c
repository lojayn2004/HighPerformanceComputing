#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char** argv) {

         // Taking user input 
         int n, m;
         printf("Multiply matrix by vector program\n\n");
         printf("Enter the number of rows: \n");
         scanf("%d", &n);
         
         printf("Enter the number of columns: \n");
         scanf("%d", &m);
         
         // Dynamically allocating space for matrix , vector and result
         int** mat = (int**) malloc(sizeof(int*) * n);
         
         for(int i = 0;i < n;i++) {
            mat[i] = (int*) malloc(sizeof(int) * m);
           
         }
         int* vect = (int*) malloc(sizeof(int) * m);
         int* res = (int*) malloc(sizeof(int) * n);
         printf("Enter matrix content: \n");
         
         for(int i = 0 ;i < n;i++) {
             for(int j = 0; j < m;j++) {
                  scanf("%d", &mat[i][j]);
             }
        }
         
        printf("Enter vector content: \n");
         
        for(int i = 0;i < m;i++) {
            scanf("%d", &vect[i]);
        }
        
        // Dividing the work across number of threads 
        #pragma omp parallel for
        for(int i = 0 ;i < n;i++) {
             int s = 0;
             for(int j = 0; j < m;j++) {
                 s += mat[i][j] * vect[j];
             }
             res[i] = s;
        }
        
        // printing the result 
        printf("Result: \n");
  
        for(int i = 0; i < m;i++) {
              printf("%d ", res[i]);
        }
        printf("\n");
        
        free(vect);
        free(res);
        for(int i = 0;i < n;i++) {
            free(mat[i]);
            
        }
        free(mat);         
}
