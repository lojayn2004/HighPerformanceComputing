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
    int total_primes = 0;
    MPI_Init(&argc, &argv);  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    if(rank == 0) {
       printf("Enter the lower bound: ");
       fflush(stdout); 
       scanf("%d", &st);
       printf("Enter the upper bound: ");
       fflush(stdout); 
       scanf("%d", &e);
    }
     double start = MPI_Wtime();
    MPI_Bcast(&st, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&e, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // range of numbers / size of all processes
    int eachProcessRange = (e - st + 1) / size; 
    int rem = (e - st + 1) % size;
    
    int p_st = st + (rank * eachProcessRange) + (rank < rem ? rank: rem);
    int p_e = p_st + eachProcessRange + (rank < rem ? 1: 0);
    int cur_cnt = 0;
    for(int i = p_st;i < p_e;i++) {
       if(IsPrime(i)) 
         cur_cnt++; 
    }
    printf("P(%d): Total Number of primes from %d to %d is:  %d \n", rank, p_st, p_e - 1, cur_cnt);
    MPI_Reduce(&cur_cnt, &total_primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double end = MPI_Wtime();
    if(rank == 0) {
       printf("Total Number of primes from %d to %d is:  %d \n", st, e, total_primes);
       printf("Total Time needed %f \n", end - start);
    }
    MPI_Finalize();  
    return 0;
}
