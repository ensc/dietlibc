#ifndef _TIME_H
#define _TIME_H

#include <sys/cdefs.h>
#include <sys/time.h>

__BEGIN_DECLS

extern int __isleap(int year);

int nanosleep(const struct timespec *req, struct timespec *rem) __THROW;

time_t mktime(struct tm *timeptr) __THROW __pure;

char *asctime(const struct tm *timeptr) __THROW;
char *asctime_r(const struct tm *timeptr, char *buf) __THROW;

char *ctime(const time_t *timep) __THROW;

size_t strftime(char *s, size_t max, const char *format, const struct tm *tm) __THROW __attribute__((format(strftime,3,0)));
time_t time(time_t *t) __THROW;

int stime(time_t *t) __THROW;

double difftime(time_t time1, time_t time0) __THROW __attribute__((const));

#define CLOCKS_PER_SEC 1000000l

extern long int timezone;
extern int daylight;
extern char* tzname[2];

void tzset (void) __THROW;

struct tm* localtime(const time_t* t) __THROW;
struct tm* gmtime(const time_t* t) __THROW;
struct tm* localtime_r(const time_t* t, struct tm* r) __THROW;
struct tm* gmtime_r(const time_t* t, struct tm* r) __THROW;

clock_t clock(void);

char *strptime(const char *s, const char *format, struct tm *tm);

#ifdef _GNU_SOURCE
time_t timegm(struct tm *timeptr) __THROW __attribute_dontuse__ __pure ;
time_t timelocal(struct tm *timeptr) __THROW __attribute_dontuse__ __pure;
#endif

__END_DECLS

#endif
