#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0) {
        // children
        printf("children die");
    } else {
        // parent
        sleep(30);
    }

    return 0;
}

