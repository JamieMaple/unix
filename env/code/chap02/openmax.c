#include "../apue.h"
#include <limits.h>
#include <errno.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

#define OPEN_MAX_GUESS 256

long open_max(void) {
    struct rlimit l;
    if (openmax == 0) {
        errno  = 0;
    }
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 || openmax == LONG_MAX) {
        /*
         *if (errno == 0) {
         *    openmax = OPEN_MAX_GUESS;
         *} else {
         *    err_sys("sysconf error for _SC_OPEN_MAX");
         *}
         */
        if (getrlimit(RLIMIT_NOFILE, &l) < 0) {
            err_sys("cannot get file limit");
        }

        if (l.rlim_max == RLIM_INFINITY) {
            openmax = OPEN_MAX_GUESS;
        } else {
            openmax = l.rlim_max;
        }
    }

    return openmax;
}
