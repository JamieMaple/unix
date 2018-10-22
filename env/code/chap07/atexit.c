#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void testFunc1() {
    printf("test func 1\n");
}

void testFunc2() {
    printf("test func 2\n");
}

int main() {
    atexit(testFunc1);
    atexit(testFunc2);
    sleep(2);
    for (int i = 0; i < 10; i++) {
        printf("%d line\n", i);
    }
    return 0;
}
