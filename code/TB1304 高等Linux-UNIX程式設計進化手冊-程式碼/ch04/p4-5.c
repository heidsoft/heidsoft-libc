#include "ch04.h"

int main(void)
{
   if (creat("aha", S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|
                    S_IROTH|S_IWOTH) < 0)
       err_exit("creat error for aha");
   umask(0); 
   if (creat("bar", S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|
                    S_IROTH|S_IWOTH) < 0)
       err_exit("creat error for bar");
   umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH); 
   if (creat("foo", S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|
                    S_IROTH|S_IWOTH) < 0)
       err_exit("creat error for foo");
   exit(0);  
}
