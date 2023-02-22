#include "ch06.h"
#include "p6-3.c" //pr_exit()

/*  parse--分解buf中的指令為獨立的參數  */
void parse(char *buf, char *args[])
{
   int i=0;
   while (*buf != '\0') {
      /* 用空字元'\0'替代空白字元使得前一參數以空字元結束  */       
      args[i++]=buf;
      while ((*buf!=' ')&&(*buf!='\t')&&(*buf!='\n')) buf++;
      while ((*buf==' ')||(*buf=='\t'||(*buf=='\n'))) *buf++ = '\0';
   }
   args[i]='\0';
}

/*  execute--衍生一子執行緒執行此程式  */
void execute(char *args[])
{
   int pid, status;
   /*  建立一子執行緒  */
   if ((pid=fork()) <0) 
      err_exit("fork");
   /*  子執行緒執行if內的程式碼  */
   if (pid==0) {
      execvp(*args,args);
      err_exit("execvp");
   } 
   /*  父執行緒等待子執行緒完成  */
   waitpid(pid, &status, 0);
   pr_exit(status,pid);
}
int main(void)
{
   char buf[1024];
   char *args[64], *cp;

   for (;;) {
     printf("Command: ");   // 舉出提示符
     cp = fgets (buf, sizeof(buf), stdin);    //讀入指令
     if (cp == (char *)NULL || *cp == '\n'){  // 空行表示離開
        printf("quit\n");
        exit(EXIT_SUCCESS);
     }
     /*  分解字串為參數  */
     parse(buf, args);
     /*  執行指令  */
     execute(args);
   }
}

