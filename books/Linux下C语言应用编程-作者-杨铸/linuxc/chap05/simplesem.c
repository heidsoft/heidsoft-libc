#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXSIZE 10

int stack[MAXSIZE][2];
int size = 0;
sem_t sem;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void provide_data(void)
{
	int i;
	for (i = 0; i < MAXSIZE; i++) {
		stack[i][0] = i;
		stack[i][1] = i;
		sem_post(&sem);
	}
}

void handle_data1(void)
{
	int i;
	while (pthread_mutex_lock(&mutex), ((i = size++) < MAXSIZE)) {
		pthread_mutex_unlock(&mutex);
		sem_wait(&sem);
		printf("Plus:    %d + %d = %d\n", stack[i][0], stack[i][1], stack[i][0] + stack[i][1]);
//		sleep(1);
	}
	pthread_mutex_unlock(&mutex);
}

 
void handle_data2(void)
{
	int i;
	while (pthread_mutex_lock(&mutex), ((i = size++) < MAXSIZE)) {
		pthread_mutex_unlock(&mutex);
		sem_wait(&sem);
		printf("Multiple: %d * %d = %d\n", stack[i][0], stack[i][1], stack[i][0] * stack[i][1]);
//		sleep(1);
	}
	pthread_mutex_unlock(&mutex);
}

int main(void)
{
	pthread_t thrd1, thrd2, thrd3;
	sem_init(&sem, 0, 0);
	pthread_create(&thrd1, NULL, (void *)handle_data1, NULL);
	pthread_create(&thrd2, NULL, (void *)handle_data2, NULL);
	pthread_create(&thrd3, NULL, (void *)provide_data, NULL);
	pthread_join(thrd1, NULL);
	pthread_join(thrd2, NULL);
	pthread_join(thrd3, NULL);
	sem_destroy(&sem);
	return 0;
}
