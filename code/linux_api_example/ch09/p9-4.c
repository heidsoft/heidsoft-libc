#include "ch09.h"
struct termios saved_attributes;        /* 用該變數儲存原來的終端屬性 */
void resume_input_mode(void)
{
   tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}
void set_input_mode(int fd)
{
   struct termios tattr;
   char *name;
   if (!isatty(fd)) { /* 確保fd 是終端 */
      fprintf(stderr, "Not a terminal.\n"); exit (EXIT_FAILURE);
   }
   tcgetattr(fd, &saved_attributes); /* 儲存目前終端屬性*/
	   /* 設定終端為非加工、不回應模式 */
   tcgetattr(fd, &tattr);
   tattr.c_lflag &= ~(ICANON|ECHO); /* 清除 ICANON和ECHO */
   tattr.c_cc[VMIN] = 1;  /* 等待至少讀一個字元 */
   tattr.c_cc[VTIME] = 0;
   tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}
int main (void)
{
   char c;
   set_input_mode(STDIN_FILENO);
   while (1){
       read (STDIN_FILENO, &c, 1);
       if (c == '\004')          /* Ctrl-d */
          break;
       else
          printf("I read %c\n",c);
    }
    resume_input_mode(); /* 還原終端屬性 */
    return EXIT_SUCCESS;
 }
