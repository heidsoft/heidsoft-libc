#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int       glob = 6;               /* external variable in initialized data */
char    buf[ ] = "a write to stdout\n";
//char    buf[ ] = "a write to stdout";
int  main(void)
{
        int       var;            /* automatic variable on the stack */
        pid_t   pid;
        var = 88;
        if ((write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1))
                { printf("write error"); exit(1); }
        printf("before fork\n");        /* we don't flush stdout */
//        printf("before fork");        /* we don't flush stdout */
        if ( (pid = fork()) < 0)
                { printf("fork error"); exit(2); }
        else if (pid == 0) {            /* child */
                glob++;                                 /* modify variables */
                var++;
        } else
                sleep(2);                               /* parent */
        printf("pid = %d, ppid = %d, glob = %d, var = %d\n", getpid(),getppid(), glob, var);
        exit(0);
}
