#include "ch08.h"
int main(void)
{
   clock_t start, end;
   double x = 0.12;
   int i;
   start = clock();  
   for (i = 0; i < 1000000; i++)
      x = log(x);    
   end = clock();
   printf ("start=%ld, end=%ld, end-start =%ld\n", start, end, end-start);
   printf ("elapsed second: %ld , elapsed time: %lf\n",
	        (end-start)/CLOCKS_PER_SEC, 
            (double)(end-start)/CLOCKS_PER_SEC );
   return 0;
}
