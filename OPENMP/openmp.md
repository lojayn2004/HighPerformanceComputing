# 🧠 OpenMP Cheatsheet

## 🔹 What is OpenMP?

Open MultiProcessing (OpenMP) is an API used for **multi-threaded parallel programming** on shared-memory systems. - Supports C/C++ and Fortran. - Uses **compiler directives (pragmas)** to control parallelism. - Designed for **shared memory architectures**.

## ⚙️ OpenMP Basics

### 1. `#pragma omp parallel` > Creates a team of threads to execute the following block in parallel. ```c #pragma omp parallel {     printf("Hello from thread %d\n", omp_get_thread_num()); } ```

### 2. `#pragma omp for` > Distributes loop iterations among existing threads (must be inside a parallel region). ```c #pragma omp parallel {     #pragma omp for     for (int i = 0; i < N; i++) {         printf("Iteration %d by thread %d\n", i, omp_get_thread_num());     } } ```

### 3. `#pragma omp parallel for` > Combines `parallel` and `for` into one directive. ```c #pragma omp parallel for for (int i = 0; i < N; i++) {     printf("Iteration %d by thread %d\n", i, omp_get_thread_num()); } ```

### 4. `#pragma omp critical` > Ensures only one thread executes the block at a time (prevents race conditions). ```c #pragma omp parallel {     #pragma omp critical     {         counter++;         printf("Counter = %d by thread %d\n", counter, omp_get_thread_num());     } } ```

### 5. `#pragma omp atomic` > Performs a single **atomic read-modify-write** operation safely. ```c #pragma omp parallel {     #pragma omp atomic     counter++; } ``` > ❗ Cannot handle compound statements or arbitrary assignments.

### 6. `#pragma omp barrier` > Synchronizes all threads — no thread proceeds until **all have reached this point**. ```c #pragma omp parallel {     printf("Before barrier: Thread %d\n", omp_get_thread_num());      #pragma omp barrier      printf("After barrier: Thread %d\n", omp_get_thread_num()); } ``` > ⚠️ All threads must reach the barrier or risk deadlock.

## 📦 Data Environment Clauses

These control how variables are shared or private in a parallel region.

### 1. `private(variable_list)` > Each thread gets its own **local copy** of the variable. ```c int x = 10; #pragma omp parallel private(x) {     x = omp_get_thread_num();     printf("x = %d in thread %d\n", x, omp_get_thread_num()); } // Original x remains unchanged (still 10) ```

### 2. `firstprivate(variable_list)` > Like `private`, but initializes each thread’s local copy with the original value. ```c int x = 10; #pragma omp parallel firstprivate(x) {     x += omp_get_thread_num();     printf("x = %d in thread %d\n", x, omp_get_thread_num()); } // Original x still equals 10 ```

### 3. `lastprivate(variable_list)` > The variable in the **original scope** is assigned the value from the **last iteration/section**. ```c int x; #pragma omp parallel for lastprivate(x) for (int i = 0; i < N; i++) {     x = i; } printf("Final x
