#include "mq.h"

int main(int argc , char **argv)
{
 printf("this is a mqcreate test program. \n");
 int c, flags;
 mqd_t mqd;

 flags = O_RDWR | O_CREAT;

 while( ( c = getopt(argc , argv , "e")) != -1){
   switch(c){
      case 'e':
          flags != O_EXCL;
          break;
   }
 }

 if(optind != argc - 1)
     fprintf(stderr,"usage: mqcreate [ -e | <name>]");

 printf("%s\n",argv[optind]);
 printf("flags=%d\n",flags);
 mqd = mq_open(argv[optind],flags,FILE_MODE, NULL);

 mq_close(mqd);
 printf("test mqcreate  end .\n");
 return 0;
}
