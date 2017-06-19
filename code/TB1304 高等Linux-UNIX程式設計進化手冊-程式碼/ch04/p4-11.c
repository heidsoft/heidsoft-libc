#include "ch04.h"
#include "y_or_n_ques.c"    /* 程式2-1 */
int main(int argc, char*argv[])
{
    if(argc != 2){
       printf("Usage: a.out <pathname>\n");
       exit(EXIT_FAILURE);
    }

    /* 建立新目錄 */
    if( mkdir(argv[1], S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH) < 0)
       err_exit("mkdir failed");
    printf("you made a directory %s\n",argv[1]);

    /* 移除此新目錄*/
     if (!y_or_n_ques("Do you really need the directory?")) {     
       if (rmdir(argv[1])<0)
          err_exit("rmdir failed");
       printf("Directory %s is removed\n",argv[1]);
     } else
       printf("Directory %s is kept.\n", argv[1]); 
}
