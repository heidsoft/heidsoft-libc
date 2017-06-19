/*A½ø³Ì*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSZ 4096	/* Size of the segment */

int main(void)
{
	int shmid, readsemid, writesemid;
	char *shmbuf;	/* Address in process */
	struct sembuf buf;

	if ((readsemid = semget(999, 1, 0666 | IPC_CREAT)) < 0) {
		perror("semget1");
		exit(EXIT_FAILURE);
	}

	if ((writesemid = semget(1099, 1, 0666 | IPC_CREAT)) < 0) {
		perror("semget2");
		exit(EXIT_FAILURE);
	}

	if((shmid = shmget(888, BUFSZ, 0666|IPC_CREAT)) < 0) {
		perror("shmget1");
		exit(EXIT_FAILURE);
	}
	/* Attach the segment */
	if((shmbuf = shmat(shmid, 0, 0)) < (char *)0) {
		perror("shmat1");
		exit(EXIT_FAILURE);
	}
	
/* in case read process or write process abnormally teminated in key area */
	if ((semctl(writesemid, 0, GETVAL) == 0) && (semctl(readsemid, 0, GETVAL) == 0)) {
		buf.sem_num = 0;
		buf.sem_op = 1;
		if ((semop(readsemid, &buf, 1)) < 0) {
			perror("semop");
			exit(EXIT_FAILURE);
		}
	}

	shmbuf[0]= 'a'-1;
	int temp=0;
	while(1) {
		buf.sem_num = 0;
		buf.sem_op = -1;
    		//buf.sem_flg = IPC_NOWAIT;
    		buf.sem_flg = 0;
		if((semop(writesemid, &buf, 1)) < 0) {
			if (errno == EAGAIN) {
				printf("err is EAGAIN\n");
			} else {
				printf("err isn't EAGAIN\n");
			}
			perror("semop2");
			exit(EXIT_FAILURE);
		}

		shmbuf[0] = shmbuf[0] + 1;

		buf.sem_num = 0;
		buf.sem_op = 1;
		buf.sem_flg = IPC_NOWAIT;
		if ((semop(readsemid, &buf, 1)) < 0) {
			perror("semop3");
			exit(EXIT_FAILURE);
		}

		printf("write #%d char: %c\n", ++temp, shmbuf[0]);
		sleep(1);
	}
	exit(EXIT_SUCCESS);
}

