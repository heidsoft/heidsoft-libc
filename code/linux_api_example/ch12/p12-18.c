#include "ch12.h"
#include "p12-7.c"   // make_socket()
#define MAXMSG  1024

int main(void)
{
   int sock,size,nbytes;
   struct sockaddr_in name;
   char message[MAXMSG];

   /* 建立資料報套接字 */
   sock = make_socket(SOCK_DGRAM, 0);

   /* 查出並列印系統賦給的通訊埠值 */
   size = sizeof(name);
   if (getsockname(sock, (struct sockaddr *)&name, (socklen_t *)&size)) 
      err_exit("getting socket name");
   printf("Socket has port #%d\n", ntohs(name.sin_port));

   while(1){
      /* 從套接字讀資料 */
      size = sizeof(name);
      nbytes = recvfrom(sock, message, MAXMSG, 0,
                  (struct sockaddr *)&name, (socklen_t *)&size);
      if (nbytes < 0)
         err_exit("recfrom (server)");
      /* 列印出收到的訊息 */
      printf("Server got message: %s\n", message);
      /* 將訊息傳回給傳送者 */
      nbytes = sendto(sock, message, nbytes, 0,(struct sockaddr *)&name, size);
      if (nbytes < 0)
         err_exit("sendto(server)");
   }
}
