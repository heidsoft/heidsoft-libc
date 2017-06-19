#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
        char **ptr;
        for (ptr = envp; *ptr != 0; ptr++)   /* and all env strings */
                printf("%s\n", *ptr);
        printf("USERNAME is %s\n", getenv("USERNAME"));
        putenv("USERNAME=yangzhu");
        printf("USERNAME is %s\n", getenv("USERNAME"));
        setenv("USERNAME", "yangzhu2", 0);
        printf("USERNAME is %s\n", getenv("USERNAME"));
        setenv("USERNAME", "yangzhu2", 1);
        printf("USERNAME is %s\n", getenv("USERNAME"));
 
	return 0;
}
