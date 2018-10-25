#include "../apue.h"

char *env_init[] = { "USER=unknow", "PATH=/tmp" };

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        // child
        if (execle("/Users/maple/Desktop/Learning/unix/env/code/chap08/echoall", "echoall", "myarg1", "My arg2", (char *)0, env_init) < 0) {
            err_sys("execle error");
        }
    }

    if (waitpid(pid, NULL, 0) < 0) {
        err_sys("wait error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    }  else if (pid == 0) {
        // child
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0) {
            err_sys("execlp error");
        }
    }

    exit(0);
}
