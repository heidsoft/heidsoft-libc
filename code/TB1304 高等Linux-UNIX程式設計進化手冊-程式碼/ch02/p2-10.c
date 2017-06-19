#include "ch02.h"
int main(void)
{
   FILE *tempfp;
   char tmpname[L_tmpnam],*cp;
   char line[256];
   char *pfx = "XXXXX";
   if ((cp=tmpnam(tmpname))==NULL)  // 檢視tmpnam()產生的檔名
      printf("a unique name cannot be generate by tmpnam()\n");
   else
      printf("       file name get by tmpnam(): %s\n", cp);
   /* 檢視tempnam()產生的檔名 */
   cp = tempnam("./", pfx);          
   printf("            by tempnam(\"./\",pfx): %s\n", cp);
   free(cp);
   cp = tempnam("/not_exist_dir/", pfx);
   printf("by tempnam(\"/not_exist_dir\",pfx): %s\n", cp);
   free(cp);
    /* 用tmpfile()建立一個暫存檔 */
   if ((tempfp=tmpfile()) == NULL)
      err_exit("tmpfile error");
   printf("tmpfile() created a temporary file\n");
   /* 往暫存檔寫入一行資料,然後將它讀出並寫至標准輸出進行驗證 */
   fputs("One line of output\n", tempfp);
   rewind(tempfp);
   if(fgets(line, sizeof(line), tempfp) == NULL)
      err_exit("fgets error");
   fputs(line,stdout); 
   exit(0);
}
