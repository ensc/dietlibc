#ifndef _SYS_SYSINFO_H
#define _SYS_SYSINFO_H

#include <sys/cdefs.h>
#include <linux/kernel.h>

int sysinfo(struct sysinfo *info) __THROW;

#endif
