# 文件和目录

### stat fstat fstatat 和 lstat

返回命名文件有关的信息结构

``` c
#include <sys/stat.h>

int stat(const char *restrict path, struct stat *restrict buf);

int fstat(int fd, struct stat *buf);

int lstat(const char *restrict pathname, struct stat *restrict buf);

int fstatat(int fd, const char *restrict pathname, struct stat *restrict buf, int flag);

```

### 文件类型

(1) 普通文件

(2) 目录文件

(3) 块特殊文件

(4) 字符特殊文件

(5) FIFO(命名管道)

(6) 套接字(用于进程间的网络通信)

(7) 符号链接

### access faccessat

测试文件的访问权限

``` c
#include <unistd.h>

int access(const char *pathname, int mode);

int faccessat(int fd, const char *pathname, int mode, int flag);

```

### umask

``` c
#include <sys/stat.h>

mode_t umask(mode_t cmask);
```

### chmod、fchmod 和 fchmodat

``` c
#include <sys/stat.h>

int chmod(const char *pathname, mode_t mode); // 在制定的文件上操作

int fchmod(int fd, mode_t mode);  // 对已打开的文件操作

int fchmodat(int fd, const char *pathname, mode_t mode, int flag); // 相对路径

```

### 

