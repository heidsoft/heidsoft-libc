#include <sys/types.h>
#include <sys/time.h>
#include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>

/*
定义新建文件的权限
 */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/**
 *定义新的sv message queues 权限
 */
#define SVMSG_MODE (MSG_R | MSG_W | MSG_R>>3 | MSG_R>>6)

#define SVSEM_MODE (SEM_R | SEM_A | SEM_R>>3 | SEM_R>>6)

#define SVSHM_MODE (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)
/*
SYSTEM_V IPC
*/
//#ifdef HAVE_SYS_IPC_H
#include <sys/ipc.h>
//#endif

//#ifdef HAVE_SYS_SEM_H
#include <sys/shm.h>
//#endif


