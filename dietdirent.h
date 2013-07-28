#include <sys/shm.h>

#include "dietpagesize.h"
#include "dietunaligned.h"

struct __dirstream {
  int fd;
  unsigned int num;
  unsigned int cur;
  char buf[] __attribute__((__aligned__(8)));
};				/* stream data from opendir() */

#define __DIRSTREAM_BUF_SIZE	(__DIET_PAGE_SIZE - offsetof(struct __dirstream, buf))

#define dirstream_get_reclen(_d, _type) \
	read_unaligned_s((_d)->buf + (_d)->cur, _type, d_reclen)
