#include "ch12.h"
int main(void)
{
   int    stayopen = 1;
   struct servent *sp;
   setservent(stayopen);    /* 開啟服務資料庫，準備掃描 */
   while (1) {                /* 逐項掃描登記項 */
      sp = getservent();
      if (sp != (struct servent *)0)
         printf( "server name=%12s, port=%6d, proto=%4s\n", sp->s_name,
                   ntohs(sp->s_port), sp->s_proto);
      else
         break;
   }
   endservent();    /* 關閉服務資料庫 */
   /* 專門檢視telnet服務的通訊埠號 */
   sp = getservbyname ("telnet", "tcp");
   if (sp != (struct servent *)0) 
      printf( "telnet's port is %d\n", ntohs (sp->s_port));
   else
      printf("ERROR: getservbyname call failed\n");
   return 0;
}
