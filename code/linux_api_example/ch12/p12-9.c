#include "ch12.h"
#include "p12-8.c"  // socket_connect()
#define  MAXBUFFSIZE    256
int main(int argc, char **argv)
{
   int sockfd, n;
   char recvbuff[MAXBUFFSIZE], *host;
   struct sockaddr_in servaddr;
   
   if(argc < 2)    /* 檢查參數 */
      host = NULL;
   else
      host = argv[1]; 
   sockfd = socket_connect(host,"13");  /* 建立套接字並建立與服務的連線 */
   /* 讀服務的回答並顯示結果 */
   while((n = read(sockfd,recvbuff,MAXBUFFSIZE)) > 0){
      recvbuff[n] = 0;    /* 填入終止符 */
      fputs(recvbuff,stdout);
   }
   if(n<0)
      err_exit("read error");
   exit(0);
}
