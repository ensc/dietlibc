#include <ctype.h>
#include "dietfeatures.h"
#include <errno.h>
#include <limits.h>

extern unsigned long int strtoul(const char *nptr, char **endptr, int base);

long int strtol(const char *nptr, char **endptr, int base)
{
  int neg=0;
  unsigned long int v;

  while(isspace(*nptr)) nptr++;

  if (*nptr == '-') { neg=-1; ++nptr; }
  v=strtoul(nptr,endptr,base);
  if (v>=LONG_MAX) {
    if (v==LONG_MAX && !neg) {
      errno=0;
      return v;
    }
    errno=ERANGE;
    return (neg?LONG_MIN:LONG_MAX);
  }
  return (neg?-v:v);
}
