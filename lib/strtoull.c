#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

unsigned long long int strtoull(const char *nptr, char **endptr, int base)
{
  int neg = 0, overflow = 0;
  long long int v=0;
  const char* orig;

  while(isspace(*nptr)) ++nptr;

  if (*nptr == '-') { neg=1; nptr++; }
  if (*nptr == '+') ++nptr;
  if (!base) {
    if (*nptr=='0') {
      base=8;
      if ((*(nptr+1)=='x')||(*(nptr+1)=='X')) {
	nptr+=2;
	base=16;
      }
    }
    else
      base=10;
  }
  orig=nptr;
  while(__likely(*nptr)) {
    register unsigned char c=*nptr;
    c=(c>='a'?c-'a'+10:c>='A'?c-'A'+10:c-'0');
    if (__unlikely(c>=base)) {
      if (nptr==orig) { if (endptr) *endptr=(char*)nptr; errno=EINVAL; return ULLONG_MAX; }
      break;
    }
    {
      register unsigned long x=(v&0xff)*base+c;
      register unsigned long long w=(v>>8)*base+(x>>8);
      if (w>(ULLONG_MAX>>8)) overflow=1;
      v=(w<<8)+(x&0xff);
    }
    ++nptr;
  }
  if (endptr) *endptr=(char *)nptr;
  if (overflow) {
    errno=ERANGE;
    return ULONG_MAX;
  }
  return (neg?-v:v);
}

/* die, BSD, die!!! */
unsigned long long int strtouq(const char *nptr, char **endptr, int base)
	__attribute__((weak,alias("strtoull")));
