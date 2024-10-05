#include "ch12.h"
int main(int argc, char *argv[])
{
   char *host, **names, **addrs, myname[256];
   struct hostent *hostinfo;
   /* 設定主電腦為指令行參數特殊的名字或程式執行所在機器的名字 */
   if(argc > 1) 
  host = argv[1];
   else {
      gethostname(myname, 255);
      host = myname;
   }   
   hostinfo = gethostbyname(host);       /* 取得主電腦位址訊息 */
   if(!hostinfo) {
      fprintf(stderr, "cannot get info for host: %s\n", host);
      exit(1);
   }
   /* 報告它的正式名和別名 */
   printf("results for host %s:\n", host);
   printf("Official name: %s\n", hostinfo->h_name);
   printf("\t Aliases:");
   names = hostinfo->h_aliases;
   while(*names) {
      printf(" %s\n", *names);
      names++;
   }
    /* 報告IP位址 */
   addrs = hostinfo->h_addr_list;
   while(*addrs) {
      printf("\taddress:%s\n",inet_ntoa(*(struct in_addr*)*addrs));
       addrs++;
   }
   exit(0);
}
