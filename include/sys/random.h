#ifndef _SYS_RANDOM_H
#define _SYS_RANDOM_H

#include <sys/cdefs.h>
#include <stddef.h>

__BEGIN_DECLS

enum {
  GRND_NONBLOCK=1,
#define GRND_NONBLOCK GRND_NONBLOCK
  GRND_RANDOM=2
#define GRND_RANDOM GRND_RANDOM
};

int getrandom(void* buf, size_t buflen, unsigned int flags) __THROW;
int getentropy(void* buf,size_t buflen) __THROW;

__END_DECLS

#endif
