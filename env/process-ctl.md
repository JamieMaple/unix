# 进程控制

### 进程标识

进程 ID 表示符是唯一的，可复用的

| ID | 作用                               | 相关的文件                                                                 |
|----|------------------------------------|----------------------------------------------------------------------------|
| 0  | 用于调度进程(交换进程)             | 并不执行任何磁盘上的程序                                                   |
| 1  | 在自举内核后启动 Unix 系统         | 初始化系统相关文件(/etc/init or /sbin/init or /etc/inittab or /etc/init.d) |
| 2  | 也守护进程，虚拟内存系统的分页操作 | 应该是虚拟内存中的                                                         |

获取进程一些标识符的函数:

``` c
#include <unistd.h>

pid_t getpid(void);

pid_t getppid(void);

uid_t getuid(void);

uid_t geteuid(void);

gid_t getgid(void);

gid_t getegid(void);
```

### fork

函数签名：

``` c
#include <unistd.h>

pid_t fork(void);
```

要注意到父子进程与缓冲形式的关系

比如父进程标准输出被重定向，那么子进程也会被重定向

父子进程每个相同的打开描述符共享文件表项

可能失败的原因：

1. 进程过多

2. 实习用户 ID 的进程总数达到了限制

### vfork

类似 `fork` 函数：

``` c
#include <unistd.h>

pid_t vfork(void);
```

`vfork` 的主要区别在于:

1. 并不完全复制父进程的地址空间，所以效率有所提高

2. `vfork` 保证子进程优先级更高

虽然会共用，但是文件描述符得到的是父进程的文件描述符数组的副本

但是要注意的是，子进程会借用父进程的 `FILE` 对象

### exit

5 种正常终止，3 种异常终止方式


父进程在子进程之前终止：孤儿进程，子进程都会被 init 进程所收养

子进程没有被正常善后：僵尸进程，子进程已经终止、但是其父进程尚未对其进行善后处理，（状态打印出来是 Z）

注：init 的子进程不可能成为僵尸进程，因为只要又一个结束都会让 init 调用一个 wait

### wait 和 waitpid

等待子进程操作：(异步操作)

``` c
#include <sys/wait.h>

pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);
```

传入的 `statloc` 存放了状态，如果不关心可以放 `NULL`

获取一些状态码：

``` c
WIFEXITED(status);
WIFSIGNALED(status);
WIFSTOPPED(status);
WIFCONTINUED(status)
```

对于 `waitpid` 的 `pid` 参数作用：

``` c
pid == -1 任意
pid > 0   相等 pid
pid == 0  等待组ID等于调用进程组 ID 的任一子进程
pid < -1  等待组ID等于 pid 绝对值的任一子进程
```

两次 fork 进程可以不出现僵尸进程，具体做法是让子进程 `fork` 出孙子进程然后子进程退出，这样孙子进程就会马上变成孤儿进程，进而由 `init` 进程托管

因为儿子进程先退出，孙子进程被 init 进程接管后，和父进程脱离了关系

### waitid

与 `waitpid` 有所相似，`waitid` 取得进程终止状态

``` c
#include <sys/wait.h>

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```

`idytype` 常量：

| 常量   | 说明                       |
|--------|----------------------------|
| P_PID  | 等待一特定进程             |
| P_PGID | 等待一特定进程组重的子进程 |
| P_ALL  | 任一进程                   |

`options` 参数有更多标志位同时能进行位运算

### wait3 和 wait4

``` c
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

pid_t wait3(int *statloc, int options, struct rusage *rusage);

pid_t wait4(pid_t pid, int *statloc, int options, struct rusage);
```

多了一个 `rusage` 参数，该参数能够获取资源统计信息

### 竞争条件

竞态发生条件：

> a race condition occurs when multiple processes are trying to do
> something whith shared data and the final outcome depends on the order
> in which the processes run.

轮训：

``` c
while (getpid() != 1) {
    sleep(1);
}
```

但是非常耗费 CPU 时间

信号机制与进程间通信(IPC)可以解决该类问题

类似以下情形代码：

``` c
/* some includes headers here */

TELL_WAIT() // set things up for TELL_xxx & WAIT_xxx

if ((pid = fork()) < 0) {
    err_sys("fork error");
} else if (pid == 0) {
    // child does something
    TELL_PARENT(getppid());
    WAIT_PARENT();
}

// parent does something
TELL_CHILD(pid);
WAIT_CHILD();

exit(0);

```

PS：假定 `TELL_WAIT`, `TELL_PARENT`, `TELL_PARENT`, `TELL_CHILD`, `WAIT_PARENT` 以及 `WAIT_CHILD` 是定义的宏或者函数


### exec

`exec` 函数并不创建新的进程，但是能够用新程序替换当前进程的正文段、数据段、堆段和栈段:

``` c

#include <unistd.h>

int execl(const char *pathname, const char *arg0, ...);

int execv(const char *pathname, char *const argv[]);

int execle(const char *pathname, const char *arg0, ...);

int execve(const char *pathname, char *const argv[], char *const envp[]);

int execlp(const char *filename, const char *arg0, ...);

int execvp(const char *filename, char *const arg[]);

int fexecve(int fd, char *const argv[], char *const envp[]);

```

几个函数一般传入的 `argv` 都以 `char* 0` 作为结束，否则会触发 `Bad address` 错误

应该对函数名进行理解记忆，比如 `l` 代表了 `list`，`v` 代表的是 `vector`

`p` 代表的是 `pathname`，`e` 代表的是 `env`

几个函数调用都会最终转化成 `execve`，只有该函数是系统调用

### 更改用户 ID 和更改组 ID

设计应用时，应该使用最小特权模型

设置和取得实际用户ID和有效用户ID或实际组ID和有效组ID：

``` c
#include <unistd.h>

int setuid(uid_t uid);
int setgid(gid_t gid);
```

交换实际用户ID和有效用户ID：

``` c
#include <unistd.h>

int setreuid(uid_t ruid, uid_t euid);

int setregid(gid_t rgid, gid_t egid);
```

更改和取得有效用户ID和有效用户组ID：

``` c
#include <unistd.h>

int seteuid(uid_t uid);

int setegid(gid_t gid);
```

apue 上以 `at` 命令为例


### 解释器文件(interpreter file)

解释器文件，文件开头是：

``` shell
#! pathname [optional argument]
```

一般而言，`pathname` 是一个绝对路径，第一行也有长度限制

基于以下好处：

1. 可以隐藏所使用的某种语言

1. 效率方面有提升

1. 可以用其他 shell 来编写脚本（不单单只用 shell 脚本）

### 函数 `system`

传入字符串更方便的进行操作，`system` 函数签名：

``` c
#include <stdlib.h>

int system(const char *cmdstring);
```

该函数内部使用了 `fork` `exec` 以及 `waitpid` 三个函数

### 进程会计

启用进程会计后，每当进程结束时，内核就会写一个会议记录

相关命令：`dd`

### 用户标示

任意进程都可以得到其实际用户ID和有效用户ID和组ID，但是有时候我们希望能够找到运行该程序用户的登陆名

通过以下函数能够获得用户登陆时的登陆名

``` c
#include <unistd.h>

char *getlogin(void);
```

### 进程调度

系统调度策略和调度优先级是由内核所确定的。但是只有特权进程能够提高自身的优先级。

进程的优先级是通过 `nice` 值确定的，值越小优先级越高

进程获取或更改它的 `nice` 值:

``` c
#include <unistd.h>

int nice(int incr);
```

获取或设置一组相关进程的 `nice` 值:

``` c
#include <sys/resource.h>

int getpriority(int which, id_t who);

int setpriority(int which, id_t who, int value);
```

相关命令：`nice`

### 进程时间

任一进程调用获取自己以及已终止子进程的时间值。

``` c
#include <sys/time.h>

clock_t times(struct tms *buf);

struct tms {
    clock_t tms_utime; // user time
    clock_t tms_stime; // system time
    clock_t tms_cutime; // user time, terminated children
    clock_t tms_cstime; // system time, terminated children
}
```



