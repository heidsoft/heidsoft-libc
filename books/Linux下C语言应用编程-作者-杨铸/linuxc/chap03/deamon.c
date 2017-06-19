#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int daemon_init(void)
{
        pid_t   pid, i;

        if ( (pid = fork()) < 0)
                return(-1);
        else if (pid != 0)
                exit(0);        /* parent goes bye-bye */
        /* child continues */
        setsid();               /* become session leader */
        chdir("/tmp");             /* change working directory */
        umask(0);               /* clear our file mode creation mask */
	for (i = 0; i < sysconf(_SC_OPEN_MAX); i++)
		close(i);
        return(0);
}

int main(void)
{
	int i = 0;
	daemon_init();
	//openlog("daemon_example", LOG_CONS | LOG_PID | LOG_PERROR, LOG_DAEMON);
	openlog("daemon_example", LOG_CONS | LOG_PID, LOG_DAEMON);
	for (i = 0; 1; i++) {
		// here do something, if error occured, output system log
		syslog(LOG_INFO, "deamon_example log output %d\n", i);
		sleep(3);
	}
	closelog();
	return 0;
}
