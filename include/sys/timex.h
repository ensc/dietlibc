#ifndef _SYS_TIMEX_H
#define _SYS_TIMEX_H

struct timex {
  int modes;           /* mode selector */
  long offset;         /* time offset (usec) */
  long freq;           /* frequency offset (scaled ppm) */
  long maxerror;       /* maximum error (usec) */
  long esterror;       /* estimated error (usec) */
  int status;          /* clock command/status */
  long constant;       /* pll time constant */
  long precision;      /* clock precision (usec) (read only) */
  long tolerance;      /* clock frequency tolerance (ppm)
			  (read only) */
  struct timeval time; /* current time (read only) */
  long tick;           /* usecs between clock ticks */
};

#define ADJ_OFFSET            0x0001 /* time offset */
#define ADJ_FREQUENCY         0x0002 /* frequency offset */
#define ADJ_MAXERROR          0x0004 /* maximum time error */
#define ADJ_ESTERROR          0x0008 /* estimated time error */
#define ADJ_STATUS            0x0010 /* clock status */
#define ADJ_TIMECONST         0x0020 /* pll time constant */
#define ADJ_TICK              0x4000 /* tick value */
#define ADJ_OFFSET_SINGLESHOT 0x8001 /* old-fashioned adjtime */

#define TIME_OK   0 /* clock synchronized */
#define TIME_INS  1 /* insert leap second */
#define TIME_DEL  2 /* delete leap second */
#define TIME_OOP  3 /* leap second in progress */
#define TIME_WAIT 4 /* leap second has occurred */
#define TIME_BAD  5 /* clock not synchronized */

int adjtimex(struct timex *buf);

#endif
