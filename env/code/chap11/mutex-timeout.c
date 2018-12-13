#include "../apue.h"
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#define NTHR 8                  // number of threads
#define NUMNUM 8000000L         // number of numbers
#define TNUM (NUMNUM/NTHR)      // number of numbers per thread

long nums[NUMNUM];
long snums[NUMNUM];



