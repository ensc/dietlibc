#ifndef __SYS_TIMEB_H
#define __SYS_TIMEB_H 1

struct timeb {
  time_t   time;
  unsigned short millitm;
  short    timezone;
  short    dstflag;
};

int ftime(struct timeb *tp);

#endif
