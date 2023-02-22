#include "p11-7.h"

int main()
{
   int msgid, running = 1;
   struct my_msg msgbuf;
   struct msqid_ds buf;
   /* 獲得訊息佇列id */
   msgid = msgget((key_t)1234, 0666|IPC_CREAT);
   if (msgid == -1) 
       err_exit("msgget failed\n");
 
   /* 反復將使用者輸入的資料傳送至訊息佇列，字串“end”標志輸入結束 */
   while(running) {
       /* 讀輸入字串至訊息緩沖區 */
       printf("Enter some text: ");         
       fgets(msgbuf.text, BUFSIZE, stdin);
       msgbuf.my_msg_type = (long int)msgbuf.text[0]; /* 第一個字元作為訊息型態 */
       /* 向訊息佇列傳送訊息，阻塞模式 */
       if (msgsnd(msgid, (void *)&msgbuf, sizeof(struct my_msg), IPC_NOWAIT) == -1) 
          err_exit("msgsnd failed\n");
       /* 若果是“end”，結束循環 */
       if (strncmp(msgbuf.text, "end", 3) == 0) 
          running = 0;
   }
   exit(EXIT_SUCCESS);
}
