#include <stdio.h>

main() {
  int i;
  if (printf("foo")!=3) return 1;
  return 0;
#if 0
  i=printf("foo %d\n",23);
  fprintf(stderr,"got %d\n",i);
  fprintf(stderr,"got %d\n",printf("foo"));
#endif
//  fprintf(stderr,"fflush returned %d\n",fflush(stdout));
}
