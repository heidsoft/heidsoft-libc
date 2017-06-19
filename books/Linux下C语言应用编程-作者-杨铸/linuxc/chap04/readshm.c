/* B½ø³Ì */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSZ 4096	/* Size of the segment */

int main(void)
{
	int shmid, readsemid, writesemid;
	char *shmbuf;	/* Address in process */
	struct sembuf buf;

	if((shmid = shmget(888, BUFSZ, 0666 | IPC_CREAT)) < 0) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	/* Attach the segment */
	if((shmbuf = shmat(shmid, 0, 0)) < (char *)0) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	
	if((readsemid = semget(999, 1, 0666 | IPC_CREAT)) < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}
	if((writesemid = semget(1099, 1, 0666 | IPC_CREAT)) < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

/* in case read process or write process abnormally teminated in key area */
	if ((semctl(writesemid,0,GETVAL) == 0) && (semctl(readsemid,0,GETVAL) == 0)) {
		buf.sem_num = 0;
		buf.sem_op = 1;
		if((semop(writesemid, &buf, 1)) < 0) {
			perror("semop");
			exit(EXIT_FAILURE);
		}
	}
	
	while(1) {
		printf("in while\n");

		buf.sem_num = 0;
		buf.sem_op = -1;
		//buf.sem_flg = IPC_NOWAIT;
		buf.sem_flg = 0;
		if((semop(readsemid, &buf, 1)) < 0) {
			perror("semop");
			exit(EXIT_FAILURE);
		}

		write(STDOUT_FILENO,shmbuf,1);
		sleep(3);

		buf.sem_num = 0;
		buf.sem_op = 1;
		//buf.sem_flg = IPC_NOWAIT;
		if((semop(writesemid, &buf, 1)) < 0) {
			perror("semop");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}

