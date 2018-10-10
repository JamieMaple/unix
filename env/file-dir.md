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

改变文件的访问权限等

``` c
#include <sys/stat.h>

int chmod(const char *pathname, mode_t mode); // 在制定的文件上操作

int fchmod(int fd, mode_t mode);  // 对已打开的文件操作

int fchmodat(int fd, const char *pathname, mode_t mode, int flag); // 相对路径

```

在以下两种情况下会自动清除一部分权限位

1. Solaris、Free BSD 等普通文件的粘着位有特殊含义，所以在这些系统上只有超级用户才能设置粘着位

2. 

### 粘着位

`S_ISVTX`

### `chown` `fchown` `fchownat` `lchown`

``` c
#include <unistd.h>

int chown(const char *pathname, uid_t owner, gid_r group);

int fchown(int fd, uid_t owner, gid_t group);

int fchownat(int fd, const char *pathname, uid_t owner, gid_t group, int flag);

int lchown(const char *pathname, uid_t owner, gid_t group)

```

### 文件长度


1. `stat.st_size`

1. `stat.st_blksize`

1. `stat.st_blocks`

1. 文件空洞

  空洞是由所设置的偏移量超过文件末端所导致的

### 文件截断

用以在文件末端截断一些数据来缩小文件。

``` c
#include <unistd.h>

int trancate(const char *pathname, off_t length);

int ftruncate(const char *pathname, off_t length);
```

### 文件系统

i 节点就是常说的 inode，是 unix 文件系统中一个非常重要的概念

| 分区 | 分区 | ... | (磁盘)
-- | 自举块 |  超级块 | 柱面组 1 - n | (文件系统)
---- | 超级块副本 | 配置信息 | i节点图 | 块位图 | i 节点 | 数据块 | (柱面组 n)
------ | i节点 | i节点 | ... |

1. i节点链接计数和硬链接

1. 符号链接

1. `ln` 不能跨文件系统

1. 重命名的细节

### `link` `linkat` `unlink` `unlinkat` `remove`

创建指向现有文件的链接

``` c
#include <unistd.h>

int link(const char *existingpath, const char *newpath);

int linkat(int efd, const char *existingpath, int nfd, const char *newpath, int flag);

```

删除现有的目录项

``` c
#include <unistd.h>

int unlink(const char *pathname);
int unlinkat(int fd, const char *pathname, int flag);
```

`remove` 类似 `unlink` 以及 `rmdir`

``` c
#include <stdio.h>

int remove(const char *pathname);
```

### `rename` `renameat`

``` c
#include <stdio.h>

int rename(const char *pathname, const char *newname);

int renameat(int oldfd, const char *oldname, int newfd, const char *newname);

```

### 符号链接（软链接）

为了避开硬链接的一些限制

1. 硬链接直接指向了文件的 i节点

1. 硬链接通常要求链接




