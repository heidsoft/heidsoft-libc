#include "ch05.h"
static void setup_core_dumping (void)
{
   struct rlimit rlim;
   if (getrlimit(RLIMIT_CORE, &rlim) != 0)   /* 取得限制值  */
      err_exit("getting core file size maximum limit ");
   rlim.rlim_cur = rlim.rlim_max;              /* 用硬限制置換軟限制  */
   if (setrlimit(RLIMIT_CORE, &rlim) != 0)   /* 重新設定限制值  */
      err_exit("setting core file size limit to maximum ");
}
