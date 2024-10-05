#include "ch07.h"
void (*signal(int sig, void (*handler) (int))) (int) 
{
    struct sigaction act, oact;
    /* 浪琩癟腹*/
    if (handler == SIG_ERR || sig<1 || sig>=NSIG){ 
       errno = EINVAL;
       return SIG_ERR;
    }
    act.sa_handler = handler;
    if (sigemptyset (&act.sa_mask) < 0)
       return SIG_ERR;
    act.sa_flags = 0;
    if(sig != SIGALRM)
       act.sa_flags |= SA_RESTART;
    if (sigaction(sig, &act, &oact) < 0)
       return (SIG_ERR);
    return(oact.sa_handler);
}

