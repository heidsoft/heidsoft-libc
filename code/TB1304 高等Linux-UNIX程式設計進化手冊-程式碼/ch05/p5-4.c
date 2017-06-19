#include "ch05.h"
static void func_1(void)
{
    printf("invoked first exit handler func_1\n");
}

static void func_2(void)
{
    printf("invoked second exit handler func_2\n");
}

int main(void)
{
    if ( atexit(func_1) != 0 ){ 
        printf("ERROR: atexit() register func_1 failed\n");
        exit(EXIT_FAILURE);
    }
    if ( atexit(func_2) != 0 ){ 
        printf ("ERROR: atexit() register func_2 failed\n");
        exit(EXIT_FAILURE);
    }
    if ( atexit(func_2) != 0 ){ 
        printf("ERROR: atexit() register func_2 failed\n");
        exit(EXIT_FAILURE);
    }
    printf("main: exit normally\n");
}
