#include <ctype.h>
#include <stdlib.h>

long long int strtoll(const char *nptr, char **endptr, int base)
{
  int neg=0;
  unsigned long long int v;

  while(isspace(*nptr)) nptr++;

  if (*nptr == '-' && isdigit(nptr[1])) { neg=-1; nptr++; }
  v=strtoull(nptr,endptr,base);
  return (neg?-v:v);
}
