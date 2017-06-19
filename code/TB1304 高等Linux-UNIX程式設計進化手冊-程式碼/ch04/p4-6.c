#include "ch04.h"

int main(int argc, char *argv[])
{
   /* 檢查參數合法否 */    
   if (argc != 2){
      printf("usage: a.out <filename>\n");
      exit(1);
   }     
   /* 檢查實際使用者是否有讀權限 */
   if (access(argv[1],R_OK) < 0){              
      printf("access error for %s. ",argv[1]);
      if (errno == EACCES)
         printf("you are not the owner of this file!\n");
   }
   else
      printf("read access OK\n");
   /* 無論是否有實際使用者讀權限，仍嘗試開啟該檔案 */
   if (open(argv[1], O_RDONLY) < 0)
      printf("open error for %s\n",argv[1]);
   else
      printf("open for reading OK\n");
   exit(0);  
}
