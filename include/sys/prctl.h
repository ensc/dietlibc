#ifndef _SYS_PRCTL_H
#define _SYS_PRCTL_H

#include <sys/cdefs.h>
#include <linux/prctl.h>

__BEGIN_DECLS

int prctl(int option, unsigned long arg2, ...) __THROW;

__END_DECLS

#endif
