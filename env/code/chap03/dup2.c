#include <stdio.h>
#include <fcntl.h>
#include "../apue.h"

int mydup2(int fd, int fd2) {
    int temp;
    // 失败创建
    if ((temp = dup(fd)) == -1 ) {
        return -1;
    } else {
        close(temp);
    }

    // 如果两者相同，返回 fd2，不必关闭他
    if (fd == fd2) {
        return fd2;
    }

    // 否则关闭 fd2
    close(fd2);

    int tempfd;

    for (int i = 0; i <= fd2; i++) {
        if ((tempfd = dup(fd)) == -1) {
            /*err_sys("dup error");*/
            return -1;
        } else {
            if (tempfd == fd2) {
                break;
            }
        }
    }

    return tempfd;
}

int main(int argc, char *argv[]) {
    int fd = open("./data", O_RDWR | O_CREAT | O_APPEND);
    int fd2 = 6;

    if ((fd2 = mydup2(fd, fd2)) == -1) {
        err_quit("mydup2 error");
    }

    char *s = "this is a test of dup and dup2\n";
    if (write(fd2, s, strlen(s)) == -1) {
        err_sys("write error");
    }

    close(fd);

    exit(0);
}
