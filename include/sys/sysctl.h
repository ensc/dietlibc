#ifndef _SYS_SYSCTL_H_
#define _SYS_SYSCTL_H_

#include <sys/types.h>
#include <linux/sysctl.h>

extern int sysctl (int *, int, void *, size_t *, void *, size_t);

#endif
