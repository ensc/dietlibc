#include "dietfeatures.h"
#include <time.h>
#include <sys/time.h>

#ifdef WANT_TZFILE_PARSER
extern void __maplocaltime(void);
extern time_t __tzfile_map(time_t t, int *isdst);
#endif

struct tm* localtime_r(const time_t* t, struct tm* r) {
  time_t tmp;
#ifdef WANT_TZFILE_PARSER
  __maplocaltime();
  tmp=__tzfile_map(*t,&r->tm_isdst);
#else
  struct timezone tz;
  gettimeofday(0, &tz);
  tmp=*t-tz.tz_minuteswest*60L;
#endif
  return gmtime_r(&tmp,r);
}
