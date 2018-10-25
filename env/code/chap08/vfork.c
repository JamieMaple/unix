#include "../apue.h"

int globval = 6;

int main() {
    int var = 88;
    pid_t pid;

    printf("before fork\n");

    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {
        // child
        globval++;
        var++;
        _exit(0);
    }

    printf("pid = %ld, glob = %d, val = %d\n", (long)getpid(), globval, var);

    exit(0);
}
