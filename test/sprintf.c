#include <string.h>
#include <stdio.h>

main() {
  char buf[1000];
  sprintf(buf,"%d",23);
  if (strcmp(buf,"23")) return 1;
  sprintf(buf,"%d",5);
  if (strcmp(buf,"5")) return 1;
  return 0;
}
