#ifndef _SYS_TIME_H
#define _SYS_TIME_H	1

#include <sys/cdefs.h>

#include <sys/types.h>
#include <linux/time.h>

#if defined __USE_GNU || defined __USE_BSD
typedef struct timezone *__timezone_ptr_t;
#else
typedef void *__timezone_ptr_t;
#endif

int getitimer(int which, struct itimerval *value) __THROW;
int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue) __THROW;

int gettimeofday(struct timeval *tv, struct timezone *tz) __THROW;
int settimeofday(const struct timeval *tv , const struct timezone *tz) __THROW;

extern int adjtime (const struct timeval *__delta,
		    struct timeval *__olddelta) __THROW;

#endif
