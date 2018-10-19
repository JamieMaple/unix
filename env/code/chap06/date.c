#include <stdio.h>
#include <time.h>

int main() {
    time_t t;
    struct tm *tmp;
    char buf[64];

    time(&t);
    tmp = localtime(&t);
    strftime(buf, 64, "%a %b %d %T %Z %G", tmp);

    fprintf(stdout, "%s\n", buf);

    return 0;
}
