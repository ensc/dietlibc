#ifndef _SYS_TIMES_H
#define _SYS_TIMES_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/times.h>

clock_t times(struct tms *buf) __THROW;

#endif
