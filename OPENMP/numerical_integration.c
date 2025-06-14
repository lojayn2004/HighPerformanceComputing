#include <stdio.h>
#include <math.h>
#include <omp.h>

// Function to integrate could be any function we want (sin(x), x*x, ....)
double f(double x) {
    return sin(x); 
}


double trapezoidal_integral(double a, double b, int n) {
    // Calculate step size
    double h = (b - a) / n; 
    
    // Calculate End points
    double sum = 0.5 * (f(a) + f(b)); 
    
    double x;
    #pragma omp parallel for private(x) reduction(+:sum)
    for (int i = 1; i < n; i++) {
        x = a + i * h;
        sum += f(x);
    }
    return sum * h;
}

int main() {
    double a = 0.0, b = 1.0; // Integration bounds
    int n;     
    
    printf("Enter number of trapezoids: ");
    scanf("%d", &n);
  
    double result = trapezoidal_integral(a, b, n);
    
    printf("Approximate integral: %.10f\n", result);
    
    return 0;
}
