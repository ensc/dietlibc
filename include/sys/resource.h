#ifndef _SYS_RESOURCE_H
#define _SYS_RESOURCE_H

#include <sys/cdefs.h>
#include <linux/resource.h>

int getpriority(int which, int who) __THROW;
int setpriority(int which, int who, int prio) __THROW;

#endif
