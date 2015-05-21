#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

int main() {
  char buf[L_cuserid];
  assert(cuserid(buf) == buf);
  puts(buf);
  return 0;
}
