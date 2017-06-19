#include “ch04.h”
int main(void)
{
    int         link_value;
    const char  *path1 = “testfile1”;
    const char  *path2 = “testfile2”;
    struct stat orig_buf, new_buf;
    printf (“create testfile\n”);
    creat (path1, S_IRWXU|S_IRWXG|S_IRWXO);
    stat (path1, &orig_buf);    /* 取得檔案狀態 */
    printf (“testfile1 status: orig_buf.st_nlink=%d\n”, orig_buf.st_nlink );
    printf (“create link from %s to %s\n”,path1,path2);
    if (link (path1, path2))    /* 建立新的連線 */
       err_exit (“link() call failed”);
    printf (“link() call successful\n”);
    stat (path1, &new_buf);   /* 再次取檔案狀態檢視新的連線數 */
    printf (“new_buf.st_nlink=%d\n”, new_buf.st_nlink );
	return (EXIT_SUCCESS);
}

