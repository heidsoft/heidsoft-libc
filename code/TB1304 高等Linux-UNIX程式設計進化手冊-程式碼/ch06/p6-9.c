#include "ch06.h" 

 /* 一個 process 代表一個執行緒.  */
typedef struct process
{
   struct process *next;       /* next process in pipeline */
   char **argv;                /* for exec */
   pid_t pid;                  /* process ID */
   char completed;             /* true if process has completed */
   char stopped;               /* true if process has stopped */
   int status;                 /* reported status value */
} process;

/* 一個job 代表由管線連線的許多個執行緒群組成的一個作業.  */
typedef struct job
{
   struct job *next;           /* next active job */
   char *command;              /* command line, used for messages */
   process *first_process;     /* list of processes in this job */
   pid_t pgid;                 /* process group ID */
   char notified;              /* true if user told about stopped job */
   struct termios tmodes;      /* saved terminal modes */
   int jstdin, jstdout, jstderr;  /* standard i/o channels */
} job;

/* 活躍的作業群組成鏈結串列。這是其標頭 */
job *first_job = NULL;

pid_t shell_pgid;     
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

/* 確保該shell 在開始工作之前是互動執行的前景作業 */
void init_shell(void)
{
   /* 檢查是否互動執行，即判定與STDIN_FILENO相連的是否終端 */
   shell_terminal = STDIN_FILENO;
   shell_is_interactive = isatty(shell_terminal);
   shell_pgid = getpid();

   if (shell_is_interactive) {  /* 是互動執行shell */
      /* 檢查是否在前景執行，若果執行緒群組ID不同於控制終端的執行緒群組ID則是在背景執行，
         因而必鬚髮送停止訊號，此過程循環直到使用者將自己放置於前景 */
      while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
         kill(- shell_pgid, SIGTTIN);
      /* 忽略互動和作業控制訊號 */
      signal(SIGINT, SIG_IGN);
      signal(SIGQUIT, SIG_IGN);
      signal(SIGTSTP, SIG_IGN);
      signal(SIGTTIN, SIG_IGN);
      signal(SIGTTOU, SIG_IGN);
      signal(SIGCHLD, SIG_IGN);
      /* 設定shell的執行緒群組 */
      if (setpgid(shell_pgid, shell_pgid) < 0)
         err_exit("Couldn't put the shell in its own process group");
      /* 搶奪控制終端 */
      tcsetpgrp(shell_terminal, shell_pgid);
      /* 儲存shell的預設終端屬性以便離開時還原 */
      tcgetattr(shell_terminal, &shell_tmodes);
   }
}

void launch_process (process *p, pid_t pgid, int infile, 
                     int outfile, int errfile, int foreground)
{
   pid_t pid;

   if (shell_is_interactive) {
      /* 放置該執行緒至執行緒群組。因為競爭的緣故，子執行緒和shell二者都必須這樣做 */
      pid = getpid();
      if (pgid == 0)          /*  pgid 為0表示這是執行緒群組的第一個執行緒 */       
         pgid = pid;         /*  使它成為執行緒群組群組長 */
      setpgid(pid, pgid);
      /*若是前景執行緒，給執行緒群組控制終端。*/ 

      if (foreground)
         tcsetpgrp(shell_terminal, pgid);

      /* 設定作業控制訊號的控制碼回到預設 */
      signal(SIGINT, SIG_DFL);
      signal(SIGQUIT, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
      signal(SIGTTIN, SIG_DFL);
      signal(SIGTTOU, SIG_DFL);
      signal(SIGCHLD, SIG_DFL);
   }

   /* 設定新執行緒的標准I/O通道 */
   if (infile != STDIN_FILENO) {       /* 管線輸入情形  */
      dup2(infile, STDIN_FILENO);    /* 複製管線描述字至標准輸入描述字 */
      close(infile);                 /* 關閉管線描述字，以下類別似 */
   }
   if (outfile != STDOUT_FILENO) {
      dup2(outfile, STDOUT_FILENO);
      close(outfile);
   }
   if (errfile != STDERR_FILENO) {
      dup2(errfile, STDERR_FILENO);
      close(errfile);
   }

   /* 執行新執行緒 */
   execvp (p->argv[0], p->argv);
   err_exit("execvp");
}

void launch_job(job *j, int foreground)
{
   process *p;
   pid_t pid;
   int mypipe[2], infile, outfile;

   infile = j->jstdin;      /* 第一個執行緒的標准輸入由指令掃描程式指定 */
   for (p = j->first_process; p; p = p->next) {
      /* 有下一個執行緒，則是用管線連線的。建立管線
         使得這個執行緒的輸出成為下一個執行緒的輸入 */
     if (p->next) {       
        if (pipe(mypipe) < 0) 
           err_exit("pipe");
        outfile = mypipe[1];
     } else
        outfile = j->jstdout;
     /* Fork 子執行緒 */
     if ((pid = fork()) < 0)    
        err_exit("fork");
     if (pid == 0)          /* 子執行緒 */
        launch_process(p, j->pgid, infile, outfile, j->jstderr, foreground);
     else{                    /* 父執行緒 */
        /* 放置該執行緒至執行緒群組。因為競爭的緣故，子執行緒和shell二者都必須這樣做 */
        p->pid = pid;
        if (shell_is_interactive) {
           if (!j->pgid)
              j->pgid = pid;      /* 記錄作業的執行緒群組ID */
           setpgid(pid, j->pgid);
        }
     }
     /* 建立管線之後的清理工作 */
     if (infile != j->jstdin)
        close(infile);
     if (outfile != j->jstdout)
        close(outfile);
     infile = mypipe[0];    /* 下一個執行緒的標准輸入是這個執行緒的管線輸出 */     
   }
   format_job_info(j, "launched");  /* 格式化輸出作業狀態訊息 */
   if (!shell_is_interactive)
      wait_for_job(j);
   else if (foreground)
      put_job_in_foreground(j, 0);
   else
      put_job_in_background(j, 0);
}

void put_job_in_foreground(job *j, int cont)
{
   /*  放置該作業於前景.  */
   tcsetpgrp(shell_terminal, j->pgid);
   /*  若果有必要，向該作業傳送一繼續訊號.  */
   if (cont) {
      tcsetattr(shell_terminal, TCSADRAIN, &j->tmodes);
      if (kill(- j->pgid, SIGCONT) < 0)
         perror("kill (SIGCONT)");
   }
   wait_for_job(j);/*  等待它的報告.  */
   tcsetpgrp (shell_terminal, shell_pgid);   /*  使shell重新回到前景.  */
   /*  還原shell的終端模式.  */
   tcgetattr(shell_terminal, &j->tmodes);
   tcsetattr(shell_terminal, TCSADRAIN, &shell_tmodes);
}

/* 放置一個作業至背景。若果cont 參數為真，則向執行緒群組傳送一個SIGCONT訊號以喚醒它 */
void put_job_in_background(job *j, int cont)
{
   /* 若有必要，向該作業傳送繼續訊號。*/
   if (cont)
      if (kill(-j->pgid, SIGCONT) < 0)
         perror("kill (SIGCONT)");
}
