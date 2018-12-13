#include <pthread.h>

#define PTHREAD_BARRIER_SERIAL_THREAD 1

typedef int pthread_barrierattr_t;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int maxCount;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned int count)
{
    if (count == 0)
    {
        return -1;
    }

    if (pthread_mutex_init(&barrier->mutex, NULL) < 0)
    {
        return -1;
    }

    if (pthread_cond_init(&barrier->cond, NULL) < 0)
    {
        return -1;
    }

    barrier -> count = 0;
    barrier -> maxCount = count;

    return 0;
}

int pthread_barrier_destory(pthread_barrier_t *barrier)
{
    pthread_mutex_destroy(&barrier->mutex);

    pthread_cond_destroy(&barrier->cond);

    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    if (barrier->maxCount <= barrier->count)
    {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
        return PTHREAD_BARRIER_SERIAL_THREAD;
    } else
    {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
        pthread_mutex_unlock(&barrier->mutex);
        return -1;
    }
}

