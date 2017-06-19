#include "ch12.h"
#include "p12-7.c"    // make_socket()
int listenfd, connfd;
void sig_urg(int signo);
//#include "newp12-17.c" 

int main(int argc,char **argv) /* 執行指令為： <程式名> <通訊埠號> */
{
   int n;
   char buff[40];
   sigset_t mask, oldmask;
   struct sigaction action;

   if(argc != 2) {   /* 檢查參數*/
      printf("Usage: a.out <port#>\n");
      exit(1);
   }
   /* 設定sigaction結構指明訊號動作. */
   action.sa_handler = sig_urg;
   sigemptyset(&action.sa_mask);
   action.sa_flags = SA_RESTART;
   sigemptyset(&mask);
   sigaddset(&mask, SIGURG);
   action.sa_mask = mask;
   /* 設定SIGURG的控制碼 */
   sigaction(SIGURG, &action , NULL);
   /* 暫時阻塞SIGURG中斷. */
   sigemptyset(&mask);
   sigaddset(&mask, SIGURG);
   sigprocmask (SIG_BLOCK, &mask, NULL);
   listenfd = make_socket(SOCK_STREAM, atoi(argv[1])); /* 建立監聽套接字 */
   listen(listenfd, 5);
   connfd = accept(listenfd, NULL, NULL); /* 接收連線 */
   /*  下面這一行與程式12-17 舉出的訊號控制碼配合使用 */
   /* setsockopt(listenfd,SOL_SOCKET,SO_OOBINLINE,&n,(socklen_t)sizeof(n)); */
   fcntl(connfd,F_SETOWN,getpid());
   /* 放開對SIGURG的阻塞 */ 
   sigprocmask (SIG_UNBLOCK, &mask, NULL);
   sleep(1);    // 讓程式p12-14執行
   while(1){
      if ((n = read(connfd, buff, 18 /*sizeof(buff)-1i*/)) == 0){
         printf("received EOF\n");
         exit(0);
      } else if(n>0) {
         buff[n] = 0;
         printf("read %d bytes:   %s\n",n, buff);
      } else if(errno == EINTR)
         continue;
      else
         err_exit("read error");
   }
}
void sig_urg(int signo)/* SIGURG訊號處理控制碼，用程式12-17時屏蔽此控制碼 */  
{
   int n;
   char oob_buf[8];
   fprintf(stderr, "SIGURG received: ");
   n = recv(connfd,oob_buf,sizeof(oob_buf),MSG_OOB);
   if(n<0) 
      err_exit("read OOB error");
   oob_buf[n] = 0;
   fprintf(stderr, "read %d OOB byte: %s\n",n,oob_buf);
}

