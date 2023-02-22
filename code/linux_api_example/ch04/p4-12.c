#include "ch04.h"
#include "get_perms.c"  /* 程式4-4 */
#define BLKSIZE 1024
void list(char *);
void printout(char *, char *);

int main(int argc, char *argv[])
{ 
   struct stat sbuf;
  
   if (argc < 2){  /* 若無參數,列出目前目錄 */
      list(".");
      exit(0);
   }

   /* 處理參數 */
   while(--argc){
     if (stat(*++argv,&sbuf) < 0) {  /* 檢視檔案型態 */
        perror(*argv);
        continue;
     }
     /* 若果是目錄，將它列出，否則列印出檔案的狀態訊息 */
     if ((sbuf.st_mode &S_IFMT) == S_IFDIR)
        list(*argv);
     else
        printout(".",*argv);
   }
   exit(0);
}

/* 讀一個目錄並列出其中的檔案 */
void list(char *name)
{
   DIR *dp;
   struct dirent *dir;
   if ((dp = opendir(name)) == NULL) { /*  開啟該目錄  */
      fprintf(stderr, "%s: cannot open.\n",name);
      return;
   }

   while((dir = readdir(dp)) != NULL)  /*  處理每一個目錄 */
      printout(name, dir->d_name);
   closedir(dp);
}

void printout(char *dir, char *name)  /* 列印出檔案的有關訊息 */
{
   int i,j;
   char perms[10];
   struct stat sbuf;
   char newname[BLKSIZE];

   sprintf(newname,"%s/%s",dir,name);   /* 形成路徑名 */
   stat(newname,&sbuf);/* 此時可以肯定該檔案存在，故不做失敗檢驗 */
   /* 列印檔案的區塊大小，每塊為BLKSIZE位元組 */
   printf("%5d",(sbuf.st_size+BLKSIZE-1)/BLKSIZE);
   /* 檢驗並列印檔案型態 */
   switch(sbuf.st_mode & S_IFMT){
     case S_IFREG:  putchar('-'); break;
     case S_IFDIR:  putchar('d'); break;
     case S_IFCHR:  putchar('c'); break;
     case S_IFBLK:  putchar('b'); break;
     case S_IFIFO:  putchar('l'); break;
#ifdef S_IFLNK
     case S_IFLNK:  putchar('l'); break;
#endif
#ifdef S_IFSOCK
     case S_IFSOCK: putchar('l'); break;
#endif
     default:       putchar('?'); break;
   }
  
   get_perms(&sbuf, perms);  /* 取得三群組存取權限（見程式4-4） */
   /* 列印存取權限、連結數、使用者ID和群組ID */
   printf("%s%3d %5d/%-5d ", perms, sbuf.st_nlink, sbuf.st_uid,
                   sbuf.st_gid);
   /* 列印檔案大小和修改時間。ctime轉換一個時間為ASCII字元 */
   printf("%7d %.20s ",sbuf.st_size, ctime(&sbuf.st_mtime));
   printf("%s\n", name); /* 最後，列印檔名 */
}
