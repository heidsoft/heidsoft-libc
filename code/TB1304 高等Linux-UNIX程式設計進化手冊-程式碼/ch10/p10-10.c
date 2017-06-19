#include "ch10.h"
#define FILE_MODE 0666
int main(int argc, char *argv[] )
{
    int fdfrom , fdto;
    char *src, *dst;
    struct stat statbuf1,statbuf2;
    if (argc != 3){
        printf( "usage: a.out <fromfile> <tofile>\n" );
     	exit(1);
    }
	/* 開啟要連線的兩個檔案 */
    if ((fdfrom = open(argv[1], O_RDONLY)) < 0){
        printf("can't open %s for reading", argv[1]);
     	exit(1);
	}
    if ((fdto = open(argv[2], O_RDWR|O_CREAT, FILE_MODE)) < 0){ 
        printf("can't creat %s for writing", argv[2]); 
     	exit(1);
	}
    /* 取得這兩個檔案的大小*/
    if (fstat(fdfrom, &statbuf1) < 0)
        err_exit("fstat errort") ;
    if (fstat(fdto, &statbuf2) < 0)
        err_exit ("fstat errort") ;
    /* 擴充輸出檔案的大小使之等於這兩個檔案大小之和 */
    if (lseek(fdto, statbuf1.st_size - 1, SEEK_END) == -1)
       err_exit("lseek error");
    /* 在檔案尾部寫一字元使擴充有效 */  
    if (write(fdto, "", 1) != 1) 
       err_exit("write error") ;
    /* 映射這兩個檔案至記憶體 */
    if ((src = mmap(0, statbuf1.st_size, PROT_READ,
           MAP_SHARED, fdfrom, 0)) == (caddr_t)-1)
       err_exit("mmap error for input") ;
    if ((dst = mmap(0, statbuf2.st_size + statbuf1.st_size, 
	       PROT_READ | PROT_WRITE, MAP_SHARED, fdto, 0)) == (caddr_t)-1)
       err_exit("mmap error for output");
    /* 連線第一個檔案至第二個檔案 */
    memcpy( dst+statbuf2.st_size, src, statbuf1.st_size);
	exit(0);
}
