#include <stdio.h>

int main() {
    printf("hello world 1\n");

    fprintf(stdout, "hello world 2\n");

    dprintf(1, "hello world 3\n");

    return 0;
}
