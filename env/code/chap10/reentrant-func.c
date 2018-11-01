#include "../apue.h"
#include <pwd.h>

static void my_alarm(int signo) {
    struct passwd *rootprt;

    printf("in signal handler");

    if ((rootprt = getpwnam("root")) == NULL) {
        err_sys("getpwnam(root) error");
    }
    alarm(1);
}

int main() {
    struct passwd *ptr;

    signal(SIGALRM, my_alarm);

    alarm(1);

    for ( ; ; ) {
        if ((ptr = getpwnam("maple")) == NULL) {
            err_sys("getpwnam error");
        }
        if (strcmp(ptr->pw_name, "maple") != 0) {
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
        }
    }

    exit(0);
}
