#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
  char buf[100];
  int i;
  if (printf("foo")!=3) return 1;
  sprintf(buf,"%8.7lx",0xfefe);
  printf("%8.7lx",0xfefe);
  assert (0 == strcmp(buf," 000fefe"));

  i=printf("foo %d\n",23);
  fprintf(stderr,"got %d\n",i);
  fprintf(stderr,"got %d\n",printf("foo"));

  fprintf(stderr,"fflush returned %d\n",fflush(stdout));

  return 0;
}
