#include <time.h>

/* nano * 1000 == usecs
 * usecs * 1000 == msecs
 * msecs * 1000 = secs */
void usleep(unsigned int usecs) {
  struct timespec t;
  t.tv_sec=usecs/1000000;
  t.tv_nsec=(usecs%1000000)*1000;
  nanosleep(&t,&t);
}
