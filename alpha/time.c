#include <time.h>
#include <sys/time.h>

time_t time(time_t *foo) {
  struct timeval tv;
  time_t tmp=(time_t)-1;
  if (gettimeofday(&tv,0)==0)
    result=(time_t)tv.tv_sec;
  if (foo) *foo=result;
  return result;
}
