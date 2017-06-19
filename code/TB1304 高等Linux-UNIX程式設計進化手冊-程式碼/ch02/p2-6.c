#include "ch02.h"
#include "y_or_n_ques.c"
char buf[132];
int main(int argc, char *argv[])
{
   FILE *fd;
   fpos_t pos;
   if (!y_or_n_ques("Should we use append mode?")) {
     if ((fd = fopen("test_file","w+")) == NULL)  /* 以寫模式開啟檔案  */
        err_exit("fopen faild");
   } else {
     if ((fd = fopen("test_file","a+")) == NULL)  /* 以加入模式開啟檔案  */
        err_exit("fopen faild");
   }
   fputs("0123456789ABCDEFGHIJ ",fd);    /* 寫入一行資料 */
	   /* 檢視目前檔案尾位置 */
   fseek(fd,0,SEEK_END);
   fgetpos(fd,&pos);
   printf("current file position is %ld\n",pos);
   fseek(fd,30,SEEK_END);   /* 定位至檔案尾之後30位元組，並檢視目前檔案尾位置  */
   fgetpos(fd,&pos);	
   printf("Now we call fseek(fd,30,SEEK_END); "
            "current file position is %ld\n",pos);
   fputs("abcdefg",fd); 	 /* 寫入資料 */
   printf("Now we write 7 bytes \"%s\"\n","abcdefg");
   fgetpos(fd,&pos);        /* 檢視目前檔案尾位置 */
   printf("current file position is %ld\n",pos);
   fclose(fd);
}
