/* This is adapted from glibc */
/* Copyright (C) 1991, 1993 Free Software Foundation, Inc */

#define SECS_PER_HOUR 3600L
#define SECS_PER_DAY  86400L

#include <time.h>

static const unsigned short int __mon_lengths[2][12] =
  {
    /* Normal years.  */
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    /* Leap years.  */
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
  };


void
__tm_conv(tmbuf, t, offset)
struct tm *tmbuf;
time_t *t;
time_t offset;
{
  long days, rem;
  register int y;
  register const unsigned short int *ip;

  days = *t / SECS_PER_DAY;
  rem = *t % SECS_PER_DAY;
  rem += offset;
  while (rem < 0)
    {
      rem += SECS_PER_DAY;
      --days;
    }
  while (rem >= SECS_PER_DAY)
    {
      rem -= SECS_PER_DAY;
      ++days;
    }
  tmbuf->tm_hour = rem / SECS_PER_HOUR;
  rem %= SECS_PER_HOUR;
  tmbuf->tm_min = rem / 60;
  tmbuf->tm_sec = rem % 60;
  /* January 1, 1970 was a Thursday.  */
  tmbuf->tm_wday = (4 + days) % 7;
  if (tmbuf->tm_wday < 0)
    tmbuf->tm_wday += 7;
  y = 1970;
  while (days >= (rem = __isleap(y) ? 366 : 365))
    {
      ++y;
      days -= rem;
    }
  while (days < 0)
    {
      --y;
      days += __isleap(y) ? 366 : 365;
    }
  tmbuf->tm_year = y - 1900;
  tmbuf->tm_yday = days;
  ip = __mon_lengths[__isleap(y)];
  for (y = 0; days >= ip[y]; ++y)
    days -= ip[y];
  tmbuf->tm_mon = y;
  tmbuf->tm_mday = days + 1;
  tmbuf->tm_isdst = -1;
}

