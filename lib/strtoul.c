#include <ctype.h>
#include "dietfeatures.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

unsigned long int strtoul(const char *nptr, char **endptr, int base)
{
  unsigned long int v=0;

  while(isspace(*nptr)) ++nptr;
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
  while(*nptr) {
    register unsigned char c=*nptr;
    c=(c>='a'?c-'a'+10:c>='A'?c-'A'+10:c<='9'?c-'0':0xff);
    if (c>=base) break;
    {
      register unsigned long int w=v*base;
      if (w<v) {
	errno=ERANGE;
	return ULONG_MAX;
      }
      v=w+c;
    }
    ++nptr;
  }
  if (endptr) *endptr=(char *)nptr;
  errno=0;	/* in case v==ULONG_MAX, ugh! */
  return v;
}
