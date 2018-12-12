# 线程

### 线程概念

每个线程能够处理各自独立的任务

处理器的数量并不影响程序线程代码

### 线程标识

由于进程标示实现不太一样，因此使用函数进行比较

``` c
#include <pthread.h>

int pthread_equal(pthread_t tid1, pthread_t tid2);

// 获取自身 thread id
pthread_t pthread_self(void);
```

### 线程创建

``` c
#include <pthread.h>

int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);
```

### 线程创建

``` c
#include <pthread.h>

int pthread_create(
    pthread_t *restrict tipd,
    const pthread_attr_t *restrict atrr,
    void *(*start_rtn)(void *), void *restrict arg);
```

### 线程终止

不终止整个进程下，三种方式退出：

1. 简单地从启动例程中退出

1. 线程可以被同一进程的其他线程取消

1. 线程调用 `pthread_exit`

``` c
#include <pthread.h>

void pthread_exit(void *rval_ptr);

int  pthread_join(pthread_t thread, void **rval_ptr);

void pthread_cleanup_push(void (*rtn*rtn*rtn*rtn*rtn*rtn*rtn*rtn)(void *), void *arg);

void pthread_cleanup_pop(int execute);

int pthread_detach(pthread_t tid);
```

### 线程同步

关键词：

1. 内存单元

1. 存储访问时间

1. 原子操作

### 互斥量

``` c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);

int pthread_mutex_destory(pthread_mutex_t *mutex);

int pthread_mutex_lock(pthread_mutex_t *mutex);

int pthread_mutex_trylock(pthread_mutex_t *mutex);

int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

另一个可以指定阻塞时间的 mutex 函数, macos 需要自己实现：

``` c
#include <pthread.h>
#include <time.h>

int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict tsptr);

```

### 读写锁

> Reader-writer locks are similar to mutexes, except that they allow for higher degrees of parallelism.
> Three states are possible with a reader-writer lock: locked in read mode, locked inn write mode and unlocked.

创建和销毁读写锁：

``` c
#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

初始化和关闭锁：

``` c
#include <pthread.h>

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);

int pthread_rwlock_wdlock(pthread_rwlock_t *rwlock);

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```




