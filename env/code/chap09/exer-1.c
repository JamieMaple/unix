#include "../apue.h"
#include <fcntl.h>

static void pr_session(const char *name) {
    pid_t pid = getpid();
    printf("%s: pid %ld, sid %ld, pgid %ld, ", name, (long)pid, (long)getsid(pid), (long)getpgid(pid));

    int fd = open("/dev/tty", O_RDWR);

    if (fd < 0) {
        printf("has no controlling terminal\n");
    } else {
        printf("has controlling terminal\n");
    }
}

int main() {
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        // child
        setsid();
        pr_session("child");
    } else {
        // parent
        pr_session("parent");
    }

    exit(0);
}
