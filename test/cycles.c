#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>

extern char **environ;

int main(int argc,char* argv[]) {
  uint64_t a,b;
  asm volatile("rdtsc":"=A"(a));
  if (!fork()) { execve(argv[1],argv+1,environ); exit(1); }
  wait(0);
  asm volatile("rdtsc":"=A"(b));
  printf("%llu cycles\n",b-a);

  return 0;
}
