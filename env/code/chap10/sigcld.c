#include "../apue.h"
#include <sys/wait.h>

static void sig_cid(int);

int main() {
    pid_t pid;

    if (signal(SIGCHLD, sig_cid) == SIG_ERR) {
        perror("signal error");
    }

    if ((pid = fork()) < 0) {
        perror("fork error");
    } else if (pid == 0) {
        sleep(2);
        _exit(0);
    }

    pause();

    exit(0);
}

static void sig_cid(int signo) {
    pid_t pid;
    int status;

    printf("SIGCLD received\n");

    if (signal(SIGCHLD, sig_cid) == SIG_ERR) {
        perror("signal error");
    }

    if ((pid = wait(&status)) < 0) {
        perror("wait error");
    }

    printf("pid = %d\n", pid);
}
