#include "p11-6-7.h"
int main()
{
   int server_fifo_fd, client_fifo_fd;
   struct data_to_pass my_data;
   int nbytes;
   char client_fifo_name[256], *tmp_char_ptr;
   /* 建立並開啟服務FIFO */
   mkfifo(SERVER_FIFO_NAME, 0777);
   server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);
   if (server_fifo_fd == -1) 
       err_exit("Server fifo failure");
   do {
       /* 接收來自客戶的請求 */    
       nbytes = read(server_fifo_fd, &my_data, sizeof(my_data));
       if (nbytes > 0) { /* 對客戶的請求進行處理 */
           tmp_char_ptr = my_data.data;
          while (*tmp_char_ptr) {
             if (isupper(*tmp_char_ptr))
                *tmp_char_ptr = tolower(*tmp_char_ptr);
             else if  (islower(*tmp_char_ptr))
                *tmp_char_ptr = toupper(*tmp_char_ptr);
             else if (*tmp_char_ptr == '\n')
                *tmp_char_ptr = '\0';
             tmp_char_ptr++;
          }
           /* 將處理後的資料送回給客戶。為此根據客戶的執行緒ID確定回答所用FIFO */
           sprintf(client_fifo_name, CLIENT_FIFO_NAME, my_data.client_pid);
           client_fifo_fd = open(client_fifo_name, O_WRONLY);
           if (client_fifo_fd != -1) {
               write(client_fifo_fd, &my_data, sizeof(my_data));
               close(client_fifo_fd);
           }
       }
   } while (nbytes > 0);
   close(server_fifo_fd);
   unlink(SERVER_FIFO_NAME);
   return 0;  
}
