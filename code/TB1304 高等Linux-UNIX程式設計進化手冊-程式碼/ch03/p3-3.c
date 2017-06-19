#include "ch03.h"
int main(void)
{
   int fd;
   if((fd = open("myoutput",O_WRONLY|O_CREAT,0644)) == -1)
      err_exit("myoutput");
   if(dup2(fd,STDOUT_FILENO) == -1)    /* 重復標准輸出至fd相連的檔案myoutput */
      err_exit("Redirect standard output failed");
   printf("this is a test program for redirect \n");
   close(fd);
}
