#include "../apue.h"

int main() {
    int c;

    while ((c = fgetc(stdin)) != EOF) {
        if (fputc(c, stdout) == EOF) {
            err_sys("ouput error");
        }
    }

    if (ferror(stdin)) {
        err_sys("input error");
    }

    exit(0);
}
