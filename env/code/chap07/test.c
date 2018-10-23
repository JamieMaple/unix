#include <stdio.h>

static int globalArgc;
static char **globalArgv;

void f1();

typedef void *ExitFunc(void);

void atexit(ExitFunc *func);


int main(int argc, char *argv[]) {
    globalArgc = argc;
    globalArgv = argv;
    f1();
}

void f1() {
    for (int i = 0; *globalArgv != NULL; i++)
        printf("%d %s length%d\n", i, globalArgv[i], globalArgc);
}
