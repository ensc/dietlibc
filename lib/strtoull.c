#include <ctype.h>
#include <stdlib.h>

unsigned long long int strtoull(const char *nptr, char **endptr, int base)
{
  long long int v=0;

  while(isspace(*nptr)) ++nptr;

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
  while(*nptr) {
    register unsigned char c=*nptr;
    c=(c>='a'?c-'a'+10:c>='A'?c-'A'+10:c-'0');
    if (c>=base) break;
    v=v*base+c;
    ++nptr;
  }
  if (endptr) *endptr=(char *)nptr;
  return v;
}
