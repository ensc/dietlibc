#define _GNU_SOURCE
#include <string.h>
#include <ctype.h>

int strverscmp(const char* s1, const char* s2)
{
  size_t i,j;
  long long a,b;
  b=0;
  for (i=0; s1[i]==s2[i]; ++i) {
    if (!s1[i]) return 0;	/* perfect match */
  }

  /* if we get here, there is a mismatch */
  /* backtrack over the matching digits */
  if (!isdigit(s1[i]) || !isdigit(s2[i]))
    return s1[i]-s2[i];
  for (j=i; j>0 && isdigit(s1[j-1]); --j) ;
  for (a=0; j<i; ++j) {
    a=a*10+s1[j]-'0';
  }
  b=a;
  while (isdigit(s1[i])) {
    a=a*10+s1[i]-'0';
    ++i;
  }
  while (isdigit(s2[j])) {
    b=b*10+s2[j]-'0';
    ++j;
  }
  return a<b ? -1 : 1;
}
