//pthread_setcanceltype和pthread_setcanclestate在redhat9下有bug，不能到达预期目的

/*
 * thrdcancel.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void task1(int *counter);
void task2(int *counter);
void cleanup(int counter1, int counter2);

int g1 = 0;
int g2 = 0;
//pthread_t thrd1, thrd2;
int main(int argc, char *argv[])
{
	pthread_t thrd1, thrd2;
	int ret;
  
 /* Create the first thread */
	ret = pthread_create(&thrd1, NULL, (void *)task1, (void *)&g1);
	if (ret) {
		perror("pthread_create: task1");
		exit(EXIT_FAILURE);
	}
  
 /* Create the second thread */
	ret = pthread_create(&thrd2, NULL, (void *)task2, (void *)&g2);
	if (ret) {
		perror("pthread_create: task2");
		exit(EXIT_FAILURE);
	}
	sleep(2);
	pthread_join(thrd2, NULL);
	pthread_cancel(thrd1);
	printf("in main , after pthread_cancel\n");
	pthread_join(thrd1, NULL);
  
	cleanup(g1, g2);
	exit(EXIT_SUCCESS);
}

void task1(int *counter)
{
	volatile int i;
	/* Disable thread cancellation */
	//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	//sleep(1);
	while((*counter) < 5) {
		printf("task1 count: %d\n", *counter);
		(*counter)++;
/*
		for (i=0; i<800000000; i++) {
			pthread_testcancel();
			(*counter)++;
		}
*/
		sleep(1);
	}
	printf("*counter in task1 is : %d\n",*counter);
  /* Enable thread cancellation */
	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

void task2(int *counter)
{
	while(*counter < 5) {
		printf("task2 count: %d\n", *counter);
		(*counter)++;
		//sleep(1);
	}
	//pthread_cancel(thrd1);
}

void cleanup(int counter1, int counter2)
{
	printf("total iterations: %d\n", counter1 + counter2);
}

