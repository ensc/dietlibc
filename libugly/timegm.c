#define _GNU_SOURCE
#include <time.h>

/* this is cut and paste from mktime. */

extern const short  __spm [];

time_t timegm(struct tm *const t) {
    register time_t  day;
    register time_t  i;

    if ( t->tm_year < 70 )
        return (time_t) -1;

    day = t->tm_yday = __spm [t->tm_mon] + t->tm_mday-1 + ( __isleap (t->tm_year+1900)  &  (t->tm_mon > 1) );

    for ( i = 70; i < t->tm_year; i++ )
        day += 365 + __isleap (i+1900);

    /* day is now the number of days since 'Jan 1 1970' */
    i = 7;
    t->tm_wday = (day + 4) % i;                        /* Sunday=0, Monday=1, ..., Saturday=6 */

    i = 24;
    day *= i;
    i = 60;
    return ((day + t->tm_hour) * i + t->tm_min) * i + t->tm_sec;
}

