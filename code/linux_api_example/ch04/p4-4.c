#include "ch04.h"

char *get_perms(struct stat *sbuf, char *perms)
{
   static char *modes[] = {   /* 與每一種存取權限許可值對應的字串 */
       "---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"
   };  
   int i,j;

   *perms = '\0';
   /* 分別獲得三群組存取權限值，用該值作為陣列modes的索引，以便獲得對應的字串拼入perms */
   for (i=2; i>=0; i--) {
      j=(sbuf->st_mode>>(i*3)) & 07;
      strcat( perms,modes[j]);
   }
   /*  處理調整ID位和sticky位 */
   if ((sbuf->st_mode & S_ISUID) !=0)
      perms[2] = 's';
   if ((sbuf->st_mode & S_ISGID) !=0)
      perms[5] = 's';
   if ((sbuf->st_mode & S_ISUID) !=0)
      perms[8] = 't';
   return perms;
}
