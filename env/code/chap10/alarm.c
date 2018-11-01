#include "../apue.h"

static void handler(int);

int main() {
    /*signal(SIGALRM, handler);*/
    alarm(2);
    
    getchar();

    exit(0);
}

static void handler(int signo) {
    printf("time out!\n");
}
