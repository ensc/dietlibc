#include <unistd.h>
#include <time.h>
#include <sys/time.h>

main() {
  int i;
  time_t t;
  for (i=0; i<10000000; ++i)
    t=time(0);
}
