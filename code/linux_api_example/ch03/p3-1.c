#include "ch03.h"
int main (int argc, char **argv)
{
   int n, from, to;
   char buf[1024] ;
   if (argc != 3) {      /* 檢查參數個數。*/
      printf("Usage : %s from-file to-file\n", argv[0]);
      exit (1);
   }
   if ((from = open (argv[1],O_RDONLY)) < 0)  /* 為讀而開啟檔案from */
      err_exit (argv[1] ); 
  /* 以加入模式開啟檔案to. 若檔案不存在，open() 將以模式644（-rw-r--r--)建立它。*/
   if ((to = open(argv[2], O_WRONLY|O_CREAT|O_APPEND, 0644)) < 0)
      err_exit (argv[2] );
   /* 從檔案from讀資料並寫至to.每次寫出的字元個數是每次讀入的字元個數 */
   while ((n = read(from, buf, sizeof(buf))) > 0)
       write (to, buf, n) ;
   close(from) ;     /* 關閉檔案 */
   close(to) ;
}
