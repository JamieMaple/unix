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



### 二进制 I/O



### 定位流



### 格式化 I/O



### 实现细节

