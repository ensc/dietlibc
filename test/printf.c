#include <stdio.h>

main() {
  int i;
  for (i=0; i<512/4-1; ++i) write(1,"foo ",4);
  i=printf("foo %d\n",23);
  fprintf(stderr,"got %d\n",i);
//  fprintf(stderr,"fflush returned %d\n",fflush(stdout));
}
