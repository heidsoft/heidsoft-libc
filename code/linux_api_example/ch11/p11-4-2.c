#include "p11-4.h"
int main(void)
{
   int fifo_fd, i, n, total_bytes = 0;
   char sendbuf[PIPE_BUF];
   /* 開啟FIFO檔案 */
   if ((fifo_fd = open(FIFO_FILE, O_WRONLY)) <0 ){
      printf( "Could not open fifo %s\n", FIFO_FILE);
      exit(EXIT_FAILURE);
   }
   printf("Process %d opened: %s\n", getpid(), FIFO_FILE);
   do {
      if ((n = write(fifo_fd, sendbuf, PIPE_BUF)) == -1) 
         err_exit("FIFO broken");
      printf(" %d write %d bytes\n", getpid(),n);
      total_bytes += n;
   }while (total_bytes < TEN_MEG);
   close(fifo_fd);
   printf("Process %d finished\n", getpid());
   return 0;
}
