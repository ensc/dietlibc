#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char* argv[]) {
  long a,b;
  asm("rdtsc":"=A"(a));
  if (!fork()) { execve(argv[1],argv+1,environ); exit(1); }
  wait(0);
  asm("rdtsc":"=A"(b));
  printf("%lu cycles\n",b-a);
}
