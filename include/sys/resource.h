#ifndef _SYS_RESOURCE_H
#define _SYS_RESOURCE_H

#include <sys/cdefs.h>
#include <linux/resource.h>

#undef RLIM_INFINITY
#define RLIM_INFINITY ((long)(~0UL>>1))

int getpriority(int which, int who) __THROW;
int setpriority(int which, int who, int prio) __THROW;

int getrlimit (int resource, struct rlimit *rlim);
int getrusage (int who, struct rusage *usage);
int setrlimit (int resource, const struct rlimit *rlim);

typedef unsigned long rlim_t;

#endif
