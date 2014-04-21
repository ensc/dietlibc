#include <time.h>

static char buf[26];

char *asctime(const struct tm *timeptr) {
  return asctime_r(timeptr,buf);
}
