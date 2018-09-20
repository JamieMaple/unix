#include "../apue.h"

int main() {
    // 提高移植性，所有最好用 long
    printf("hello world from process ID %ld\n", (long)getpid());
    exit(0);
}
