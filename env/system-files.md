# 系统数据文件和信息

### 口令文件

口令文件结构体：

``` c
#include <pwd.h>

struct passwd {
    char *pw_name;
    char *pw_passwd;
    uid_t pw_uid;
    gid_t pw_gid;
    char *pw_gecos;
    char *pw_dir;
    char *pw_shell;
    char *pw_class;
    time_t pw_change;
    time_t pw_expire;
}

```

相关操作几个函数：

``` c
#include <pwd.h>
struct passwd *getpwuid(uid_t uid);

struct passwd *getpwnam(const char *name);

struct passwd *getpwent(void);

void setpwent(void);

void endpwent(void);
```

### 阴影口令

阴影口令结构体：

``` c
struct spwd {
    char *sp_namp;
    char *sp_pwdp;
    int sp_lschg;
    int sp_min;
    int sp_max;
    int sp_warn;
    int sp_inact;
    int sp_expire;
    unsigned int sp_flag;
}
```

相关操作：

``` c
#include <shadow.h>

struct spwd *getspnam(const char *name);

struct spwd *getspent(void);

void setspent(void);

void endspent(void);
```

### 组文件

``` c
#include <grp.h>

struct group {
    char *gr_name;
    char *gr_passwd;
    int gr_gid;
    char **gr_mem;
}

struct group *getgrdid(gid_t gid);

struct group *getgrnam(const char *name);

```

相关操作：

``` c
#include <grp.h>
struct group *getgrent(void);

void setgrent(void);

void endgrent(void);

```

### 附属组

``` c
#include <unistd.h>

int getgroups(int gidsetsize, gid_t grouplist[]);

#include <grp.h>

#include <unistd.h>

int setgroupsv(int ngroups, const gid_t grouplist[])

#include <grp.h>

#include <unistd.h>

int initgroups(const char *username, gid_t basegid);
```

### 登陆账户记录

``` c
struct utmp {
    char ut_line[8];
    char ut_name[8];
    long ut_time;
}

```

### 系统标示


### 时间和日期例程

Unix 内核提供的基本时间服务是计算自协调世界时。

秒数以 `time_t` 表示的，通常称为日历时间。

1. 以协调统一时间而非本地时间即使

1. 可自动进行转换，如变换到夏令时

1. 将时间和日期作为一个量值保存

返回当前时间和日期：

``` c
#include <time.h>

time_t time(time_t *calptr);
```

获取制定时钟到时间(秒和纳秒):

``` c
#include <sys/time.h>

int clock_gettime(clockid_t clock_id, struct timespec *tsp);

```

设置精度：

``` c
#include <sys/time.h>

int clock_getres(clockid_t clock_id, struct timespec *tsp);
```

``` c
#include <time.h>

struct tm *gmtime(const time_t *calptr);

struct tm *localtime(const time_t *calptr);
```

格式化输出

``` c
#include <time.h>

size_t strftime(char *restrict buf, size_t maxsize, const char *restrict format, const struct tm *restrict tmptr);

size_t strftime_l(char *restrict buf, size_t maxsize, const char *restrict format, const struct tm *restrict tmptr, locale_t locale);
```

字符串分解时间：

``` c
#include <time.h>

char *strptime(const char *restrict buf, const char *restrict format, struct tm *restrict tmptr);
```

三个函数：`localtime` `strftime` `mktime` 都会受到环境变量 TZ 到影响

