#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int global_val = 0;

void*
thread_start_routine(void* data) {
    int val = (int)data;
    int i;
    printf("thread_start_routine : val=%d\n", val);

    for (i = 0; i < 10; i++) {
        global_val++;
        printf("thread_start_routine : i=%d, g=%d\n", i, global_val);
        sleep(1);
    }

    return NULL;
}

int
main() {
    int i;
    pthread_t th;
    void* th_ret;

    if (pthread_create(&th, NULL, thread_start_routine, (void*)100) != 0) {
        perror("pthread_create");
        return 1;
    }

    for (i = 0; i < 10; i++) {
        global_val++;
        printf("main : i=%d, g=%d\n", i, global_val);
        sleep(1);
    }

    if (pthread_join(th, &th_ret) != 0) {
        perror("pthread_join");
        return 1;
    }

    return 0;
}
