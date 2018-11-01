#include "../apue.h"

static void sig_int(int signo) {
    printf("system interrupted key!\n");
}

int main() {
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("error signal(SIGINT)");
    }
    sleep(10);
}
