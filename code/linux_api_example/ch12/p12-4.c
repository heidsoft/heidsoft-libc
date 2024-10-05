#include "ch12.h"
int main(void)
{
   union{
      short inum;
      char c[sizeof(short)];
   }un;
   struct utsname uts;
   un.inum = 0x0102;
   /* 列印出動作系統及其版本號 */
   if(uname(&uts) < 0) 
      err_exit("Could not get host information\n");
   printf(" %s-%s-%s: ", uts.machine, uts.sysname, uts.release);
    /* 檢驗位元組順序 */
   if(sizeof(short) != 2)
      printf("sizeof short =%d\n",sizeof(short));
   else if(un.c[0]==1 && un.c[1]==2)
      printf("big_endian\n");
   else if(un.c[0]==2 && un.c[1]==1)
      printf("little_endian\n");
   else
      printf("unknown\n");
   exit(0);
}
