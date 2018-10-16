#include "../apue.h"
#ifdef SOLARIS
#include <sys/mkdev.h>
#endif


int main(int argc, char *argv[]) {
    struct stat buf;
    for (int i = 0; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (stat(argv[i], &buf) < 0) {
            err_ret("stat error");
            continue;
        }
    }
    exit(0);
}
