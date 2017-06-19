/*A½ø³Ì*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define BUFSZ 4096	/* Size of the segment */

int main(void)
{
	int shmid;
	char *shmbuf;	/* Address in process */

	if((shmid = shmget(888, BUFSZ, 0666|IPC_CREAT)) < 0) {
		perror("shmget1");
		exit(EXIT_FAILURE);
	}
	
	/* Attach the segment */
	if((shmbuf = shmat(shmid, 0, 0)) < (char *)0) {
		perror("shmat1");
		exit(EXIT_FAILURE);
	}
	shmbuf[0] = 'd';
/*
	shmbuf[0]= 'a'-1;
	int temp=0;
	while(1) {
		shmbuf[0] = shmbuf[0] + 1;
		printf("write #%d char: %c\n", ++temp, shmbuf[0]);
    		sleep(1);
	}
*/
	exit(EXIT_SUCCESS);
}

