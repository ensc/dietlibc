#include <time.h>

extern const short  __spm [];

/* apparently mktime is expected to treat the time as local time.
 * I don't understand what that is supposed to mean, though */

time_t  mktime ( register struct tm* const t )
{
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

time_t timelocal(struct tm* const t) __attribute__((alias("mktime")));

#if 0

#include <math.h>

main()
{
    int i, j, k;
    static char *s[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

    for (i=70; i<200; i++)
        for (j=0; j<12; j++)
            for (k=1; k<=31; k++) {
                struct tm t;
                double x;
                t.tm_year = i;
                t.tm_mon  = j;
                t.tm_mday = k;
                t.tm_hour = 0;
                t.tm_min  = 0;
                t.tm_sec  = 0;
                x = mktime (&t);
                printf ("%4d.%5.0f  %2u.%02u.%04u  %s\n", (int)floor(x/86400), x-86400.*(int)floor(x/86400), k, j+1, 1900+i, s[t.tm_wday] );
            }

}
#endif
