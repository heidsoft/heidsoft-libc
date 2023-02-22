#include <sys/types.h>
#include "err_exit.h"
void * xmalloc(size_t size)
{
    register void  * value = malloc(size);
    if (value == 0)
        err_exit ("virtual memory exhausted");
    return value;
}
