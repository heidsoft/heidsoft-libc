#include "ch11.h"
char * SERVER_FIFO_NAME = "/tmp/serv_fifo";
char * CLIENT_FIFO_NAME = "/tmp/client_%d_fifo";
#define BUFFER_SIZE 80
struct data_to_pass {  
    pid_t  client_pid; 
    char  data[BUFFER_SIZE];
};

