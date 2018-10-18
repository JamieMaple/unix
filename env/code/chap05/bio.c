#include "../apue.h"

int main() {
    float data[10] = { 1.0, 2.2, 3.444 };

    if (fwrite(&data[2], sizeof(float), 4, stdout) != 4) {
        err_sys("error in fwrite");
    }

    exit(0);
}
