#include "../apue.h"

static void echoInfo(siginfo_t*);

int main() {
    siginfo_t info;
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        exit(7);
    }

    if (waitid(P_PID, pid, &info, WEXITED) == -1) {
        err_sys("wait error");
    }
    echoInfo(&info);

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        abort();
    }

    if (waitid(P_PID, pid, &info, WEXITED) == -1) {
        err_sys("wait error");
    }
    echoInfo(&info);

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        int test = 1;
        test /= 0;
    }

    if (waitid(P_PID, pid, &info, WEXITED) == -1) {
        err_sys("wait error");
    }
    echoInfo(&info);

    exit(0);
}

static void echoInfo(siginfo_t *info) {
    int code = info->si_code;
    int status = info->si_status;

    if (CLD_EXITED == code) {
        printf("normal termination, exit status = %d\n", status);
    } else if (CLD_STOPPED == code) {
        printf("child stoped, signal number = %d\n", status);
    } else {
        printf("abnormal termiantion, signal number = %d %s\n",
            status, CLD_DUMPED == code ? "(core file generated)" : "");
    }
}

