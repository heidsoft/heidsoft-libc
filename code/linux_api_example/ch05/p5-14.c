#include "ch05.h"

int main(void)
{
    if(getlogin()==NULL)
       err_exit("getlogin error");
    printf("login_name is: %s, cuser_name is: %s\n", 
             getlogin(), cuserid(NULL));
    printf("real uid is: %d, effective uid is: %d\n", 
             getuid(), geteuid());
}
