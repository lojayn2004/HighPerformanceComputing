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
       double start = MPI_Wtime();
       int eachProcessRange = (e - st + 1) / size; 
       int rem = (e - st + 1) % size;
       int p_st = st + rank * eachProcessRange + (rank < rem ? rank: rem);
       int p_e = p_st + eachProcessRange + (rank <= rem ? 1: 0);
       int cur_cnt = 0;
       for(int i = p_st;i < p_e;i++) {
            if(IsPrime(i)) 
               cur_cnt++; 
       }
       printf("P(%d): Total Number of primes from %d to %d is:  %d \n", rank, p_st, p_e - 1, cur_cnt);
       total_primes = cur_cnt;
       for(int i = 1;i < size;i++) {
          MPI_Send(&st, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
          MPI_Send(&e, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
       }
       for(int i = 1;i < size;i++) {
          int part_sum = 0;
          MPI_Recv(&part_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          total_primes += part_sum;
       
       }
       double end = MPI_Wtime();
       
       printf("Total Number of primes from %d to %d is:  %d \n", st, e, total_primes);
       printf("Total Time needed %f \n", end - start);
    }
    else  {
          MPI_Recv(&st, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(&e, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          // range of numbers / size of all processes - 1
          // Determine the range of elements for each process.
          // If the process rank is less than the remainder, it gets one extra element.
          // This distributes the remaining elements evenly across processes,  
          // preventing the root process from handling all the extra work.
          int eachProcessRange = (e - st + 1) / size; 
          int rem = (e - st + 1) % size;
          int p_st = st + rank * eachProcessRange + (rank < rem ? rank : rem);
          int p_e = p_st + eachProcessRange + (rank < rem ? 1: 0);
          int cur_cnt = 0;
          for(int i = p_st;i < p_e;i++) {
            if(IsPrime(i)) 
               cur_cnt++; 
          }
          
          printf("P(%d): Total Number of primes from %d to %d is:  %d \n", rank, p_st, p_e - 1, cur_cnt);
          MPI_Send(&cur_cnt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }
  
    MPI_Finalize();  
    return 0;
}
