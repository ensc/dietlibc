#include <time.h>
#include <sys/time.h>

struct tm* localtime_r(const time_t* t, struct tm* r) {
  struct timezone tz;
  time_t tmp;
  gettimeofday(0, &tz);
  tmp=*t-tz.tz_minuteswest*60L;
  return gmtime_r(&tmp,r);
}
