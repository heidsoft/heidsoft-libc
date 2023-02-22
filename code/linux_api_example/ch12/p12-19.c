#include "ch12.h"
#include "p12-6.c"  //init_sockaddr()
#define MAXMSG 512
int main(int argc, char **argv) 
{ 
   int sock,n; 
   struct sockaddr_in name; 
   struct hostent *hp;
   char sndmsg[MAXMSG],rcvmsg[MAXMSG];
   /* 检查参数的合法性 */
   if (argc < 3) {
      fprintf(stderr, "Usage: a.out <hostname> <port>\n");
      exit(1);
   }
   /* 创建数据报套接字 */ 
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
      err_exit("opening datagram socket error"); 
   /* 形成发送目的地套接字地址 */ 
   init_sockaddr(&name, argv[1], argv[2]); 
   while(1) {
      printf("Please enter any string:");
      fgets(sndmsg,MAXMSG,stdin);
      if (sndmsg[0]=='\n')
         break;
       /* 发送从终端读入的消息 */
      if (sendto(sock, sndmsg, sizeof(sndmsg), 0, 
              (struct sockaddr *)&name, sizeof(name)) < 0) 
         err_exit("(client) sending error");
       /* 接收并显示服务返回的回答 */
      if ((n = recvfrom(sock,rcvmsg, MAXMSG,0,NULL,NULL)) < 0)
         err_exit("(client) receive error ");
      rcvmsg[n] = 0;
      printf("I received echo: %s",rcvmsg);
   }
   close(sock);
   exit(0); 
}

