#include <stdio.h>
#include <sys/utsname.h>

int main() {
    struct utsname un;

    if (uname(&un) < 0) {
        printf("uname failed\n");
    } else {
        printf("%s %s %s %s %s\n", un.sysname, un.nodename, un.release, un.version, un.machine);
    }

    return 0;
}

