#include "ch07.h"
static volatile sig_atomic_t sigflag = 0;
static void sig_usr (int signo)
{
    psignal (signo, "   recieved");
}
static void sig_intr (int signo)
{
    psignal (signo, "   received");
    sigflag = 1;
}
void wait_for_signal (int sig, volatile sig_atomic_t *sflag)
{
    static sigset_t mask,oldmask;
    sigemptyset(&mask);     
    sigaddset(&mask,sig);
    /* 阻塞訊號sig，並儲存目前訊號屏蔽 */
    if (sigprocmask(SIG_BLOCK,&mask,&oldmask)<0){
       printf("SIG_BLOCK error");
       exit(1);
    }
    sigdelset (&oldmask, SIGUSR1);   /* 這兩行是為範例而加入的非必要程式碼 */
    sigdelset (&oldmask, SIGUSR2);
    while (!*sflag){                    /* 等待訊號 */
       printf ("door opened\n");
       sigsuspend(&oldmask);       
       printf ("door closed\n");
    }
    *sflag = 0;
    if (sigprocmask(sig, &oldmask, NULL)<0)   /* 還原訊號屏蔽至原來的值 */
       printf("SIG_SETMASK error");
}
int main(void)
{
    pid_t pid;
    static sigset_t mask,oldmask;
    signal (SIGUSR1, sig_usr);
    signal (SIGUSR2, sig_usr);
    signal (SIGINT, sig_intr);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);   /* 為範例目的先阻塞這兩個訊號 */
    sigaddset(&mask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK,&mask,&oldmask)<0){
       printf("SIG_BLOCK error");
       exit(1);
    }
    if ((pid = fork()) == 0) {     /* 子執行緒  */
       kill (getppid(), SIGUSR1);
       kill (getppid(), SIGUSR2);
       printf ("child ok");
       while (1);
    } else {             /* 父執行緒  */
       wait_for_signal (SIGINT, &sigflag);
       printf ("Now, I can do my work.  :)\n");
       kill (pid, SIGTERM);
    }
    exit(0);
}
