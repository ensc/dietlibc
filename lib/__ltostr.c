#include <string.h>

static char *num="0123456789abcdefghijklmnopqrstuvwxyz";

int __ltostr(char *s, int size, unsigned long i, int base, char UpCase)
{
  char *tmp;
  int j=0;

  s[--size]=0;

  tmp=s+size;

  if ((base==0)||(base>36)) base=10;

  j=0;
  if (!i)
  {
    *(--tmp)='0';
    j=1;
  }

  while((tmp>s)&&(i))
  {
    tmp--;
    *tmp=num[i%base];
    if ((UpCase)&&(*tmp>96)) (*tmp)-=' ';
    i=i/base;
    j++;
  }
  memmove(s,tmp,j+1);

  return j;
}
