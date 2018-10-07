#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int fd = open("./data", O_RDONLY);

    printf("%d %d\n", fd, dup(fd));

    close(fd);

    _exit(0);
    
}
