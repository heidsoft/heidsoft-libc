#include "ch01.h"

char *get_current_dir()
{
    char *buffer;
    char *value;
    int size = 0;
    /*確定目前工作目錄路徑名的最大長度於size,當PATH_MAX 不確定時，size為-1*/
#ifdef PATH_MAX
    size = PATH_MAX;
#else
    errno = 0;
    if ((size = pathconf("./",_PC_PATH_MAX)) < 0)
    if (errno != 0){
        printf("pathconf error for _PC_PATH_MAX\n");
        exit(-1);
    }
#endif
    if (size > 0){ /* PATH_MAX有定義,可以確保分配的空間足以存放路徑名*/
        buffer = (char *)malloc(size+1);
        value = getcwd(buffer, size);
    }else{  /* PATH_MAX沒有定義,必須試探性地分配足夠的空間來存放路徑名*/
        size = _POSIX_PATH_MAX;
        buffer = (char *)malloc(size);
        while (1) {
            value = getcwd(buffer, size);
            if (value == 0 && errno ==ERANGE){ /* buffer太小，重新申請更大的空間 */
               size *= 2;
               free (buffer);
               buffer = (char *) malloc (size);
            }
        } 
    }
    return buffer;
}
