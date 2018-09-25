#include <stdio.h>
#include <sys/resource.h>
#include "openmax.c"

int main() {
    struct rlimit t;
    printf("getrlimit %d\n", getrlimit(RLIMIT_NOFILE, &t));
    printf("long max %ld\n", LONG_MAX);
    printf("rlim_max %lld\n", t.rlim_max);
    printf("RLIM_INFINITY %lld\n", RLIM_INFINITY);
    printf("open_max %ld\n",  open_max());
}
