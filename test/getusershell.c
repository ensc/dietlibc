#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>

main() {
  char* c;
  while (c=getusershell())
    puts(c);
}
