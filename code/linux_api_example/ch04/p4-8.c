#include "ch04.h"

int main(int argc, char *argv[])
{
   int i;
   struct  stat statbuf;
   struct utimbuf times;

   if (argc != 2) {
      printf("Usage: a.out filename\n");
      exit(1);
   }
   if (stat(argv[1],&statbuf) < 0)       /* 獲得檔案的目前時間 */
      err_exit(argv[1]);
   if (open(argv[1],O_RDWR|O_TRUNC) < 0)      /* 裁斷檔案 */
      err_exit(argv[1]);
   printf("%s is truncated now.\n", argv[1] );
   times.actime = statbuf.st_atime;          /* 還原檔案時間至原時間 */
   times.modtime = statbuf.st_mtime;   
   if (utime(argv[1], &times) == 0)            
      printf ("utime() successful\n");
   else
      printf ("ERROR: utime() failed. %s\n",strerror(errno));
   exit(0);
}
