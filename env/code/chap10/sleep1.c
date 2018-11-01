#include <signal.h>
#include <unistd.h>

static void sig_alrm(int signo) {
    /*do something*/
}

unsigned int sleep1(int seconds) {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        return seconds;
    }

    // mabe some race condition
    alarm(seconds);
    pause();

    return 0;
}
