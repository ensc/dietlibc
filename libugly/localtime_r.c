#include "dietfeatures.h"
#include <time.h>
#include <sys/time.h>

#ifdef WANT_TZFILE_PARSER
extern void __maplocaltime();
extern time_t __tzfile_map(time_t t, int *isdst);
#endif

struct tm* localtime_r(const time_t* t, struct tm* r) {
  struct timezone tz;
  time_t tmp;
  gettimeofday(0, &tz);
#ifdef WANT_TZFILE_PARSER
  __maplocaltime();
  tmp=__tzfile_map(*t,&r->tm_isdst);
#else
  tmp=*t-tz.tz_minuteswest*60L;
#endif
  return gmtime_r(&tmp,r);
}
