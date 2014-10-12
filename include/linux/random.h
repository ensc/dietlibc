/*
 * include/linux/random.h
 *
 * Include file for the random number generator.
 */

#ifndef _LINUX_RANDOM_H
#define _LINUX_RANDOM_H

#include <sys/cdefs.h>

__BEGIN_DECLS

#ifndef GRND_NONBLOCK
enum {
  GRND_NONBLOCK=1,
#define GRND_NONBLOCK GRND_NONBLOCK
  GRND_RANDOM=2
#define GRND_RANDOM GRND_RANDOM
};

int getrandom(void* buf, size_t buflen, unsigned int flags) __THROW;
#endif

__END_DECLS

#endif /* _LINUX_RANDOM_H */
