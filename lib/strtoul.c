#include <ctype.h>
#include "dietfeatures.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

unsigned long int strtoul(const char *nptr, char **endptr, int base)
{
  int neg = 0, overflow = 0;
  unsigned long int v=0;
  const char* orig;

  while(isspace(*nptr)) ++nptr;
  if (*nptr == '-') { neg=1; nptr++; }
  if (*nptr == '+') ++nptr;
  if (base==16 && nptr[0]=='0') goto skip0x;
  if (!base) {
    if (*nptr=='0') {
      base=8;
skip0x:
      if (nptr[1]=='x'||nptr[1]=='X') {
	nptr+=2;
	base=16;
      }
    } else
      base=10;
  }
  orig=nptr;
  while(__likely(*nptr)) {
    register unsigned char c=*nptr;
    c=(c>='a'?c-'a'+10:c>='A'?c-'A'+10:c<='9'?c-'0':0xff);
    if (__unlikely(c>=base)) {
      if (nptr==orig) { if (endptr) *endptr=(char*)nptr; errno=EINVAL; return ULONG_MAX; }
      break;
    }
    {
      register unsigned long x=(v&0xff)*base+c;
      register unsigned long w=(v>>8)*base+(x>>8);
      if (w>(ULONG_MAX>>8)) overflow=1;
      v=(w<<8)+(x&0xff);
    }
    ++nptr;
  }
  if (endptr) *endptr=(char *)nptr;
  if (overflow) {
    errno=ERANGE;
    return ULONG_MAX;
  }
  if (v==ULONG_MAX) errno=0;
  return (neg?-v:v);
}
