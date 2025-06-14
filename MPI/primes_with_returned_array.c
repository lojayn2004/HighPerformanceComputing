#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool IsPrime(int n) {
  if(n <= 1) return false;
  if(n == 2) return true;
  if(n % 2 == 0) return false;
  for(int i = 3 ; i * i <= n;i += 2) {
     if(n % i == 0) return false;
  }
  return true;
}

int main(int argc, char** argv) {
    int rank, size;
    int st, e;
  
    MPI_Init(&argc, &argv);  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    
    if(rank == 0) {
       printf("Enter the lower bound: \n");
       fflush(stdout); 
       scanf("%d", &st);
       printf("Enter the upper bound: \n");
       fflush(stdout); 
       scanf("%d", &e);
      
       
    }
    // Bcast start & end so they are visible to all processes
    MPI_Bcast(&st, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&e, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int portion = (e - st + 1) / size;
    int p_st, p_e;
    
    if(rank == 0) {
       // process zero takes the first portion of the array & the remainder
       p_st  = 0;
       p_e = portion + (e - st + 1) % size;
       // will hold all primes including root primes & also the primes for other processes
       int* global_primes = (int*) malloc(sizeof(int) * (e - st + 1));
       int cur_cnt = 0;
       // Root calculates its portion from the range 
       for(int i = p_st + 1; i <= p_e;i++) {
            if(IsPrime(i)) {
               global_primes[cur_cnt] = i;
               cur_cnt++;
            }
       }
      
       int local_sum = 0;
       
       for(int i = 1; i < size;i++) {
           MPI_Recv(&local_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           if(local_sum > 0) {
               int* cur_position = (global_primes + cur_cnt - 1);
               MPI_Recv(cur_position, local_sum, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
           cur_cnt += local_sum;
       }
       
       printf("Total Primes Count: %d \n", cur_cnt - 1);
       for(int i = 0; i < cur_cnt - 1;i++) {
            printf("%d ", global_primes[i]);
       }
       printf("\n");
       free(global_primes);
    
    }
    else {
       p_st  = rank * portion + (e - st + 1) % size;
       p_e = p_st + portion;
       int* local_primes = (int*) malloc(sizeof(int) * portion);
       int local_cnt = 0;
       for(int i = p_st + 1; i <= p_e;i++) {
            if(IsPrime(i)) {
                local_primes[local_cnt] = i;
                local_cnt++;
            }
       }
      
       MPI_Send(&local_cnt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
       if(local_cnt > 0) {
          MPI_Send(local_primes, local_cnt, MPI_INT, 0, 0, MPI_COMM_WORLD);
       }
       free(local_primes);
    }
    
    
    MPI_Finalize();  
    return 0;
}
