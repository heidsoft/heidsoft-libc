#include "ch03.h"
#include "p3-6.c"
#define B_SIZE 100000
char buf[B_SIZE];
char fmt[] = "%d Hi :( -> :) ---aha!---";

int main(void)
{
   int nbytes=0, j=0, nwrite, ntimes = 1, success=0;
   char *ptr;

   if(set_nonblock_flag(STDOUT_FILENO, 1) < 0)     /* 設定無阻塞I/O */
      err_exit("set nonblock flag failed ");   
   while (nbytes+sizeof(fmt) < B_SIZE) {
      sprintf(&buf[nbytes],fmt,j++);
      nbytes += sizeof(fmt);
   }

   /* 將buf中的資料寫至標准輸出，直到全部寫出 */
   for(ptr=buf; nbytes >0; ntimes++){
      errno = 0;
      nwrite = write(STDOUT_FILENO, ptr, nbytes);
      if(nwrite < 0){
         fprintf(stderr,"\n%d  nwrite=%d, error=%d ",ntimes,nwrite,errno);
         perror("");
      } else {
         fprintf(stderr,"\n%d  nwrite=%d, error=%d ",ntimes,nwrite,errno);
         ptr += nwrite;
         success++;
         nbytes -= nwrite;
      }
   }
   printf("success=%d\n",success);
   exit(0);
}
