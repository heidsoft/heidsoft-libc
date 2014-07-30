/* 
 * File:   main.cpp
 * Author: bin.liu
 * Email:  heidsoftg@gmail.com
 * Created on November 10, 2013, 7:26 PM
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/**
 * fn1
 * @param arg
 * @return 
 */
void *thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    //线程id是无符号整型
    printf("my thread 1 id=%u\n",pthread_self());
    return ((void *)1);
}

/**
 * fn2
 * @param arg
 * @return 
 */
void *thr_fn2(void *arg)
{
    printf("thread 2 exitting\n");
    printf("my thread 2 id=%u\n",pthread_self());
    pthread_exit((void*)2);
}

/**
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) 
{
    int err;
    pthread_t tid1,tid2;
    void *thret;
    
    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err !=0){
        printf("can't create thread 1: %s\n",strerror(err));
        exit(0);
    }
    
    err = pthread_create(&tid2,NULL,thr_fn2,NULL);
    if(err !=0){
        printf("can't create thread 2: %s\n",strerror(err));
        exit(0);
    }
    
    err = pthread_join(tid1,&thret);
    if(err!=0){
        printf("can't join thread 1: %s\n",strerror(err));
    }
    printf("thread 1 exit code is %d\n",(int*)thret);
    err = pthread_join(tid2,&thret);
    if(err!=0){
        printf("can't join thread 1: %s\n",strerror(err));
    }
    
    err = pthread_join(tid2,&thret);
    if(err!=0){
        printf("can't join thread 2: %s\n",strerror(err));
    }
    //thret 是void 的指针
    printf("thread 1 exit code is %d\n",(int*)thret);

    
    return 0;
}

