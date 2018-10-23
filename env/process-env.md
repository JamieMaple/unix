# 进程环境

### main 函数

主函数原型：

``` c
int main(int argc, char *argv[]);
```

`argc` 指的是命令行参数个数

`argv` 指的是指向参数的各个指针组成的数组

注意 main 内核调用它的顺序

### 进程终止

8 种正常终止的方式

1. 从 `main` 返回

2. 调用 `exit`

3. 调用 `_exit` 或 `_Exit`

4. 最后一个线程从其启动例程返回

5. 从最后一个线程调用 `pthread_exit`

6. 调用 `abort`

7. 接到一个信号

8. 最后一个线程对取消请求做出响应

三个 `exit` 函数

`atexit` 终止处理函数

一个 C 程序如何启动和终止

``` c

|   k  |<----- Exit|_exit <----| <-------|
|   e  | -> C 启动例程 -> main 函数 -> 用户函数
|   r  |                        exit <---|
|   n  |                          | <--> 终止处理程序
|   e  |                          | <--> 标准 I/O 清理程序
|   l  |<-------------------------|

```

### 命令行参数

一个小技巧
``` c
for (int i = 0; argv[i] != NULL; i++)
```

### 环境表

环境变量：

``` c
extern char **environ;
```

### C 程序的内存空间布局(原文翻译有问题)

(该存储空间位于内存中，或者应该称为的是虚拟地址空间(Virtual Address Space))

c 程序组成：
- 正文段，CPU 执行的机器指令部分。
- 初始化数据段
- 未初始化数据段(bss)
- 栈(自动变量以及每次函数调用时所需保存的信息都存放在此段中)
- 堆(动态存储分配)

```
|                | 高地址段（命令行参数和环境变量）
|       栈       |
|       ⬇️        |
|                |
|                |
|       ⬆️        |
|       堆       |
| 未初始化的数据 | 由 exec 初始化
|    (bbs)       | 为 0
|  初始化的数据  | 由 exec 从程序
|      正文      | 文件中读入
```

`size` 命令可报告正文段、数据段以及 bss 段长度，后面两个分别是十进制和十六进制的长度

### 共享库

共享库使得可执行文件中不再需要包含公用的库函数，
而只需要在所有进程都可引用都存储区中保存这种库都一个副本

程序第一次执行或第一次调用某个库函数时，用动态链接方法将共享库函数相链接

运行时换空间

可以共享库新版本替换老版本时无需编译

### 内存分配

1. `malloc`

1. `calloc`

1. `realloc`

``` c
#include <stdlib.h>

void *malloc(size_t size); // 分配初始不定

void *calloc(size_t nobj, size_t size); // 指定对象指定数量，分配初始为 0

void *relloc(void *ptr, size_t newsize); // 再分配

void free(void *ptr);
```

三个动态分配内存的函数以及释放空间的 `free` 函数要非常谨慎，否则非常容易出错

存在其他一些替代的存储空间分配程序

### 环境变量

环境字符串形式：

``` haskell
name = value
```

几个函数：

``` c
#include <stdlib.h>

char *getenv(const char *name);

int putenv(char *str);

int setenv(const char *name, const char *value, int rewrite);

int unsetenv(const char *name);

```

由于环境表存放于进程内存空间的顶部，其内存大小是固定的，所以增删改操作相对较复杂

### setjmp 和 longjmp

c 中 `goto` 不能跨越函数，执行该种跳转功能的是函数 `setjmp` 以及 `longjmp`

用于处理发生在很深层嵌套函数调用当中当出错情况是非常有用的

解决逐层检查返回值就是使用非局部 `goto`

``` c
#include <setjmp.h>

int setjmp(jmp_buf env);

void longjmp(jmp_buf env, int val);
```

对于栈帧上的自动变量、寄存器变量的状态得看情况

`volatile` 对应的是易失不稳定变量

全局变量，静态变量以及易失不稳定变量不会受到优化影响

自动变量存在潜在出错的情况

### getrlimit 和 setrlimit

每个进程都有资源限制，可以进行查看和更改：

``` c
#include <sys/resource.h>

int getrlimit(int resource, struct rlimit *rlptr);

int setrlimit(int resource, const struct rlimit *rlptr);

struct rlimit {
    rlim_t rlim_cur;
    rlim_t rlim_max;
}
```

软硬限制的区别是是否可以在进程运行期间自行改变

资源限制影响到调用进程并由子进程继承，所以资源限制设置构造在 `shell` 之上，其具有内置的 `ulimit` 指令







