#include "ch08.h"
int main(void)
{
   int i;
   time_t current_time, begin_time;
   begin_time = time((time_t *)NULL);
   for(i=1; i<=5; i++){
      printf("current time is %ld\n", time((time_t *)0));
      sleep(2);
   }
   printf("elapsed wall time is %lf(s)\n", 
      difftime(time((time_t *)NULL), begin_time));
   exit(0);
}
