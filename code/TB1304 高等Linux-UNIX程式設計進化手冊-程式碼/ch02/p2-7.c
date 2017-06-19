#include <stdio.h>
int get_line(char *buf, int bufsize, FILE *fd)
{
   if( fgets(buf, bufsize, fd) == EOF){
     if (feof(fd)){
        printf("We meet end of file\n");
        return EOF;
     }else {
        perror("fgets failed");
        return 0;
     }
   }
   printf("call fgets: %s",buf);
   return 1;
}
