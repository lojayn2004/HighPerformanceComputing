# HighPerformanceComputing

## 📌 Description
This repository contains implementations of parallel algorithms developed while studying High-Performance Computing through:
- **Mahara Tech's HPC Course**
- **"Computer Architecture: A Quantitative Approach" (Hennessy & Patterson)**

## ⚠️ Disclaimer
> These implementations were developed as part of my HPC studies. They are not officially affiliated with any institution. While efforts have been made to ensure correctness, no guarantees are provided. Always verify results before use in critical applications.



## 🔧 Dependencies
- MPI 
- OpenMP
- C Compiler 

## 🚀 Usage
```bash
# Compile MPI programs
mpicc mpi_matvec.c -o mpi_matvect

# Run with 4 processes
mpirun -np 4 mpi_matvec

#openmp compilation
gcc -fopenmp vect.c
