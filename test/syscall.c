#include <asm/unistd.h>

int main() {
  syscall(__NR_write,1,"foo\n",4);
  return 0;
}
