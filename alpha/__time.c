#include <sys/time.h>

extern time_t __dietlibc_time(time_t *foo);

time_t time(time_t *foo) {
  time_t temp;
  temp=__dietlibc_time(0);
  temp&=0xffffffff;
  if (foo) *foo=temp;
  return temp;
}
