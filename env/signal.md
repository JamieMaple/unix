# 信号

### 信号概念

定义在 `<signal.h>` 中，信号名为正整数常量，信号是异步事件的经典实例

信号来源无非硬件和软件两种来源

处理信号的三种方式：

1. 忽略此信号，除 `SIGKILL` 和 `SIGSTOP` 之外的

1. 捕捉信号，除上面两种信号之外

1. 执行系统默认动作

PS: 程序奔溃时，在进程当前工作目录的 core 文件复制了该进程的存储图像

`core` 文件产生有限制

### signal 函数

``` c
#include <signal.h>

void (*signal(int signo, void (*func)(int)))(int);

// 换种方式看它

typedef void Sigfunc(int);

Sigfunc *signal(int, Sigfunc);
```

由于 `signal` 函数语义和实现有很大关系，所以最好使用 `sigaction` 替代

`kill` 这个词是有点不恰当的，因为其其实是将一个信号发送给一个进程或进程组

`func` 允许的值：

1. `SIG_IGN`：忽略信号

1. `SIG_DFL`：默认行为

1. 指定函数地址时，就称为捕捉该信号，此函数也被称为信号处理函数(signal handler)或吸纳后捕捉函数(signal-catching function)

``` c
#define SIG_ERR ((void (*)())-1)
#define SIG_DFL (void (*)())0
#define SIG_IGN (void (*)())1
```

### 不可靠的信号

不可靠指的是信号可能会丢失

早期的 Unix 版本中存在

### 终端的系统调用

如果进程在执行一个低速系统调用二阻塞期间捕捉到一个信号，则该系统调用就会被中断而不再继续执行

### 可重入函数(Reentrant Functions)

在信号处理程序中保证调用安全的函数，这些函数是可重载入的，也被称为异步信号安全的（async-signal safe）

但是信号处理函数也会修改每个不同线程的 `errno`

在信号处理程序中调用一个非重入的函数，结果是不可预知的

### SIGCLD 语义

目前的实现上看来就是 `SIGCHLD` 的 `alias`

### 可靠信号术语和语义

当造成信号的事件发生时，为进程产生一个信号。

当一个信号产生时，内核通常以某种形式在进程表中设置一个标志位。

在信号产生和递送过程之间存在时间间隔，称信号是未决的。

每个进程都有信号屏蔽字，规定了当前要阻塞递送到该进程到信号集，其类型是 `sigset_t`。

### kill 和 raise 函数

`kill` 函数将信号发送给进程或进程组，`raise` 函数将允许进程向自身发送信号

``` c
#include <signal.h>

int kill(pid_t  pid, int signo);

int raise(int signo);
```

### alarm 和 pause 函数

设置一个定时器：

``` c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

进程调度存在时延，所以还需要一个时间间隔。

使调用进程挂起知道捕捉到一个信号：

``` c
#include <unistd.h>

int pause(void);
```

### 信号集

信号集处理函数：

``` c
#include <signal.h>

int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signo);
int sigdelset(sigset_t *set, int signo);

int sigismember(sigset_t *set, int signo);
```

### sigprocmask 函数

检测和更改进程信号屏蔽字：

``` c
#include <signal.h>

int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);
```

PS: 单线程进程专用，看来用处很小了。

### sigpending 函数

> This function returns the set of signals that are blocked from delivery
> and currently pending for the calling process

``` c
#include <signal.h>

int sigpending(sigset_t *set);
```

### sigaction 函数

检查与修改和指定信号相关联的处理动作：

``` c
#include <signal.h>

int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict oact);

struct sigaction {
    void (*sa_handler)(int);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_sigaction)(int, siginfo_t *, void);
}
```

### sigsetjmp 和 siglongjmp 函数

信号处理相关的非局部转移：

``` c
#include <setjmp.h>

int sigsetjmp(sigjmp_buf env, int savemask);

void siglongjmp(sigjmp_buf env, int val);
```

### sigsuspend 函数

恢复信号屏蔽子然后使程序进入休眠的一个原子操作：

``` c
#include <signal.h>

int sigsuspend(const sigset_t *sigmask);
```

### abort 函数

使得程序异常终止：

``` c
#include <stdlib.h>

void abort(void);
```

### system 函数

第八章曾做过的函数是有缺陷的

### sleep、nanosleep 和 clock_nanosleep 函数

挂起一段时间：

``` c
#include <unistd.h>

unsigned int sleep(unsigned int seconds);

#include <time.h>

int nanosleep(const struct timespec *reqtp, struct timespec *remtp);

#include <time.h>

int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *reqtp, struct timespec *remtp);
```

### sigqueue 函数

对信号排队进行了支持：

``` c
#include <signal.h>

int sigqueue(pid_t pid, int signo, const union sigval value);
```

### 作业控制信号

作业控制相关的：

``` c
SIGCHLD
SIGCONT
SIGSTOP
SIGTSTP
SIGTTIN
SIGTTOU
```

### 信号名和编号

信号编号和信号名之间的映射:

``` c
extern char *sys_siglist [];
```




