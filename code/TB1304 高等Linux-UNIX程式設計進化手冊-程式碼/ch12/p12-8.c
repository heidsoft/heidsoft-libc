#include "ch12.h"
#include "init_sockaddr.c"
int socket_connect(char *hostname, char *serv_port)
{
   int sockfd;
   struct sockaddr_in saddr;
   struct hostent *hp;
   char *host, myname[104];
   /* 起始伺服器的IP位址和通訊埠 */
   init_sockaddr(&saddr, hostname, serv_port); 
   /* 建立套接字 */
   if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
      err_exit("socket error");
   /* 建立與服務的連線 */
   if(connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
      err_exit("connect error");
   return(sockfd);
}
