#include <syslog.h>

int
main() {
    openlog("HOGE", LOG_CONS, LOG_USER);
    syslog(LOG_USER | LOG_NOTICE, "hoge %d", __LINE__);
    closelog();
    return 1;
}
