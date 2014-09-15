#ifndef _SYS_EVENTFD_H
#define _SYS_EVENTFD_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <fcntl.h>

__BEGIN_DECLS

enum {
  EFD_CLOEXEC = O_CLOEXEC,
#define EFD_CLOEXEC EFD_CLOEXEC
  EFD_NONBLOCK = O_NONBLOCK,
#define EFD_NONBLOCK EFD_NONBLOCK
  EFD_SEMAPHORE = 1
#define EFD_SEMAPHORE EFD_SEMAPHORE
};

typedef uint64_t eventfd_t;

int eventfd(unsigned int initval, int flags) __THROW;

__END_DECLS

#endif
