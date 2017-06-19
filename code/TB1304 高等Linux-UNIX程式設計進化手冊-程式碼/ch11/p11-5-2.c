#include "p11-6-7.h"
int main()
{
   int server_fifo_fd, client_fifo_fd;
   struct data_to_pass my_request;
   char client_fifo_name[256];
   pid_t mypid;
   /* 開啟服務管線 */   
   server_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
   if (server_fifo_fd == -1) 
       err_exit("Sorry, no server");
   /* 建立以執行緒ID命名的客戶接收有名管線 */
   mypid = getpid();
   sprintf(client_fifo_name, CLIENT_FIFO_NAME, mypid);
   if (mkfifo(client_fifo_name, 0777) == -1)
       err_exit(client_fifo_name);
   /* 向服務執行緒連續傳送和接收資料 */
   while (1) {
       /* 形成給服務執行緒的資料 */
       printf ("Enter your request:");
       fgets (my_request.data, BUFFER_SIZE, stdin);
       if (strcmp(my_request.data,"bye\n")==0
                      ||strcmp(my_request.data, "BYE\n")==0)
           break;
       my_request.client_pid = mypid;
       /* 向服務執行緒發出請求 */
       write(server_fifo_fd, &my_request, sizeof(my_request));
       /* 從服務執行緒接收回答，為此，先開啟客戶接收有名管線 */
       client_fifo_fd = open (client_fifo_name, O_RDONLY);
       if (client_fifo_fd != -1) {
           if (read (client_fifo_fd, &my_request, sizeof(my_request)) > 0) {
               printf ("---I get answer: %s\n", my_request.data);
           }
           close(client_fifo_fd); 
       }
    }
     /* 關閉服務FIFO並移除客戶接收FIFO */   
    close(server_fifo_fd);
    unlink(CLIENT_FIFO_NAME);
    exit(EXIT_SUCCESS);
}
