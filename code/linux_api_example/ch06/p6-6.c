#include "ch06.h"

int main(void)
{
   pid_t pid;
   char  *message;
   int   n;

   printf("fork program start\n");
   pid = fork();
   switch(pid){
   case -1:         
      exit(EXIT_FAILURE);
   case 0:        
      message = "This is child";
      n = 1;
      break;
   default:
      message = "This is parent";
      n = 5;
      break;
   }
   puts(message);
   sleep(n-1);
   exit(EXIT_SUCCESS);
}
