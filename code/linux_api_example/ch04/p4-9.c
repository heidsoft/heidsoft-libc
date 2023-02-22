#include "ch04.h"

char outbuf[100];

int main(void)
{
   int   fdtmp, fdout, length;

   /* 建立二個檔案，一個作為中間檔案，一個作為結果檔案 */
   fdtmp = open("tmpfile", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
   fdout = open("outfile", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
   if ( fdtmp < 0 || fdout < 0) 
      err_exit("ERROR: creat file failed");

   /* 立即移除中間檔案 */
  if ( unlink ("tmpfile") < 0 )
      err_exit("unlink call failed");

   /* 往中間檔案寫資料，盡管這個檔案已經移除，我們現在仍然可以讀寫它 */
   length = write(fdtmp, "An example progamm for unlink().", 32);

   /* 反繞中間檔案 */
   if(lseek(fdtmp, (long)0, SEEK_SET) < 0)
      err_exit("lseek call failed");

   /* 複製中間檔案至結果檔案 */
   read(fdtmp, outbuf, length);
   write(fdout, outbuf, length);

   close (fdout);
}
