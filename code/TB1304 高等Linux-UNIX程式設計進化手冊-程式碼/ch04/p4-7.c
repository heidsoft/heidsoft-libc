#include "ch04.h"

int main(int argc, char*argv[])
{  
   long int length;
   int fd;
   char *cp;
   
   if (argc != 3){   /* 檢查參數的正確性 */
      printf("usage: a.out <filename> <file size>\n");
      exit(1);
   }

   /* 讀取參數並開啟檔案 */
   length = strtol(argv[2], &cp, 10); 
   if (cp==argv[2]) {  /* 第二參數非數字 */
      printf("usage: a.out <filename> <file size> and "
             "<file size> must be a integer\n");
      exit(1);
   }

   if ((fd = open(argv[1],O_RDWR)) < 0)
      err_exit("open() call failed");

   /* 按第二參數指定位元組裁斷檔案 */
   printf ("truncate %s to %d characaters\n", argv[1], length);
   if ( ftruncate(fd, length) < 0 ) 
      err_exit("truncate() call failed");
   printf ("truncate() call successful\n");

   lseek(fd,(long)0, SEEK_END); /* 定位至檔案尾 */
   write(fd, "@T" ,2);          /* 寫入尾部標誌 */

   close(fd);
}
