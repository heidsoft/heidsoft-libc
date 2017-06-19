#include "ch12.h"
#include "p12-7.c"   // make_socket.c", 程式12-7
void doit(int);
int main(void)
{
   int server_sockfd, client_sockfd;
   int server_len, client_len;
   struct sockaddr_in client_address;
   server_sockfd = make_socket(SOCK_STREAM, 2003);
   /* 建立連線佇列，忽略子執行緒終止訊號 */
   listen(server_sockfd, 5);
   signal(SIGCHLD, SIG_IGN);
   while(1) {
      printf("server waiting\n");
      /* 接收連線 */
      client_len = sizeof(client_address);
      client_sockfd = accept(server_sockfd, 
            (struct sockaddr *)&client_address, &client_len);
      /* 為這個客戶建立一個子執行緒 */
       if(fork() == 0) {             /* 若為子執行緒，接收並回答客戶的訊息 */
         close(server_sockfd);      /* 子執行緒關閉監聽套接字 */
         doit(client_sockfd);       /* 處理客戶的請求 */
         close(client_sockfd);      /* 處理完畢關閉套接字 */
         exit(0);                     /* 子執行緒終止 */
      } else {
         close(client_sockfd);     /* 父執行緒關閉被連線的套接字 */
      }
   }
}
void doit(client_sockfd)
{
   char buf[255];
   int n;
   /* 接收並回應客戶的訊息 */
   n = read(client_sockfd, buf, sizeof(buf));
   buf[n] = 0;
   sleep(1);
   write(client_sockfd, buf, n);
}
