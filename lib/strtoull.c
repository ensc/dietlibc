#include <ctype.h>

static char *num="0123456789abcdefghijklmnopqrstuvwxyz";

unsigned long long int strtoull(const char *nptr, char **endptr, int base)
{
  long long int v=0;
  int a=1,i;

  while(isspace(*nptr)) nptr++;

  if (*nptr == '+') { nptr++; }
  if (!base)
  {
    if (*nptr=='0')
    {
      base=8;
      if ((*(nptr+1)=='x')||(*(nptr+1)=='X')) base=16;
    }
    else
      base=10;
  }
  while((a)&&(*nptr))
  {
    a=0;
    for (i=0;(!a)&&(i<base);i++)
    {
      if ((*nptr == num[i]) || (*nptr == (num[i]-' ')))
      {
	a=1;
	v=(v*base)+(&num[i]-num);
      }
    }
    if ((!a)&&(base==16)&&((*nptr=='x')||(*nptr=='X'))) a=1;
    if (a) nptr++;
  }
  if (endptr) *endptr=(char *)nptr;
  return v;
}
