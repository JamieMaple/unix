# 标准 I/O 库

### 流和 FILE 对象

当标准 I/O 打开或创建一个文件时，我们已经使一个流和一个文件相关联

标准 IO 流可以用于单字节或多字节（"宽"）字符集。

流定向（stream orientation）决定了所读、写的字符单字节还是多字节的。

设置流的定向

``` c
#include <stdio.h>
#include <wchar.h>

int fwidth(FILE *fp, int mode);
```

1. 如 mode 为负数，面向字节

1. 如 mode 为正数，面向宽定向

1. 如 mode 为零值，不设置流定向，但返回标识该流定向的值

### stdin stdout stderr

预定义了三个流，这三个文件指针（*FILE）定义在了 `stdio.h` 中

### 缓冲

目的猜测应该是解决 CPU 和内存之间速度不一致的问题

1. 全缓冲

在填满标准 IO 缓冲区后才进行实际的 IO 操作

`flush` 指的是标准 IO 缓冲区的写操作(当然有其他意思)

1. 行缓冲

当写入一行才进行实际的 IO 操作

存在着两个限制

  1. 每一行的长度是固定的，填满了就进行 IO 操作

  1. 任何时候通过标准 IO 库从一个不带缓冲或者行缓冲的流中得到输入数据，那么都会冲洗所有行缓冲的流

1. 不带缓冲

不对字符进行缓冲存储

`stderr` 就是不带缓冲的

更改流缓冲类型:

``` c
#include <stdio.h>

void setbuf(FILE *restrict fp, char *restrict buf);

int setvbuf(FILE *restrict fp, char *restrict buf, int mod, size_t size);
```

强制冲洗一个流:

``` c
#include <stdio.h>

int fflush(FILE *fp);
```

如若传入 NULL，则所有输出流被冲洗

### 打开流

打开标准 I/O 流

``` c
#include <stdio.h>

FILE *fopen(const char *restrict pathname, const char *restrict type);

FILE *freopen(const char *restrict pathname, const char *restrict type, FILE *restrict fp);

FILE *fdopen(int fd, const char *type); // 通常用于创建管道和网络通信通道函数返回的文件描述符
```

`type` 制定文件

| type    | 说明                      |
|:-------:|:-------------------------:|
| r 或 rb | 为读而打开                |
| w 或 wb | 把文件截断至0，或为写打开 |
| ....    | ....                      |


当以读和写类型打开一个文件时，限制：

1. 如果中间没有 `fflush` `fseek` `fsetpos` `rewind`，则在输出当后面不能直接跟随输入

1. 如果没有 `fseek` `fsetpos` `rewind`，或者一个输入操作没有到达文件尾端，则在输入操作时候不能直接跟随输出

关闭一个流:

``` c
#include <stdio.h>

int fclose(FILE *fp);
```

关闭一个流前，应当冲洗缓冲区中的输出数据。否则会丢弃任何输入数据或释放该缓冲区。

当一个进程正常终止时，所有未带写缓冲的标准 I/O 流都被冲洗，所有打开的标准 I/O 流都被关闭

### 读和写流

打开流之后可以选择3种不同类型的流：

1. 每次打开一个字符的 IO

``` c
#include <stdio.h>

int getc(FILE *fp);

int fgetc(FILE *fp);

int getchar(void);
```

其中有几个区别，

   1. `getc` 可以实现为宏操作
   1. `getc` 参数不应当具有副作用的表达式
   1. `fgetc` 一定是一个函数，所以可以获取其地址
   1. `fgetc` 执行时间可能长，毕竟是函数一般比宏慢

区分出错还是到达文件尾端

``` c
#include <stdio.h>

int ferror(FILE *fp);

int feof(FILE *fp);

void clearerr(FILE *fp);
```

大多数流在 FILE 对象中实现中维护了两个标志:

   - 出错标志
   - 文件结束标志

将字符押送回流中：

``` c
#include <stdio.h>

int ungetc(int, FILE *fp);
```

输出函数：

``` c
#include <stdio.h>

int putc(int c, FILE *fp);

int fputc(int c, FILE *fp);

int putchar(int c); // equals to putc(c, stdout)
```

2. 每次打开一行 IO

每次输入一行

``` c
#include <stdio.h>

char *fgets(char *restrict buf, int n, FILE *restrict fp);

char *gets(char *buf);
```

`gets` 不推荐使用，容易造成缓冲区溢出，曾被利用

输出行：

``` c
#include <stdio.h>

int fputs(const *restrict str, FILE *restrict fp);

int puts(const char *str);
```

尽量避免使用 `puts` 而应当更多的使用 `fputs` 和 `fgets`

3. 直接 IO

`fread` 和 `fwrite` 支持这种类型的 I/O。每次 I/O 操作读或写某种数量的对象，而每个对象具有制定的长度。
该两种函数常用于读从二进制文件中每次读或写一个结构。

### 标准 I/O 的效率

对于大多数应用而言，最主要的用户 CPU 时间是由应用本身的各种处理消耗的，而不是标准 I/O 例程消耗的。

### 二进制 I/O

``` c
#include <stdio.h>

size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);

size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
```

对于读而言，当出错或到达文件尾端，所以可能返回当少于 `nobj`
所以该情况最好调用 `ferror` 或 `feof` 以判断究竟是哪种情况。

对于写而言，当返回当少于所要求当 `nobj` 时出错

对于异构系统，很比较大的问题(通常是在网络通信中回遇到)

### 定位流

获取流或更改流的位置：

``` c
#include <stdio.h>

long ftell(FILE *fp);

int fseek(FILE *fp, long offset, int whence);

void rewind(FILE *fp);
```

有所不同的：

``` c
#include <stdio.h>

off_t ftello(FILE *fp);

int fseeko(FILE *fp, off_t offset, int whence);
```

ISO C 引入：

``` c
#include <stdio.h>

int fgetpos(FILE *restrict fp, fpos_t *restrict pos);

int fsetpos(FILE *fp, const fpos_t *pos);
```

### 格式化 I/O

格式化标准输出：

``` c
#include <stdio.h>

int printf(const char *restrict format, ...);

int fprintf(FILE *restrict fp, const char *restrict format, ...);

int dprintf(int fd, const char *restrict format, ...);

int sprintf(char *restrict buf, const char *restrict format, ...);

int snprintf(char *restrict buf, size_t n, const char *restrict format, ...);
```

有变种：

``` c
#include <stdio.h>
#include <stdarg.h>

int vprintf(const char *restrict format, va_list arg);

int vfprintf(FILE *restrict fp, const char *restrict format, va_list arg);

int vdprintf(int fd, const char *restrict format, va_list arg);

int vsprintf(char *restrict buf, const char *restrict format, va_list arg);

int vsnprintf(char *restrict buf, size_t n, const char *restrict format, va_list arg);
```

格式化输出：

``` c
#include <stdio.h>

int scanf(const char *restrict format, ...);

int fscanf(FILE *restrict fp, const char *restrict format, ...);

int sscanf(const char *restrict buf, const char *restrict format, ...);
```

### 实现细节

**为了了解所使用的系统标准 I/O 库的实现，最好从头文件<stdio.h>开始**。然后可以详细看源码的实现

### 临时文件

创建临时文件：
``` c
#include <stdio.h>

char *tmpnam(char *ptr);

FILE *tmpfile(void);
```

另外的创建临时文件和目录：

``` c
#include <stdio.h>

char *mkdtemp(char *template);

int mkstemp(char *template);
```

`template` 字符串后6位为 `XXXXXX`

`mkstemp` 创建的临时文件不会自动删除

另外，`tmpnam` 和 `mkdtemp` 返回唯一路径和使用该路径之间存在一个时间窗口

### 内存流

创建内存流：

``` c
#include <stdio.h>

FILE *fmemopen(void *restrict buf, size_t size, const char *restrict type);

FILE *open_memstream(char **bufp, size_t *sizep);

#include <wchar.h>

FILE *open_wmemstream(wchar_t **bufp, size_t *sizep);

```

后两者与第一个有很大区别

### 替代标准 I/O

标准 IO 库不足之处在于它效率不高，和它需要复制的数据量有很大关系

(当使用每次一行函数 `fgets` 和 `fputs`)
1. 内核与标准 IO 缓冲之间需要复制一次数据

1. 标准 IO 缓冲区和用户程序当行缓冲区之间又要复制

