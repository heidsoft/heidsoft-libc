#include <pwd.h>
#include <string.h>
struct passwd *getpwnam (const char *name)
{
   struct passwd *ptr;
   setpwent();                                  /* 開啟使用者資料庫 */
   while((ptr = getpwent()) != NULL) {
      if (strcmp(name, ptr->pw_name) == 0)
         break;                                  /* 找到比對項 */
   }
   endpwent();                                  /* 關閉使用者資料庫 */
   return(ptr);                                 /* 找不到比對時ptr為NULL */
}
