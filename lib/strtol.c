#include <ctype.h>

extern unsigned long int strtoul(const char *nptr, char **endptr, int base);

long int strtol(const char *nptr, char **endptr, int base)
{
  int neg=0;
  unsigned long int v;

  while(isspace(*nptr)) nptr++;

  if (*nptr == '-' && isdigit(nptr[1])) { neg=-1; nptr++; }
  v=strtoul(nptr,endptr,base);
  return (neg?-v:v);
}
