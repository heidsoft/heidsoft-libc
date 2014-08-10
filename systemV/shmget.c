

#include "systemv.h"

int main(int argc , char **argv)
{
    int  c, id, oflag;

    char *ptr;

    size_t length;

    oflag = SVSHM_MODE | IPC_CREAT;

    while( (c= getopt(argc, argv, "e")) != -1){
       switch(c){
           case 'e':
              oflag |= IPC_CREAT;
               break;
       }
    }

    if(optind!=argc-2){
      fprintf(stderr,"usage: shmget [-e | <pathname> <length>]");
    }

    length = atoi(argv[optind + 1]);
    
    id= shmget(ftok(argv[optind], 0), length, oflag);
    ptr = shmat(id, NULL , 0);

    return 0;
}
