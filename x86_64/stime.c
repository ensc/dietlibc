#include <errno.h>
#include <sys/time.h>

int stime(const time_t *when)
{
  struct timeval tv;
  tv.tv_sec = *when;
  tv.tv_usec = 0;
  return settimeofday(&tv, (struct timezone *)0);
}
