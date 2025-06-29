#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv) {
    // Taking user input 
    int n, m, r, c;
    printf("Multiply matrix by Matrix program\n\n");
    printf("Enter the number of rows in first Matrix : \n");
    scanf("%d", &n);
    
    printf("Enter the number of columns in first Matrix: \n");
    scanf("%d", &m);
    
    printf("Enter the number of rows in second Matrix : \n");
    scanf("%d", &r);
    
    printf("Enter the number of columns in second Matrix: \n");
    scanf("%d", &c);
    
    if(m != r) {
        printf("Cannot Multiply Matrices\nIncompatible rows & columns count\n"); 
        return -1;
    }
    
    // Dynamically allocating space for matrices and result
    int** mat1 = (int**) malloc(sizeof(int*) * n);
    int** mat2 = (int**) malloc(sizeof(int*) * r);
    int** res = (int**) malloc(sizeof(int*) * n);
    
    for(int i = 0; i < n; i++) {
        mat1[i] = (int*) malloc(sizeof(int) * m);
        res[i] = (int*) malloc(sizeof(int) * c);
    }
    
    for(int i = 0; i < r; i++) {
        mat2[i] = (int*) malloc(sizeof(int) * c);
    }
    
    printf("Enter first matrix content: \n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            scanf("%d", &mat1[i][j]);
        }
    }
    
    printf("Enter second matrix content: \n");
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            scanf("%d", &mat2[i][j]);
        }
    }
    
    // Initialize result matrix to 0
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < c; j++) {
            res[i][j] = 0;
        }
    }
    
   
    #pragma omp parallel for shared(mat1, mat2, res)
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < c; j++) {
            for(int k = 0; k < m; k++) { 
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    
    // printing the result 
    printf("Result: \n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < c; j++) {
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }
    
    // Free memory
    for(int i = 0; i < n; i++) {
        free(mat1[i]);
        free(res[i]);
    }
    for(int i = 0; i < r; i++) {
        free(mat2[i]);
    }
    free(mat1);
    free(mat2);
    free(res);
    
    return 0;
}
