#include <poll.h>
#include <stdio.h>
static void choose(int *po1, int *po2)
{
    struct pollfd as[2];
    as[0].fd = *po1;
    as[0].events = POLLIN;
    as[1].fd = *po2;
    as[1].events = POLLIN;
    if (poll(as, 2, 5*1000) < 0) 
       perror("poll");
    if ((as[0].revents&POLLIN) == 0)
       *po1 = -1;
    if ((as[1].revents&POLLIN) == 0)
       *po2 = -1;
}
