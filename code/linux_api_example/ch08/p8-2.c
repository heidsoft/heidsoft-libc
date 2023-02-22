#include "ch08.h"
void report_time(struct tm *);
static char *wday[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
int main(void)
{
   time_t     the_time;
   struct tm  *tm_time;
   struct tm  unknown_time = {
   /* sec, min, hour, mday, mon, year, wday, yday, isdst */
       40,  30,    2,    1,    8,    98,    0,     259,   0,
   };
   time (&the_time);                   /*  取目前原始時間  */
   printf (" current raw time is %ld\n", the_time);
   tm_time = gmtime (&the_time);     /*  轉換原始時間至GMT分解時  */
   printf (" gmtime gives: \n");
   report_time (tm_time);
   tm_time = localtime (&the_time); 	/*  轉換原始時間至本機時  */
   printf (" localtime gives: \n");
   report_time (tm_time);
   /*  轉換一個用分解時表示的不存在的日期至原始時間  */
   the_time = mktime (&unknown_time);
   if ( the_time <0 ) 
      printf ("ERROR: mktime() call failed\n");
   else {
      printf (" after calling mktime:\n");
      printf ("     time = %ld\n", the_time);
      report_time (&unknown_time);
      printf ("     weekday: %s   yearday: %d\n",
              wday[unknown_time.tm_wday], unknown_time.tm_yday);
   }
}
void report_time(struct tm *tm_ptr)
{
   printf ("     date: %4d/%02d/%02d\n",  
              1900+tm_ptr->tm_year, tm_ptr->tm_mon+1, tm_ptr->tm_mday);
   printf ("     time: %02d:%02d:%02d\n", 
              tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
   asctime(tm_ptr);
}
