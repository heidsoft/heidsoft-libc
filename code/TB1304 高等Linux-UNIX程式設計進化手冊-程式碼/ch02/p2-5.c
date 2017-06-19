#include "ch02.h"
int main (int argc, char *argv[])
{
   int n;
   FILE *from, *to;
   char buf[BUFSIZ] ;
   if (argc != 3) {    /*檢查參數。*/
      fprintf(stderr, "Usage : %s from-file to-file\n", *argv) ;
      exit (1);
   }
   if ((from = fopen(argv[1],"r")) == NULL)  /* 為讀而開啟檔案from */
      err_exit (argv[1] ) ; 
	   /* 以加入模式開啟檔案to. 若此檔案不存在，fopen 將建立它。*/
   if ((to=fopen(argv[2], "a")) == NULL)
      err_exit(argv[2] ) ;
	   /* 現在每次可以從檔案from讀入並寫至to. 注意我們寫出的字元個數是實際讀入
      的字元個數而不總是BUFSIZ位元組。*/
   while ((n = fread(buf, sizeof(char),BUFSIZ,from)) > 0)
      fwrite (buf, sizeof(char),n,to) ;
	   /*關閉檔案*/
   fclose (from) ;
   fclose (to) ;
   exit (0) ;
}
