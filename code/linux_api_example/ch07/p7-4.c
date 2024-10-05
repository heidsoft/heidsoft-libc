#include "ch07.h"
volatile sig_atomic_t keep_going = 1;
void catch_alarm(int sig)
{
    keep_going = 0;
    puts("catched alarm");
}
void do_stuff(void)
{
   puts("Doing work while wait for alarm ... ");
}
int main(void)
{
    signal(SIGALRM, catch_alarm);
    alarm(2);
    while(keep_going)
       do_stuff();
    puts("SUCCESS"); 
    return 0;
}

