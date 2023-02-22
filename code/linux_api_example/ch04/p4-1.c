#include "ch04.h"
void * xmalloc(int);     /* 程式5-6 */
read_specs(char *filename)
{
   int desc;
   struct stat statbuf;
   char *buffer;
   register char *p;
   desc = open(filename, O_RDONLY, 0);    /* 開啟檔案並取得檔案訊息 */
   if (desc < 0)
     err_exit(filename);
   if (stat(filename,&statbuf) < 0)
     err_exit(filename);
   /* 為buffer分配大小為st_size的空間 */
   buffer = xmalloc ((unsigned)statbuf.st_size + 1); 
   read (desc,buffer,(unsigned)statbuf.st_size);     /* 讀整個檔案內容至buffer */
   buffer[statbuf.st_size] = 0;
   close (desc);
   /* 處理讀入的檔案，... */
}
