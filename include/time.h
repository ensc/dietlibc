#ifndef _TIME_H
#define _TIME_H

#include <sys/cdefs.h>
#include <sys/time.h>

extern int __isleap(int year);

int nanosleep(const struct timespec *req, struct timespec *rem) __THROW;
unsigned int sleep(unsigned int secs) __THROW;

time_t mktime(struct tm *timeptr) __THROW __pure__;

char *asctime(const struct tm *timeptr) __THROW;
char *asctime_r(const struct tm *timeptr, char *buf) __THROW;

char *ctime(const time_t *timep) __THROW;

size_t strftime(char *s, size_t max, const char *format, const struct tm *tm) __THROW;
time_t time(time_t *t) __THROW;

int stime(time_t *t) __THROW;

double difftime(time_t time1, time_t time0) __THROW __attribute__((const));

#define CLOCKS_PER_SEC 1000000l

extern long int timezone;

#endif
