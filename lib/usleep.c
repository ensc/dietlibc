#include <linux/time.h>
#include <time.h>

unsigned int usleep(unsigned int usecs) {
  struct timespec t;
  t.tv_sec=usecs/1000;
  t.tv_nsec=(usecs%1000)*1000;
  nanosleep(&t,&t);
  return usecs-(t.tv_sec*1000)+(t.tv_nsec/1000);
}

