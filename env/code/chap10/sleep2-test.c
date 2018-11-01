#include "../apue.h"
#include "./sleep2.c"

static void sig_int(int);

int main() {
    unsigned int unslept;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("signal(SIGINT) error");
    }
    unslept = sleep2(5);

    printf("sleep2 returned: %u\n", unslept);

    exit(0);
}

static void sig_int(int signo) {
    volatile int k = 0;

    printf("\nsig_int starting\n");
    for (int i = 0; i < 300000; i++) {
        for (int j = 0; j < 4000; j++ ) {
            k += i * j;
        }
    }
    printf("sig_int finished\n");
}
