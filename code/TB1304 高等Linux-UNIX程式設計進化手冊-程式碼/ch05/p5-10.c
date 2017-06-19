#include "ch05.h"

double get_cpu_time(void)
{
   struct rusage r_usage;
   time_t msec;

   getrusage(RUSAGE_SELF,&r_usage);
   msec = r_usage.ru_utime.tv_sec*1000000.0 + r_usage.ru_utime.tv_usec;
   return ((double)(msec)/1000000.0);
}

