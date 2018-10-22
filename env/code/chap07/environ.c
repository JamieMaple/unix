#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
    
    while (*envp != NULL) {
        printf("%s\n", *envp);
        envp++;
    }

    exit(0);
}
