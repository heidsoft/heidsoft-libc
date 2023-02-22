#include "ch09.h"
int main(void)
{
   int  n1, n2, fd;
   if((fd=open("/dev/ttyS0", O_RDWR|O_NOCTTY|O_NONBLOCK))== -1){
      fprintf(stderr, "Open port: Unable to open, %s\n",strerror(errno));
      exit(EXIT_FAILURE);
   }
   write(fd, "Example of line control functions ", 34);
   tcdrain(fd);             /* 等待輸出佇列中的資料全部送出 */
   tcflow(fd, TCOOFF);      /* 懸掛輸出傳輸 */
   n1 = write(fd, "this line will be thrown over\n", 30);
   tcflush(fd, TCOFLUSH);   /* 清除輸出佇列 */ 
   n2 = write(fd, "this line will not be thrown over\n", 34);
   tcflow(fd, TCOON);       /* 還原輸出傳輸 */
   write(fd,"restart the output\n",19);
   exit(EXIT_SUCCESS);
}
