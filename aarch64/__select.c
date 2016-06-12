#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
  struct timespec ts;
  int r;
  if (timeout) {
    ts.tv_sec = timeout->tv_sec;
    ts.tv_nsec = timeout->tv_usec * 1000;
  }
  r = pselect(nfds, readfds, writefds, exceptfds, timeout ? &ts : NULL, NULL);
  if (timeout) {
    timeout->tv_sec = ts.tv_sec;
    timeout->tv_usec = ts.tv_nsec / 1000;
  }
  return r;
}
