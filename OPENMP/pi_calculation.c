#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415926535897932384626433832795029L


double f(double x) {
	return ( double)4.0/(( double )1.0+( x *x ));
}
int main ( int argc , char * argv []){
	int n , i;
	double h , pi , sum ,x ;
	
        printf (" Enter the number of intervals: ");
	scanf ("%d", &n);
	if (n == 0)
	    return 0;
	h = 1.0/( double)n ;
	sum = 0.0;
	#pragma omp parallel for private(i, x) reduction(+: sum)
        for (i = 1; i <= n ;i ++) {
		x = h *(( double )i -( double )0.5);
		sum += f (x);
	}
	pi = h* sum ;
	printf ("Real PI: %.16Lf \n", PI);
	printf ("Computed PI: %.16f \n", pi);
      
	 return EXIT_SUCCESS ;
}
