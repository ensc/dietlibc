#ifndef _SYS_PTRACE_H
#define _SYS_PTRACE_H

#include <sys/cdefs.h>

#include <linux/ptrace.h>

extern long int ptrace (int request, ...) __THROW;

#endif
