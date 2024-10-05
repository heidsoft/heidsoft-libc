#include "ch12.h"
#include "p12-7.c"  // make_socket()
#define LISTENQ  5
#define MAXLINE  512
int main(int argc, char **argv)
{
   int listenfd,connfd;
   socklen_t len;
   struct sockaddr_in servaddr,cliaddr;
   char buff[MAXLINE];
   time_t ticks;
   /* 建立並命名套接，5013為服務通訊埠 */
   listenfd = make_socket(SOCK_STREAM, 5013);  
   listen(listenfd, LISTENQ);    /* 建立監聽佇列 */
   /* 連線處理 */
   for(; ;){
     len = sizeof(cliaddr);
      /* 接收連線 */
     connfd=accept(listenfd, (struct sockaddr *)&cliaddr, &len);
      /* 列印出連線客戶的IP位址 */
     printf("connect from %s,port %d\n", 
                    inet_ntoa((struct in_addr)cliaddr.sin_addr),
                    ntohs(cliaddr.sin_port));
      /* 向客戶報告時間 */
     ticks = time(NULL);
     sprintf(buff,"%.24s\r\n",ctime(&ticks));
     write(connfd,buff,strlen(buff));
     close(connfd);        /* 關閉連線套接字 */
   }
}
