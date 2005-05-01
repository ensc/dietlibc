#include <asm/unistd.h>
#include <stdio.h>
#include <errno.h>

int main() {
  int a,b;
  syscall(__NR_write,1,"foo\n",4);
  a=syscall(__NR_write,23,"bar\n",4);
  b=errno;
  printf("%d %d\n",a,b);
  return 0;
}
