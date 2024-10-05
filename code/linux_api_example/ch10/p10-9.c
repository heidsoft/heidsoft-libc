#include "ch10.h"
#define NRECORDS (100)

typedef struct {
    int integer;
	char string[16];
} RECORD;

int main(void)
{
    RECORD record, *mapped;
    int i,fd;
    FILE *fp;
    fp = fopen("records.dat","w+");
    if (fp==NULL)
       err_exit("fopen:");
    /* 寫記錄至檔案 */
    for (i=0;i<NRECORDS;i++){
       record.integer = i;
       sprintf(record.string, "RECORD-%4d", i);
       fwrite(&record,sizeof(record),1,fp);
    }
    fclose(fp);
    /* 重新開啟此檔案用儲存映射方法修改第43個記錄 */
    fd = open("records.dat",O_RDWR);
    /* 將檔案的前NRECORDS個記錄映射到記憶體 */
    mapped = (RECORD *)mmap(0, NRECORDS*sizeof(record),
	              PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    mapped[43].integer = 243;     /* 修改第43個記錄的記錄號 */
    sprintf(mapped[43].string, "RECORD-%d",mapped[43].integer);
    msync((void*)mapped, NRECORDS*sizeof(record), MS_ASYNC); /* 同步磁碟 */
    munmap((void*)mapped, NRECORDS*sizeof(record));	/* 移除儲存映射 */
	/*檢視檔案有否改變 */ 
    lseek(fd, 43*sizeof(record), SEEK_SET);
    read(fd, &record, sizeof(record));
    printf("record[43].integer = %d\n",record.integer);
    close(fd);
    exit(0);
}
