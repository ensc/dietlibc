#include <string.h>

char *strtok_r(char *s, const char *delim, char **ptrptr)
{
  int i;
  char *tmp=0;

  if (s) (*ptrptr)=s;

  if (**ptrptr)
  {
    while(!(i=strcspn(*ptrptr,delim))) (*ptrptr)++;
    if (**ptrptr)
    {
      tmp=(*ptrptr);
      (*ptrptr)+=i;
      if (**ptrptr) *(*ptrptr)++=0;
    }
  }
  return tmp;
}
