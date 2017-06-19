/* simplemutex.c
 * access same global variable for adding it
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void task1(void);
void task2(void);
void task3(void);
int sharedi=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int main(void) {
	pthread_t thrd1, thrd2, thrd3;
	int ret;
/*
	ret = pthread_create(&thrd3, NULL, (void *)task3, NULL);
	if (ret) {
		perror("pthread_create: task3");
		exit(EXIT_FAILURE);
	}
//	sleep(2);
*/
	ret = pthread_create(&thrd1, NULL, (void *)task1, NULL);
	if (ret) {
		perror("pthread_create: task1");
		exit(EXIT_FAILURE);
	}
	ret = pthread_create(&thrd2, NULL, (void *)task2, NULL);
	if (ret) {
		perror("pthread_create: task2");
		exit(EXIT_FAILURE);
	}
	pthread_join(thrd1, NULL);
	pthread_join(thrd2, NULL);
	printf("sharedi = %d\n", sharedi);
/*
	printf("befor join thrd3\n");
	pthread_join(thrd3, NULL);
	printf("after join thrd3\n");
*/
}

void task1(void)
{
	long i,tmp;
	for(i=0; i<1000000; i++) {
/*
		if(pthread_mutex_lock(&mutex) != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
*/
		tmp = sharedi;
		tmp = tmp + 1;
		sharedi = tmp;
/*
		if (pthread_mutex_unlock(&mutex) != 0) {
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
*/
	}
}

void task2(void)
{
	long i,tmp;
	for(i=0; i<1000000; i++) {
/*
		if(pthread_mutex_lock(&mutex) != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
*/
		tmp = sharedi;
		tmp = tmp + 1;
		sharedi = tmp;
/*
		if (pthread_mutex_unlock(&mutex) != 0) {
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
*/
	}
}

void task3(void)
{
	long i,tmp;
	for(i=0; i<1000000; i++) {

		if(pthread_mutex_lock(&mutex) != 0) {
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		tmp = sharedi;
		tmp = tmp + 1;
		sharedi = tmp;

		if (pthread_mutex_unlock(&mutex) != 0) {
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}

	}
}
