#include <unistd.h>

#include <pthread.h>
#include "thread_internal.h"

#if 1
#include <dietwarning.h>

int tcdrain(int fd) { return 0; }

link_warning("tcdrain","tcdrain not yet available in dietlibc (no pthread wrapper)");
#else
int tcdrain(int fd)
{
  __TEST_CANCEL();
  return __libc_tcdrain(fd);
}
#endif
