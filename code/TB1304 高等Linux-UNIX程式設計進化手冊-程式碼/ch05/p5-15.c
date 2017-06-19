#include "ch05.h"
#define SCORES_FILE "/tmp/score"
static uid_t euid, ruid;  /* 儲存有效和實際UIDS*/
void do_setuid(void)      /* 還原原來的有效UID */
{
   int status;
#ifdef _POSIX_SAVED_IDS
   status = setuid(euid);
#else
   status = setreuid(ruid, euid);
#endif
   if (status < 0) 
      err_exit("Couldn't set uid");
}

void undo_setuid(void) /* 設定有效UID為實際UID */
{   int status;
#ifdef _POSIX_SAVED_IDS
   status = setuid(ruid);
#else
   status = setreuid(euid, ruid);
#endif
   if (status < 0) 
      err_exit("Couldn't set uid");
}

int record_score(int score) /* 記錄得分 */
{
   FILE *stream;
   char *myname;
   do_setuid ();     /* 讓游戲者有讀寫得分檔案的權利 */
   stream = fopen (SCORES_FILE, "a");   /* 開啟得分檔案 */
   undo_setuid();    /* 收回游戲者讀寫得分檔案的權利,不影響後面的寫入操作 */
   /* 寫得分至記分檔案 */
   if (stream) {
      myname = cuserid((char *)NULL);  /* 游戲執行者的使用者名稱 */
      if (score < 0)
         fprintf (stream, "%10s: Couldn't lift the caber.\n",myname);
      else
         fprintf (stream, "%10s: %d feet.\n", myname, score);
      fclose (stream);
      return 0;
   }
   else
      return -1;
}
int main(void)
{
   /* 儲存實際和有效使用者IDs */
   ruid = getuid();
   euid = geteuid();
   undo_setuid();             /* 讓游戲者只有他自己的權利 */
   /* 進行游戲並記錄得分 */
   /* ... */
}
