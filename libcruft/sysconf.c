#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>

long sysconf(int name)
{
  switch(name)
   {
   case _SC_OPEN_MAX:
       {
       struct rlimit limit;
       getrlimit(RLIMIT_NOFILE, &limit);
       return limit.rlim_cur;
       }
   case _SC_CLK_TCK:
#ifdef __alpha__
       return 1024;
#else
       return 100;
#endif
   }

  return -1;
}
