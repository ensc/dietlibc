#include <sys/times.h>
#include <time.h>
#include <unistd.h>

#ifdef __alpha__
# define HZ  1024
#else
# define HZ   100
#endif

clock_t  clock ( void )
{
    struct tms  buf;

    times ( &buf );

/*  printf("utime %d, stime %d, CLOCKS_PER_SEC %d, HZ %d\n",buf.tms_utime,buf.tms_stime,CLOCKS_PER_SEC,HZ); */

#if    CLOCKS_PER_SEC % HZ == 0
    return ((unsigned long) buf.tms_utime + buf.tms_stime) * (CLOCKS_PER_SEC / HZ);
#elif  HZ % CLOCKS_PER_SEC == 0
    return ((unsigned long) buf.tms_utime + buf.tms_stime) / (HZ / CLOCKS_PER_SEC);
#else
    return ((unsigned long long) buf.tms_utime + buf.tms_stime) * CLOCKS_PER_SEC / HZ;
#endif
}
