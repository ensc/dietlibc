#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

#include <limits.h>

#define MAXPATHLEN     PATH_MAX
#define MAXHOSTNAMELEN 64

#undef MIN
#undef MAX
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifndef howmany
# define howmany(x, y)  (((x)+((y)-1))/(y))
#endif
#define roundup(x, y)   ((((x)+((y)-1))/(y))*(y))
#define powerof2(x)     ((((x)-1)&(x))==0)

#endif
