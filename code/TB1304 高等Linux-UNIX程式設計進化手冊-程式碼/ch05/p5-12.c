#include "ch05.h"

int main(void)
{
   char *me, **members;
   struct passwd  *my_passwd;
   struct group   *my_group;

   me = getlogin();             // 獲獲得使用者ID （§6.11）
   my_passwd = getpwnam(me);    // 獲得使用者訊息 */
   if (!my_passwd) 
      err_exit("getpwuid failed");
   
   /* 列印使用者訊息 */
   printf("I am %s\n", my_passwd->pw_gecos);
   printf("My login name is %s\n", my_passwd->pw_name);
   printf("My uid is %d\n", (int) (my_passwd->pw_uid));
   printf("My home directory is %s\n", my_passwd->pw_dir);
   printf("My default shell is %s\n", my_passwd->pw_shell);

   /* 獲得預設群組ID訊息 */
   my_group = getgrgid(my_passwd->pw_gid);
   if (!my_group) 
      err_exit("getgrgid failed");

   /* 列印群組訊息 */
   printf("My default group is %s (%d).\n",
           my_group->gr_name, (int) (my_passwd->pw_gid));
   members = my_group->gr_mem;
   if (*members)
     printf("The members of this group are:\n");
   else 
      printf("no other members in the group\n");
   while(*members){
      printf("  %s\n", *(members));
      members++;
   }
   return EXIT_SUCCESS;
}
