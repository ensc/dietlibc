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

struct tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation.  */
};

struct tm* gmtime(const time_t *t) __THROW;
struct tm* gmtime_r(const time_t *t, struct tm *r) __THROW;

struct tm* localtime(const time_t *t) __THROW;
struct tm* localtime_r(const time_t *t, struct tm *r) __THROW;

#ifndef __FD_SET
#define	__FD_SET(d, set)	((set)->fds_bits[__FDELT(d)] |= __FDMASK(d))
#define	__FD_CLR(d, set)	((set)->fds_bits[__FDELT(d)] &= ~__FDMASK(d))
#define	__FD_ISSET(d, set)	(((set)->fds_bits[__FDELT(d)] & __FDMASK(d)) != 0)
#define	__FD_ZERO(set)	\
  ((void) memset ((void*) (set), 0, sizeof (__kernel_fd_set)))
#endif

#endif
