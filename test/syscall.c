#include <asm/unistd.h>

main() {
  syscall(__NR_write,1,"foo\n",4);
}
