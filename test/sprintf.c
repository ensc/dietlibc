#include <string.h>
#include <stdio.h>

main() {
  char buf[1000];
  sprintf(buf,"%d",23);
  if (strcmp(buf,"23")) return 1;
  sprintf(buf,"%d",5);
  if (strcmp(buf,"5")) return 1;
  sprintf(buf,"%.2f", 0.05);
  if (strcmp(buf,"0.05")) return 1;
  sprintf(buf,"%f", 9e-6);
  if (strcmp(buf,"0.000009")) return 1;
  sprintf(buf,"%f", 1e-2);
  if (strcmp(buf,"0.010000")) return 1;
  return 0;
}
