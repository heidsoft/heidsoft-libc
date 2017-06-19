#define _XOPEN_SOURCE
#include "ch08.h"
int main(void)
{
    struct tm *tm_ptr, timestruct;
    time_t the_time;
    char buf[256];
    char *result;
    (void)time (&the_time);            /* 獲得時間值 */
    tm_ptr = localtime (&the_time);    /* 轉為分解時間值 */
  	strftime (buf, 256, "%A %d %B, %I:%S %p",tm_ptr); /* 轉為字元格式 */
 	printf("strftime gives: %s\n",buf);
	strcpy (buf, "Mon 1 February 1999 17:53 will do fine");
 	printf ("calling strptime with: %s\n", buf);
	result = strptime (buf, "%a %d %b %Y %R",&timestruct);
 	printf ("strptime parsed up to: %s\n",result);
 	printf ("strptime gives:\n");
	printf ("    date: %4d/%02d/%02d\n",
		     1900+tm_ptr->tm_year,tm_ptr->tm_mon+1,tm_ptr->tm_mday);
	printf ("    time: %02d:%02d\n",tm_ptr->tm_hour,tm_ptr->tm_min);
 	exit(0);
}
