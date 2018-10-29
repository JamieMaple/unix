#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        printf("error fork\n");
    } else if (pid == 0) {
        exit(0);
    }

    system("ps -ao pid,ppid,state,tty,command");
    sleep(5);

    exit(0);
}
