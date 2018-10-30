# 进程关系

### 终端登陆

1. BSD 登录

系统管理员创建 `/etc/ttys` 文件，每一行说明设备名和传到 `getty` 程序的参数

系统自举时，内核创建 `init` 进程（ID 为 1）

多用户模式读取文件 `/etc/ttys` 每次登陆终端设备 `init` 调用 `fork`，子进程再调用 `exec getty`

`getty` 对设备调用 `open`，以读写方式打开设备，0、1和2描述符被设置到该设备

`getty` 对用户输入处理，输入用户名和密码登陆成功后，`execle("/bin/login", "login", "-p", username, (char *)0, envp)`

`login` 接下里又会做一些权限相关的事情，同时还会进行 `setuid`

然后登陆用户的登陆 `shell` 开始运行，然后 `shell` 读取启动文件

### 网络登陆


`login` 作为一种服务

为使同一个软件既能处理终端登录，又能处理网络登录，系统使用了一种称为伪终端的软件驱动程序。

1. BSD 实现

作为系统启动的一部分，`init` 会调用一个执行 `/etc/rc` shell 脚本。`inetd` 守护进程会被启动，它会等待 `TCP/IP` 请求。

当请求到达时，`inetd` 会使用 `fork` 和 `exec` 然后生成适当子进程

### 进程组

1. 进程组是一个或多个进程的集合

1. 每个进程组有一个组长进程，组长进程的进程 ID 等于其进程 ID。

得到和设置进程组的 ID:

``` c
#include <unistd.h>

pid_t getpgrp(void);
int setpgid(pid_t pid, pid_t pgid);
```

### 会话

会话（session）是一个或多个进程组的集合

创建和得到会话：

``` c
#include <unistd.h>

pid_t setsid(void);

pid_t getsid(pid_t pid);
```


### 控制终端

1. 一个会话可以有一个控制终端，这通常是终端设备或伪终端设备

1. 建立与控制终端连接的会话首进程通常被称为控制终端

1. 一个会话中的几个进程组可以被分成一个前台进程组以及一个或多个后台进程组

1. 如果终端接口检测到调制解调器已经断开，则将挂断信号发送至控制进程(通常是会话首进程)

### 函数 `tcgetpgrp` `tcsetpgrp` 和 `tcgetsid`

通知内核哪一个进程组是前台进程组，这样，终端设备驱动程序就能知道将终端输入和终端产生的信号发送到何处

``` c
#include <unistd.h>

pid_t tcgetpgrp(int f);

int tcsetpgrp(int fd, pid_t pgrpid);
```

给出 TTY 的文件描述符，通过下列函数能够获得会话首进程的进程组 ID：

``` c
#include <termios.h>

pid_t tcgetsid(int fd);
```

需要管理控制终端的应用程序可以调用该函数获得控制终端的会话首进程的会话 ID

### 作业控制

> Job Control allows us to start multple jobs (groups of processes) from a single terminal and to control which jobs can access the terminal and which jobs are run in the background

例子：

``` c
// foreground
vim main.c

// background
pr *.c | lpr &
make all &
```

挂起键：`Ctrl + z`，将信号 SIGTSTP 发送到前台进程组所有进程中，后台进程组作业不会受其影响

退出字符：`Ctrl + \\` 产生信号 SIGQUIT

中断字符：`Ctrl + c` 或 `Delete` 产生 SIGINT

当后台作业试图读终端输入时，终端驱动会检测到同时向后台作业发送一个特定信号 `SIGTTIN`

当转为前台进程组时，会将信号(SIGCONT)送给该进程组

当后台作业试图向控制终端写时，终端驱动程序会识别出该操作，并向该作业发出 `SIGTTOU` 信号

### Shell 执行程序

这里书上和 macos 以及 Ubuntu 上表现不一样，

猜测是由于 Bourne shell 有关

### 孤儿进程组

> a process whose parent terminates it called an orphan and is inherited by the init process

孤儿进程组：

1. POSIX.1

> An orphaned process group is one in which the parent of every member is either itself a member of the group
> or is not a member of the group's session.

要么是改组的一个成员，要么不是改组所属会话的成员

或者说，当该组中有一个进程，其父进程在同一会话的另一个组中，该组就不是孤儿进程组

### FreeBSD 实现

进程、进程组、会话和控制终端(tty)几个重要属性:

``` c
// session
struct session {
    int s_ count;               // 计数器
    struct proc *s_leader;      // 首进程 proc 指针
    struct vnode *s_ttyvp;      // vnode pointer
    struct tty *s_ttyp;         // 指向控制终端 tty
    int s_sid;                  // session id
}

// tty
struct tty {
    struct session *t_session;                  // session pointer，相互指向
    struct pgrp *t_pgrp;                        // 前台进程组
    struct termios t_termios;                   // 包含一些特殊字符和该终端有关的信息
    struct winsize t_winsize;                   // 终端窗口大小结构
}

// pgrp
struct pgrp {
    int pg_id;                      // 进程组 ID
    struct session *pg_session;     // 指向所属 session 结构的指针
    struct proc *pg_members;        // 指向进程组 proc 的结构表指针
}

// proc 进程
struct proc {
    int p_pid;                            // 进程 ID
    struct pgrp *p_pptr;                  // 指向所属进程组的 pgrp 结构的指针
    struct pglist p_pglist;               // 包含指向进程组上一个和下一个进程的结构，双向链表
}
```


