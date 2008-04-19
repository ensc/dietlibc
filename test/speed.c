#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

int main() {
  int i;
  time_t t;
  
  printf("%ld\n", (long)time(0));
  for (i=0; i<10000000; ++i)
    t=time(0);

  printf("%ld\n", (long)time(0));
 return 0;   
}
