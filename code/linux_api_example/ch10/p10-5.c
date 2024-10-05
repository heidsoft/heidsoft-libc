#include "ch10.h"

void input_ready(int signo);
FILE *file;

int main(void)
{
   int flags;
   char buffer[64];
   struct termios newsettings,oldsettings;

   file=fopen("tstfile", "w");   // 開啟輸出檔案
   signal(SIGIO, input_ready);   // 安裝SIGIO訊號控制碼
 // 設定標准輸入為每次讀入一字元的非加工模式
   tcgetattr(STDIN_FILENO, &oldsettings);
   newsettings = oldsettings;
   newsettings.c_lflag &= (~ICANON);
   newsettings.c_cc[VTIME] = 0;
   newsettings.c_cc[VMIN] = 1;
   tcsetattr(STDIN_FILENO, TCSANOW, &newsettings);
 // 設定標准輸入為無阻塞SIGIO訊號驅動IO模式
   fcntl(STDIN_FILENO, F_SETOWN, getpid());
   flags = fcntl(STDIN_FILENO, F_GETFL, 0);
   flags = flags | O_ASYNC;//|O_NONBLOCK;
   fcntl(STDIN_FILENO, F_SETFL, flags);
   while (1) sleep(1);  // 沒有輸入時休息
}

void input_ready(int signo) // 接收輸入，累積至一行後輸出。行首字元為q終止執行緒
{
   char c,input[80];
   int n;
   static int i=0;

   if (read(STDIN_FILENO, &c, 1)>0) {
      if (c != '\n')
         input[i++]=c;
      else {
        input[i++]='\0';
        fprintf(file, "received %d SIGIOs, and Input line is: %s\n", i, input);
        i=0;
        if (input[0]=='q') {
           fclose(file);
           _exit(0);
        }
      }
   }
}
