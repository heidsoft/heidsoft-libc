#include <sys/types.h>
#include <sys/utsname.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include "err_exit.h"

int sockatmark(int fd)
{
  int flag;
  if(ioctl(fd,SIOCATMARK,&flag)<0)
    return-1;
  return(flag!=0?1:0);
}
