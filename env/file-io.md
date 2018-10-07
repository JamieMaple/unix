# 文件 IO

> chap 03
> 不带缓冲的 IO
> 不是 ISO C 的组成部分
> 但是是 PoSIX.1 | Single UNIX Spec 组成部分

### 文件描述符

1. 对于内核而言，所有打开的文件都通过文件描述符引用

1. 文件描述符号是一个非负整数

1. 当打开或创建一个文件时，内核向进程返回一个文件描述符

| fd | 代表     | 符号常量        |
|----|:--------:|:---------------:|
| 1  | 标准输出 | `STDOUT_FILENO` |
| 2  | 标准错误 | `STDERR_FILENO` |

PS: 幻数的原文是 `magic number`，尽可能使用语义化的常量表示一些数字

### `open` 和 `openat`

``` c
#include <fcntl.h>

int open(const char *path, int oflag /* mode_t mode */);

int openat(int fd, const char *path, int oflag, /* mode_t mode */);

```

原子操作：`atomic operation` 不能分割的最小粒度操作

`openat` 是新增的函数，希望解决两个问题：

  1. 线程通过相对路径打开文件

  2. 避免 `time-of-check-to-time-of-use` 问题 (TOCTTOU)

### `creat`

``` c
#include <fcntl.h>

int creat(const char* path, mode_t mode);

// 等价于
open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);

```

缺点是以只写方式打开创建的文件

两种方式解决问题：

  1. 先 `creat` 后 `close` 再调用 `open`

  2. `open(path, O_RDWR | O_CREAT | O_TRUNC, mode)`

### `close`

``` c
#include <unistd.h>

int close(int fd);
```

1. 关闭一个文件会释放该进程在该文件上的所有记录锁

2. 进程终止时，内核会自动关闭所有打开的文件

(或许标准输入输出错误就是被内核自动关闭的 🤔 ？)

### `lseek`

> 没打开一个文件都有一个与它相关联的 `current file offest` 偏移量
> 通常是一个非负整数

``` c
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);
```

通常情况下，偏移量不可能为负，有些设备支持非负

### `read`

``` c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t nbytes);
```

### `write`

``` c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t nbytes);
```

### IO 效率

预读

### 文件共享

进程表项 -> 文件描述表              -> v 节点表项
            |- 文件表项
            |- 文件表项
              |- 文件状态标志
              |- 当前文件偏移量
              |- v 节点指针

### 原子操作

一般而言，原子操作（atomic operation） 指的是由多步组成的一个操作

``` c
#include <unistd.h>

O_APPEND

ssize_t pread(int fd, void *buf, size_t nbytes, off_t offset);

ssize_t pwrite(int fd, const void *buf, size_t nbytes, off_t offset);
```

### `dup` 和 `dup2`

用以复制现有的文件描述符

``` c
#include <unistd.h>

int dup(int fd);
int dup2(int fd, int fd2);
```

### `sync` 和 `fsync` 以及 `fdatasync`

``` c
#include <unistd.h>

int fsync(int fd); // 等待写磁盘操作结束才返回

int fdatasync(int fd); // 

void sync(void);

// sync 将所有的块缓冲区排入写队列，不等接待实际写磁盘操作结束
// update 系统守护进程定期调用 sync，定期 flush 内核的块缓冲区
```

### `fcntl`

可以用来改变已经打开文件的属性

``` c
#include <fcntl.h>

int fcntl(int fd, int cmd, ... /* int arg */);
```

### `ioctl`

io 操作杂物箱

``` c
#include <unistd.h>
#include <sys/ioctl.h>

int ioctl(int fd, int request, ...);
```

### /dev/fd

打开文件描述符 `/dev/fd/{n}` 等效于复制

一般常用于 shell 使用场景

### 总结

`read` 和 `write` 都是在内核中进行的，所以这些函数不带缓冲的 IO 操作

原子操作

