#include "p11-7.h"
int main(void)
{
   int running = 1, msgid;    
   struct my_msg msgbuf;
   long int msg_type = -57;    /* 優先接收型態小於57的訊息 */
   /* 建立/開啟訊息佇列 */
   if ((msgid = msgget((key_t)1234, 0666|IPC_CREAT)) == -1) 
      err_exit("msgget failed");
   /* 從佇列中反復接收訊息，直到收到字串”end” */
   while(running) {/* 優先接收型態小於57的訊息,不等待 */
      if (msgrcv(msgid, (void *)&msgbuf, BUFSIZ, -57, IPC_NOWAIT)!= -1)
         printf("You wrote: %s", msgbuf.text);        /* 顯示收到的訊息 */
      else if (errno==ENOMSG /* 按標準順序接收訊息 */
                && msgrcv(msgid, (void *)&msgbuf, BUFSIZ, 0, 0) != -1)
         printf("You wrote: %s", msgbuf.text);        /* 顯示收到的訊息 */
      else 
         err_exit ("msgrcv failed\n");
      if (strncmp(msgbuf.text, "end", 3) == 0)
         break;
   }
   /* 移除訊息佇列 */
   if (msgctl(msgid, IPC_RMID, 0) == -1) 
      err_exit("msgctl(IPC_RMID) failed\n");
   exit(EXIT_SUCCESS);
}
