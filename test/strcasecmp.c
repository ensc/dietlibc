#include <stdio.h>
#include <stdlib.h>

void die(const char* message) {
  puts(message);
  exit(1);
}

main() {
  char buf[100]="fNord";
  char buf2[100]="fnOrt";
  if (strcasecmp(buf,buf)) die("strcmp say a != a");
  if (strcasecmp(buf,buf2)>=0) die("strcmp said fnord > fnort");
  if (strcasecmp(buf2,buf)<=0) die("strcmp said fnort < fnord");
  return 0;
}
